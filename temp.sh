#!/usr/bin/env bash
set -euo pipefail

# =====================================================
# Batch ACE run execution script — PRINT COMMANDS ONLY
# =====================================================

BASE_DIR="/home/jim/ConcoLLMic/logic_bombs/copies"
HARNESS="/home/jim/ConcoLLMic/logic_bombs/copies/harness/count.py"
MODEL_NAME="deepseek-chat"   # <-- SET THIS EACH TIME
RESULTS_DIR="/home/jim/ConcoLLMic/results/$MODEL_NAME"
COMMAND_FILE="all_commands.sh"

mkdir -p "$RESULTS_DIR"
echo "#!/usr/bin/env bash" > "$COMMAND_FILE"
echo "# Auto-generated ACE run commands" >> "$COMMAND_FILE"
echo >> "$COMMAND_FILE"

# -----------------------------------------------------
# Generate the ACE command for this instrumented folder
# -----------------------------------------------------
gen_cmd() {
  local instr_dir="$1"
  local name
  name="$(basename "$(dirname "$instr_dir")")_$(basename "$instr_dir")"

  local out_dir="$RESULTS_DIR/$name/out"
  mkdir -p "$out_dir"

  local cmd="python3 ACE.py run --project_dir \"$instr_dir\" --execution \"$HARNESS\" --out \"$out_dir\" --rounds 1 --parallel_num 1"

  # ------- CHANGED HERE (print instead of run) -------
  echo "timeout 15m $cmd" >> "$COMMAND_FILE"
  # ---------------------------------------------------
}

echo "🔍 Searching recursively under $BASE_DIR for instrumented folders..."
mapfile -t instr_dirs < <(find "$BASE_DIR" -type d -name "instr" | sort)

if [[ ${#instr_dirs[@]} -eq 0 ]]; then
  echo "⚠️  No instrumented folders found under $BASE_DIR."
  exit 1
fi

for instr_dir in "${instr_dirs[@]}"; do
  gen_cmd "$instr_dir"
done

chmod +x "$COMMAND_FILE"

echo
echo "📄 All commands saved to: $COMMAND_FILE"
echo "Use: ./all_commands.sh"
