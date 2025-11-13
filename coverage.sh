#!/usr/bin/env bash
set -euo pipefail

#
# UNIVERSAL COVERAGE SCRIPT
# Works with:
#   - ACE replay (calls with 3 args)
#   - Basic coverage mode (0 or 2 args)
#

# ---------------------------
# 0) INIT
# ---------------------------

covered_times_of_line=0

# ---------------------------
# CASE A: 0 ARGS → Just print summary
# ---------------------------
if [ $# -eq 0 ]; then
    cov_data=$(gcovr -r . -s | grep "[lb][a-z]*:")
    l_per=$(echo "$cov_data" | grep lines | awk '{print $2}' | tr -d '%')
    l_abs=$(echo "$cov_data" | grep lines | awk '{print $3}' | tr -d '()')
    b_per=$(echo "$cov_data" | grep branch | awk '{print $2}' | tr -d '%')
    b_abs=$(echo "$cov_data" | grep branch | awk '{print $3}' | tr -d '()')

    echo "$l_per,$l_abs,$b_per,$b_abs,0"
    exit 0
fi

# ---------------------------
# CASE B: 2 ARGS → compile coverage binary
#        (ACE calls this before replay)
#
#   ARG1 = SRC_DIR
#   ARG2 = OUT_DIR
# ---------------------------
if [ $# -eq 2 ]; then
    SRC_DIR="$1"
    OUT_DIR="$2"

    mkdir -p "$OUT_DIR"
    cd "$OUT_DIR"

    # Clean
    find . -name '*.gcda' -delete 2>/dev/null || true
    find . -name '*.gcno' -delete 2>/dev/null || true
    find . -name '*.gcov' -delete 2>/dev/null || true

    CFILE=$(find "$SRC_DIR" -maxdepth 1 -name '*.c' | head -n 1)
    BIN=$(basename "$CFILE" .c)

    gcc --coverage -O0 -g -o "$BIN" "$CFILE"

    exit 0
fi

# ---------------------------
# CASE C: 3 ARGS → ACE test-case-specific coverage query
#
#   ARG1 = relative_path
#   ARG2 = line_no
#   ARG3 = line_content
# ---------------------------

if [ $# -eq 3 ]; then
    relative_path="$1"
    line_no="$2"
    line_content="$3"

    if [[ "$relative_path" == "None" ]]; then
        # ACE sometimes passes None None None → skip gracefully
        echo "0,0,0,0,0"
        exit 0
    fi

    if [ ! -f "$relative_path" ]; then
        echo "File $relative_path does not exist"
        echo "0,0,0,0,0"
        exit 0
    fi

    src_dir=$(dirname "$relative_path")
    base=$(basename "$relative_path")

    cd "$src_dir"
    gcov -r -b -o . "$base" >/dev/null 2>&1 || true

    gcov_file="$base.gcov"
    if [ ! -f "$gcov_file" ]; then
        echo "0,0,0,0,0"
        exit 0
    fi

    # coverage summary
    cov_data=$(gcovr -r . -s | grep "[lb][a-z]*:")
    l_per=$(echo "$cov_data" | grep lines | awk '{print $2}' | tr -d '%')
    l_abs=$(echo "$cov_data" | grep lines | awk '{print $3}' | tr -d '()')
    b_per=$(echo "$cov_data" | grep branch | awk '{print $2}' | tr -d '%')
    b_abs=$(echo "$cov_data" | grep branch | awk '{print $3}' | tr -d '()')

    # count hits
    covered_times_of_line=$(grep -F "$line_content" "$gcov_file" \
        | awk '{print $1}' | grep -v '[-#]' | head -n 1)
    covered_times_of_line=${covered_times_of_line:-0}

    echo "$l_per,$l_abs,$b_per,$b_abs,$covered_times_of_line"
    exit 0
fi

# ---------------------------
# FALLBACK
# ---------------------------
echo "0,0,0,0,0"
exit 0
