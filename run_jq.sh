#!/usr/bin/env bash
set +e
set +u
set +o pipefail


# =====================================================
# Batch ACE run execution script (XML / Expat)
# =====================================================

trap 'echo "❗ Interrupted."; exit 130' INT

BASE_DIR="$HOME/ConcoLLMic/jq/ConcoLLMic_bench"
LOG_DIR="logs_run_jq"
SUMMARY_JSON="run_summary_jq.json"
TIMEOUT_DURATION="15m"
MODEL_NAME="claude"
RESULTS_DIR="$HOME/ConcoLLMic/results_jq/$MODEL_NAME"

mkdir -p "$LOG_DIR" "$RESULTS_DIR"
echo "[]" > "$SUMMARY_JSON"

# -----------------------------------------------------
# Append result to JSON
# -----------------------------------------------------
append_result() {
  local file="$1"
  local status="$2"
  local log="$3"
  local cmd="$4"

  jq --arg f "$file" \
     --arg s "$status" \
     --arg l "$log" \
     --arg c "$cmd" \
     '. += [{"benchmark":$f,"status":$s,"log":$l,"command":$c}]' \
     "$SUMMARY_JSON" > "${SUMMARY_JSON}.tmp" \
  && mv "${SUMMARY_JSON}.tmp" "$SUMMARY_JSON"
}

# -----------------------------------------------------
# Run ACE on a single benchmark
# -----------------------------------------------------
run_ace() {
  set +e
  local instr_dir="$1"
  local name
  name="$(basename "$(dirname "$instr_dir")")"

  local HARNESS="$instr_dir/driver.py"
  if [[ ! -f "$HARNESS" ]]; then
    echo "⚠️  No driver.py in $instr_dir — skipping"
    return 0
  fi

  local out_dir="$RESULTS_DIR/$name/out"
  # mkdir -p "$out_dir"
  #if out_dir exists delete it
  rm -rf "$out_dir"

  local log_file="$LOG_DIR/${name}.log"
  : > "$log_file"

  echo "=============================="
  echo "Running bc benchmark: $name"
  echo "=============================="

  {
    echo "Benchmark: $name"
    echo "Instr dir: $instr_dir"
    echo "Harness: $HARNESS"
    echo "Output: $out_dir"
    echo "Start: $(date)"
    echo
  } > "$log_file"

  local cmd="python3 ACE.py run \
    --project_dir \"$instr_dir\" \
    --execution \"$HARNESS\" \
    --out \"$out_dir\" \
    --plateau_slot 5 \
    --parallel_num 1"

  echo "Command:" >> "$log_file"
  echo "$cmd" >> "$log_file"
  echo >> "$log_file"

  timeout "$TIMEOUT_DURATION" bash -c "$cmd" 2>&1 | tee -a "$log_file"
  local status=${PIPESTATUS[0]}

  if [[ $status -eq 0 ]]; then
    echo "✅ SUCCESS" | tee -a "$log_file"
    append_result "$name" "success" "$log_file" "$cmd"
  elif [[ $status -eq 124 ]]; then
    echo "⏰ TIMEOUT" | tee -a "$log_file"
    append_result "$name" "timeout" "$log_file" "$cmd"
  else
    echo "❌ FAILED (exit $status)" | tee -a "$log_file"
    append_result "$name" "failed" "$log_file" "$cmd"
  fi
  
  return "$status"
}


# -----------------------------------------------------
# Benchmarks to run
# -----------------------------------------------------
BENCHES=(
  jv
  jv_alloc
  jv_aux
  jv_parse
  jv_unicode
)
  

echo "🔍 Running JQ benchmarks..."
echo

success=0
fail=0
timeout=0

# for bench in "${BENCHES[@]}"; do
#   instr_dir="$BASE_DIR/$bench/instr"

#   if [[ ! -d "$instr_dir" ]]; then
#     echo "⚠️  Missing instr dir: $instr_dir"
#     continue
#   fi

#   if run_ace "$instr_dir"; then
#     ((success++))
#   else
#     rc=$?
#     if [[ $rc -eq 124 ]]; then
#       ((timeout++))
#     else
#       ((fail++))
#     fi
#   fi
# done
for bench in "${BENCHES[@]}"; do
  instr_dir="$BASE_DIR/$bench/instr"

  if [[ ! -d "$instr_dir" ]]; then
    echo "⚠️  Missing instr dir: $instr_dir"
    continue
  fi

  run_ace "$instr_dir"
  rc=$? 

  if [[ $rc -eq 0 ]]; then
    ((success++))
  elif [[ $rc -eq 124 ]]; then
    ((timeout++))
  else
    ((fail++))
  fi
done

# -----------------------------------------------------
# Summary
# -----------------------------------------------------
echo
echo "=============================="
echo " JQ ACE SUMMARY"
echo "=============================="
echo "✅ Success: $success"
echo "❌ Failed:  $fail"
echo "⏰ Timeout: $timeout"
echo "Logs:       $LOG_DIR"
echo "Results:    $RESULTS_DIR"
echo "Summary:    $SUMMARY_JSON"
