#!/usr/bin/env bash
set -uo pipefail   # we handle errors manually

# =====================================================
# Batch compilation script for ACE instrumented outputs
# =====================================================

BASE_DIR="/home/jim/ConcoLLMic/logic_bombs/copies/"
INCLUDE_DIR="/home/jim/ConcoLLMic/logic_bombs/include"
LIB_DIR="/home/jim/ConcoLLMic/logic_bombs/lib"
LOG_DIR="logs_compile"
SUMMARY_JSON="compilation_summary.json"

mkdir -p "$LOG_DIR"
echo "[]" > "$SUMMARY_JSON"

# -----------------------------------------------------
# Append entry to JSON summary
# -----------------------------------------------------
append_result() {
  local name="$1"
  local status="$2"
  local log="$3"
  local cmd="$4"
  jq --arg f "$name" --arg s "$status" --arg l "$log" --arg c "$cmd" \
    '. += [{"file":$f,"status":$s,"log":$l,"command":$c}]' "$SUMMARY_JSON" \
    > "${SUMMARY_JSON}.tmp" && mv "${SUMMARY_JSON}.tmp" "$SUMMARY_JSON"
}

# -----------------------------------------------------
# Compile a single .c file
# -----------------------------------------------------
compile_c_file() {
  local c_file="$1"
  local out_dir
  out_dir=$(dirname "$c_file")

  # use parent + instr folder as unique name
  local name
  name="$(basename "$(dirname "$out_dir")")_$(basename "$out_dir")"

  local exe="$out_dir/temp_executable"
  local log_file="$LOG_DIR/${name}_compile.log"

  echo "=============================="
  echo "Compiling: $name"
  echo "=============================="

  {
    echo "=== Compilation started: $(date) ==="
    echo "Source: $c_file"
    echo "Output: $exe"
    echo
  } > "$log_file"

  # -----------------------------
  # Build the compilation command
  # -----------------------------
  local cmd="gcc -o \"$exe\" \"$c_file\" -I\"$INCLUDE_DIR\" -L\"$LIB_DIR\""

  # --- Include extra libraries when headers are detected ---
  if grep -q "aes.h" "$c_file" && [[ -f "$LIB_DIR/aes.c" ]]; then
    cmd+=" \"$LIB_DIR/aes.c\""
  fi
  if grep -q "sha1.h" "$c_file"; then
    if [[ -f "$LIB_DIR/sha1.c" ]]; then
      cmd+=" \"$LIB_DIR/sha1.c\""
    fi
    if [[ -f "$LIB_DIR/crypto_utils.c" ]]; then
      cmd+=" \"$LIB_DIR/crypto_utils.c\""
    fi
  fi
  if grep -q "utils.h" "$c_file" && [[ -f "$LIB_DIR/utils.c" ]]; then
    cmd+=" \"$LIB_DIR/utils.c\""
  fi
  if grep -q "a_tester.h" "$c_file" && [[ -f "$LIB_DIR/a_tester.c" ]]; then
    cmd+=" \"$LIB_DIR/a_tester.c\""
  fi

  # --- Link math if needed ---
  if grep -qE "#[[:space:]]*include[[:space:]]*<math.h>" "$c_file" \
     || grep -qE '\b(sin|cos|tan|log|exp|pow|sqrt|log10|log2|floor|ceil)\s*\(' "$c_file"; then
    cmd+=" -lm"
  fi

  echo "Command: $cmd" >> "$log_file"
  echo >> "$log_file"

  # -----------------------------
  # Execute the compilation
  # -----------------------------
  eval $cmd >> "$log_file" 2>&1
  local status=$?

  if [[ $status -eq 0 ]]; then
    echo "✅ SUCCESS: $name"
    echo "✅ SUCCESS: $name" >> "$log_file"
    append_result "$name" "success" "$log_file" "$cmd"
    return 0
  else
    echo "❌ FAILED: $name (exit code $status)"
    echo "❌ FAILED (exit code $status)" >> "$log_file"
    append_result "$name" "failed" "$log_file" "$cmd"
    return 1
  fi
}

# -----------------------------------------------------
# Find all instr directories recursively and compile
# -----------------------------------------------------
echo "🔍 Searching recursively under $BASE_DIR for instrumented .c files..."
mapfile -t c_files < <(find "$BASE_DIR" -type f -path "*/instr/*.c" | sort)

if [[ ${#c_files[@]} -eq 0 ]]; then
  echo "⚠️  No instrumented .c files found under $BASE_DIR."
  exit 1
fi

failed_count=0
success_count=0
failed_files=()

for c_file in "${c_files[@]}"; do
  if compile_c_file "$c_file"; then
    ((success_count++))
  else
    ((failed_count++))
    failed_files+=("$c_file")
  fi
done

# -----------------------------------------------------
# Summary
# -----------------------------------------------------
echo
echo "📜 Compilation Summary"
echo "=============================="
echo "✅ Success: $success_count"
echo "❌ Failed:  $failed_count"
echo "=============================="

if ((failed_count > 0)); then
  echo "❌ The following files failed to compile:"
  for f in "${failed_files[@]}"; do
    echo "   - $f"
  done
fi

echo
echo "🗂 Logs stored in: $LOG_DIR"
echo "📝 JSON summary written to: $SUMMARY_JSON"
jq . "$SUMMARY_JSON"
