#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <ROOT_DIR>"
    exit 1
fi

ROOT_DIR="$(realpath "$1")"
SUMMARY_JSON="$ROOT_DIR/coverage_summary_all.json"

echo "======================================="
echo "🔍 Searching for out directories under:"
echo "    $ROOT_DIR"
echo "📄 Summary JSON:"
echo "    $SUMMARY_JSON"
echo "======================================="

# Create a fresh clean JSON file
echo "[]" > "$SUMMARY_JSON"
echo "✔ Created clean results file"

# Find all directories named exactly 'out' or starting with out
mapfile -t OUT_DIRS < <(find "$ROOT_DIR" -type d -name "out*" | sort)

if [[ ${#OUT_DIRS[@]} -eq 0 ]]; then
    echo "⚠ No out directories found under $ROOT_DIR"
    exit 0
fi

echo "Found ${#OUT_DIRS[@]} out directories:"
printf " - %s\n" "${OUT_DIRS[@]}"
echo

# Loop and process each out directory
for OUT in "${OUT_DIRS[@]}"; do
    echo "▶ Processing: $OUT"
    python3 summarise_coverage.py "$OUT" "$SUMMARY_JSON"
    echo
done

echo "======================================="
echo "✔ Finished processing all out directories."
echo "📄 Results stored at:"
echo "    $SUMMARY_JSON"
echo "======================================="
