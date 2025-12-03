#!/usr/bin/env bash
set -euo pipefail

# Manual interrupt only — timeout will NOT trigger this
trap 'echo "❗ Manual interrupt. Exiting..."; kill 0' INT

# =====================================================
# Batch ACE run execution script with timeout + JSON report
# =====================================================

BASE_DIR="/home/jim/ConcoLLMic/logic_bombs/copies/ping_csv_klee"
HARNESS="/home/jim/ConcoLLMic/logic_bombs/copies/harness/count.py"
LOG_DIR="logs_run"
SUMMARY_JSON="run_summary.json"
TIMEOUT_DURATION="15m"   # timeout per run
MODEL_NAME="deepseek-chat"   # <-- SET THIS EACH TIME
RESULTS_DIR="/home/jim/ConcoLLMic/results/$MODEL_NAME"
mkdir -p "$RESULTS_DIR"

mkdir -p "$LOG_DIR"
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
# -----------------------------------------------------
run_ace() {
  local instr_dir="$1"
  local name
  name="$(basename "$(dirname "$instr_dir")")_$(basename "$instr_dir")"
  local out_dir="$RESULTS_DIR/$name/out"
  mkdir -p "$out_dir"
  local log_file="$LOG_DIR/${name}_run.log"

  mkdir -p "$(dirname "$log_file")"
  : > "$log_file"

  echo "=============================="
  echo "Running ACE: $name"
  echo "=============================="

  {
    echo "=== ACE run started: $(date) ==="
    echo "Project dir: $HARNESS"
    echo "Instrumented dir: $instr_dir"
    echo "Output dir: $out_dir"
    echo
  } > "$log_file"

  local cmd="python3 ACE.py run --project_dir \"$instr_dir\" --execution \"$HARNESS\" --out \"$out_dir\" --rounds 1 --parallel_num 1"
  echo "Command: timeout $TIMEOUT_DURATION $cmd" >> "$log_file"
  echo >> "$log_file"

  # ============================================================
  # ✔ Run with timeout, but NEVER exit the whole script
  # ============================================================
  # ============================================================
  # Run with timeout, show output on terminal, log to file
  # ============================================================
  timeout "$TIMEOUT_DURATION" bash -c "$cmd" 2>&1 | tee -a "$log_file"
  local status=${PIPESTATUS[0]}   # Exit code from timeout


  if [[ $status -eq 0 ]]; then
    echo "✅ SUCCESS: $name"
    echo "✅ SUCCESS: $name" >> "$log_file"
    append_result "$name" "success" "$log_file" "$cmd"

  elif [[ $status -eq 124 ]]; then
    echo "⏰ TIMEOUT: $name (exceeded $TIMEOUT_DURATION)"
    echo "⏰ TIMEOUT: process exceeded $TIMEOUT_DURATION" >> "$log_file"
    append_result "$name" "timeout" "$log_file" "$cmd"

  else
    echo "❌ FAILED: $name (exit code $status)"
    echo "❌ FAILED (exit code $status)" >> "$log_file"
    append_result "$name" "failed" "$log_file" "$cmd"
  fi

  return 0   # <== ALWAYS return 0 so the for-loop keeps going
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
    ((success_count++))
  else
    # Determine if timeout
    if grep -q "⏰ TIMEOUT" "$LOG_DIR/$(basename "$(dirname "$instr_dir")")_instr_run.log" 2>/dev/null; then
      ((timeout_count++))
      timeout_dirs+=("$instr_dir")
    else
      ((failed_count++))
      failed_dirs+=("$instr_dir")
    fi
  fi
done

# -----------------------------------------------------
# Summary
# -----------------------------------------------------
echo
echo "📜 ACE Run Summary"
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
