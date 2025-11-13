#!/usr/bin/env bash
set -euo pipefail

# ============================================
# This script is called by ACE with:
#   (1) to compute global coverage:
#         coverage.sh <src_dir> <out_dir>
#
#   (2) to compute line coverage:
#         coverage.sh <src_dir> <out_dir> <relative_path> <line_no> <line_content>
#
# Your script MUST support both.
# ============================================

# --- Required arguments ---
SRC_DIR="$1"
OUT_DIR="$2"

mkdir -p "$OUT_DIR"
cd "$OUT_DIR"

# --------------------------------
# CLEAN OLD COVERAGE DATA
# --------------------------------
find . -name '*.gcda' -delete 2>/dev/null || true
find . -name '*.gcno' -delete 2>/dev/null || true
find . -name '*.gcov' -delete 2>/dev/null || true

# --------------------------------
# COMPILE THE PROJECT FOR COVERAGE
# --------------------------------
CFILE=$(find "$SRC_DIR" -maxdepth 1 -name '*.c' | head -n 1)
BIN_NAME=$(basename "$CFILE" .c)

gcc --coverage -O0 -g -o "$BIN_NAME" "$CFILE"

# Run once to generate initial .gcda files
"./$BIN_NAME" >/dev/null 2>&1 || true

# ============================================
# CASE 1: Only global coverage requested
# ============================================
if [[ $# -eq 2 ]]; then
    cov_data=$(gcovr -r "$SRC_DIR" -s | grep "[lb][a-z]*:")
    l_per=$(echo "$cov_data" | grep lines | cut -d" " -f2 | tr -d '%')
    l_abs=$(echo "$cov_data" | grep lines | awk '{print $3}' | tr -d '()')
    b_per=$(echo "$cov_data" | grep branch | cut -d" " -f2 | tr -d '%')
    b_abs=$(echo "$cov_data" | grep branch | awk '{print $3}' | tr -d '()')

    echo "$l_per,$l_abs,$b_per,$b_abs,0"
    exit 0
fi

# ============================================
# CASE 2: Specific line coverage requested
# ============================================
RELATIVE_PATH="$3"
LINE_NO="$4"
LINE_CONTENT="$5"

# Must cd into the directory containing the file
cd "$SRC_DIR"
gcov -r -b -o "$OUT_DIR" "$RELATIVE_PATH" >/dev/null 2>&1

GCOV_FILE="$(basename "$RELATIVE_PATH").gcov"
if [[ ! -f "$GCOV_FILE" ]]; then
    echo "0,0,0,0,0"
    exit 0
fi

# Search line in +/- 30 lines window
SEARCH_RANGE=30
LOWER=$((LINE_NO - SEARCH_RANGE))
UPPER=$((LINE_NO + SEARCH_RANGE))

# Normalize whitespace
WANT="$(echo "$LINE_CONTENT" | sed 's/^[ \t]*//;s/[ \t]*$//' | tr -s ' ')"

covered_times=$(awk -v lb="$LOWER" -v ub="$UPPER" -v want="$WANT" '
    BEGIN {
        gsub(/^[ \t]+|[ \t]+$/, "", want)
        gsub(/[ \t]+/, " ", want)
        found = 0
    }
    {
        # Extract gcov fields
        p1 = index($0, ":")
        p2 = index(substr($0, p1+1), ":")
        if (!p1 || !p2) next
        p2 += p1

        count = substr($0, 1, p1-1)
        lineno = substr($0, p1+1, p2-p1-1) + 0
        code = substr($0, p2+1)

        if (lineno >= lb && lineno <= ub) {
            gsub(/^[ \t]+|[ \t]+$/, "", code)
            gsub(/[ \t]+/, " ", code)
            if (code == want) {
                gsub(/^[ \t]+/, "", count)
                if (count ~ /^[-#]/) print 0
                else print count + 0
                found = 1
                exit
            }
        }
    }
    END {
        if (!found) print 0
    }
' "$GCOV_FILE")

# Return same 5-field format
cov_data=$(gcovr -r "$SRC_DIR" -s | grep "[lb][a-z]*:")
l_per=$(echo "$cov_data" | grep lines | cut -d" " -f2 | tr -d '%')
l_abs=$(echo "$cov_data" | grep lines | awk '{print $3}' | tr -d '()')
b_per=$(echo "$cov_data" | grep branch | cut -d" " -f2 | tr -d '%')
b_abs=$(echo "$cov_data" | grep branch | awk '{print $3}' | tr -d '()')

echo "$l_per,$l_abs,$b_per,$b_abs,$covered_times"
