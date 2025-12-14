#!/usr/bin/env bash
set -euo pipefail

# Manual interrupt only — timeout will NOT trigger this
trap 'echo "❗ Manual interrupt. Exiting..."; kill 0' INT

# =====================================================
# Batch ACE run execution script with timeout + JSON report (FDLIBM)
# =====================================================

BASE_DIR="/home/jim/ConcoLLMic/fdlibm/"
LOG_DIR="logs_run_fdlibm"
SUMMARY_JSON="run_summary_fdlibm.json"
TIMEOUT_DURATION="15m"
MODEL_NAME="claude"
RESULTS_DIR="/home/jim/ConcoLLMic/results_fdlibm/$MODEL_NAME"

mkdir -p "$RESULTS_DIR" "$LOG_DIR"
echo "[]" > "$SUMMARY_JSON"

# -----------------------------------------------------
# Append entry to JSON summary
# -----------------------------------------------------
append_result() {
  local file="$1"
  local status="$2"
  local log="$3"
  local cmd="$4"

  jq --arg f "$file" --arg s "$status" --arg l "$log" --arg c "$cmd" \
    '. += [{"file":$f,"status":$s,"log":$l,"command":$c}]' "$SUMMARY_JSON" \
    > "${SUMMARY_JSON}.tmp" && mv "${SUMMARY_JSON}.tmp" "$SUMMARY_JSON"
}

# -----------------------------------------------------
# Run ACE on a single instrumented folder
# Returns:
#   0   success
#   124 timeout (from `timeout`)
#   else failure code
# -----------------------------------------------------
run_ace() {
  local instr_dir="$1"

  # harness exists INSIDE each instr folder
  local HARNESS="$instr_dir/driver.py"
  if [[ ! -f "$HARNESS" ]]; then
    echo "⚠️  Missing driver.py in: $instr_dir — skipping"
    return 0
  fi

  local name
  name="$(basename "$(dirname "$instr_dir")")"

  local out_dir="$RESULTS_DIR/$name/out"
  if [[ -d "$out_dir" ]]; then
    return "Output dir exists: $out_dir — skipping"
    local counter=1
    while [[ -d "${out_dir}_$counter" ]]; do
      counter=$((counter + 1))
    done
    out_dir="${out_dir}_$counter"
  fi
  mkdir -p "$out_dir"

  local log_file="$LOG_DIR/${name}_run.log"
  : > "$log_file"

  echo "=============================="
  echo "Running ACE: $name"
  echo "=============================="

  {
    echo "=== ACE run started: $(date) ==="
    echo "Instrumented dir: $instr_dir"
    echo "Harness: $HARNESS"
    echo "Output dir: $out_dir"
    echo
  } > "$log_file"

  local cmd="python3 ACE.py run --project_dir \"$instr_dir\" --execution \"$HARNESS\" --out \"$out_dir\" --plateau_slot 5 --parallel_num 1"
  echo "Command: timeout $TIMEOUT_DURATION $cmd" >> "$log_file"
  echo >> "$log_file"

  # Run with timeout, stream output, NEVER kill batch
  # IMPORTANT: preserve exit status from timeout/command (PIPESTATUS[0])
  timeout "$TIMEOUT_DURATION" bash -c "$cmd" 2>&1 | tee -a "$log_file"
  local status=${PIPESTATUS[0]}

  if [[ $status -eq 0 ]]; then
    echo "✅ SUCCESS: $name" | tee -a "$log_file"
    append_result "$name" "success" "$log_file" "$cmd"
  elif [[ $status -eq 124 ]]; then
    echo "⏰ TIMEOUT: $name (exceeded $TIMEOUT_DURATION)" | tee -a "$log_file"
    append_result "$name" "timeout" "$log_file" "$cmd"
  else
    echo "❌ FAILED: $name (exit code $status)" | tee -a "$log_file"
    append_result "$name" "failed" "$log_file" "$cmd"
  fi

  return "$status"
}

# -----------------------------------------------------
# Find all instrumented directories recursively and run
# -----------------------------------------------------
echo "🔍 Searching recursively under $BASE_DIR for instrumented folders..."
mapfile -t instr_dirs < <(find "$BASE_DIR" -type d -name "instr" | sort)

if [[ ${#instr_dirs[@]} -eq 0 ]]; then
  echo "⚠️  No instrumented folders found under $BASE_DIR."
  exit 1
fi

failed_count=0
success_count=0
timeout_count=0
failed_dirs=()
timeout_dirs=()

for instr_dir in "${instr_dirs[@]}"; do
  if run_ace "$instr_dir"; then
    success_count=$((success_count + 1))
  else
    rc=$?
    if [[ $rc -eq 124 ]]; then
      timeout_count=$((timeout_count + 1))
      timeout_dirs+=("$instr_dir")
    else
      failed_count=$((failed_count + 1))
      failed_dirs+=("$instr_dir")
    fi
  fi
done

# -----------------------------------------------------
# Summary
# -----------------------------------------------------
echo
echo "📜 ACE Run Summary (FDLIBM)"
echo "=============================="
echo "✅ Success: $success_count"
echo "❌ Failed:  $failed_count"
echo "⏰ Timeout: $timeout_count"
echo "=============================="

if ((failed_count > 0)); then
  echo "❌ Failed directories:"
  for d in "${failed_dirs[@]}"; do
    echo "   - $d"
  done
fi

if ((timeout_count > 0)); then
  echo
  echo "⏰ Timed-out directories:"
  for d in "${timeout_dirs[@]}"; do
    echo "   - $d"
  done
fi

echo
echo "🗂 Logs stored in: $LOG_DIR"
echo "📝 JSON summary written to: $SUMMARY_JSON"
jq . "$SUMMARY_JSON"
