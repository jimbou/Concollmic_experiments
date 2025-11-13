#!/usr/bin/env bash
set -euo pipefail

# ============================================================
# Usage check
# ============================================================

if [[ $# -ne 2 ]]; then
  echo "Usage: $0 <OUT_DIR> <ORIGINAL_C_FILE>"
  exit 1
fi

OUT_DIR="$(realpath "$1")"
C_FILE="$(realpath "$2")"

if [[ ! -d "$OUT_DIR" ]]; then
  echo "❌ OUT_DIR does not exist: $OUT_DIR"
  exit 1
fi
if [[ ! -f "$C_FILE" ]]; then
  echo "❌ C file does not exist: $C_FILE"
  exit 1
fi

SRC_DIR="$(dirname "$C_FILE")"
BENCH_NAME="$(basename "$C_FILE" .c)"

COV_DIR="$OUT_DIR/coverage"
mkdir -p "$COV_DIR"

echo "==========================================="
echo "▶ Running coverage for benchmark: $BENCH_NAME"
echo "▶ OUT_DIR : $OUT_DIR"
echo "▶ SRC_DIR : $SRC_DIR"
echo "▶ C_FILE  : $C_FILE"
echo "==========================================="

# ============================================================
# 1️⃣ Copy the official coverage.sh to SRC_DIR
# ============================================================

MAIN_COV_SCRIPT="/home/jim/ConcoLLMic/coverage.sh"   # <-- your static script

if [[ ! -f "$MAIN_COV_SCRIPT" ]]; then
  echo "❌ Main coverage.sh not found at $MAIN_COV_SCRIPT"
  exit 1
fi

COV_SCRIPT="$SRC_DIR/coverage.sh"

cp "$MAIN_COV_SCRIPT" "$COV_SCRIPT"
chmod +x "$COV_SCRIPT"

echo "✔ Using official coverage.sh from: $MAIN_COV_SCRIPT"
echo "✔ Copied to benchmark dir: $COV_SCRIPT"

# ============================================================
# 2️⃣ Run ACE replay
# ============================================================

echo "▶ Running ACE replay..."
REPLAY_LOG="$OUT_DIR/replay.log"

if ! python3 ACE.py replay \
    "$OUT_DIR" \
    "$SRC_DIR" \
    "$COV_DIR/coverage.csv" \
    --cov_script "$COV_SCRIPT" \
    > "$REPLAY_LOG" 2>&1; then
  echo "❌ Replay failed. See log:"
  echo "   $REPLAY_LOG"
  exit 1
fi

echo "✔ Replay complete"

# ============================================================
# 3️⃣ Find .gcov file
# ============================================================

GCOV_FILE=$(find "$SRC_DIR" -maxdepth 1 -name '*.gcov' | head -n 1 || true)

if [[ -z "$GCOV_FILE" ]]; then
  echo "❌ No gcov file generated"
  exit 1
fi

echo "✔ Found gcov file: $GCOV_FILE"

# ============================================================
# 4️⃣ Parse for specific lines
# ============================================================

LOGIC_COUNT=$(
  grep -E "^\s*[0-9]+" "$GCOV_FILE" \
    | grep -F "Logic bomb triggered" \
    | awk '{print $1}' \
    | grep -v '[-#]' \
    | wc -l
)

NOBOMB_COUNT=$(
  grep -E "^\s*[0-9]+" "$GCOV_FILE" \
    | grep -F "Path without the bomb" \
    | awk '{print $1}' \
    | grep -v '[-#]' \
    | wc -l
)

echo
echo "📊 Coverage result:"
echo "   Logic bomb triggered : $LOGIC_COUNT"
echo "   Path without bomb    : $NOBOMB_COUNT"

# ============================================================
# 5️⃣ Produce JSON summary
# ============================================================

SUMMARY_JSON="$OUT_DIR/coverage_summary.json"

jq -n \
  --arg bench "$BENCH_NAME" \
  --argjson logic "$LOGIC_COUNT" \
  --argjson nobomb "$NOBOMB_COUNT" \
  '{
    benchmark: $bench,
    logic_bomb_triggered: $logic,
    path_without_bomb: $nobomb
  }' > "$SUMMARY_JSON"

echo
echo "✔ Summary written to:"
echo "  $SUMMARY_JSON"

echo
echo "Done."
