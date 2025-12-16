#!/usr/bin/env bash
set -euo pipefail

############################
# CONFIGURATION
############################

FDLIBM_ROOT="/home/jim/ConcoLLMic/fdlibm_no_comments"
DRIVER="/home/jim/ConcoLLMic/fdlibm_no_comments/driver.c"

EXE="fdlibm_cov_exe"
PROFRAW="fdlibm.profraw"
PROFDATA="fdlibm.profdata"
OUT_JSON="fdlibm_coverable_lines_without_comments.json"
echo "==> Configuration:"
echo "  FDLIBM_ROOT: $FDLIBM_ROOT"
############################
# SANITY CHECKS
############################
# Detect llvm-cov (versioned or unversioned)
LLVM_COV="$(command -v llvm-cov \
    || command -v llvm-cov-17 \
    || command -v llvm-cov-16 \
    || command -v llvm-cov-15 \
    || command -v llvm-cov-14)"

if [[ -z "$LLVM_COV" ]]; then
    echo "ERROR: llvm-cov not found"
    exit 1
fi

if [[ ! -d "$FDLIBM_ROOT" ]]; then
    echo "ERROR: FDLIBM_ROOT not found: $FDLIBM_ROOT"
    exit 1
fi

if [[ ! -f "$DRIVER" ]]; then
    echo "ERROR: driver.c not found: $DRIVER"
    exit 1
fi

if ! command -v llvm-cov >/dev/null; then
    echo "ERROR: llvm-cov not found in PATH"
    exit 1
fi

if ! command -v llvm-profdata >/dev/null; then
    echo "ERROR: llvm-profdata not found in PATH"
    exit 1
fi

############################
# STEP 1: BUILD EXECUTABLE
############################

echo "==> Building coverage-instrumented fdlibm executable"

clang -O0 -g \
  -fprofile-instr-generate \
  -fcoverage-mapping \
  -I "$FDLIBM_ROOT" \
  "$DRIVER" \
  "$FDLIBM_ROOT"/*.c \
  -o "$EXE"

############################
# STEP 2: RUN ONCE
############################

echo "==> Running executable once to generate profile data"

LLVM_PROFILE_FILE="$PROFRAW" "./$EXE" >/dev/null 2>&1 || true

llvm-profdata merge "$PROFRAW" -o "$PROFDATA"

############################
# STEP 3: EXTRACT COVERABLE LINES
############################

echo "==> Extracting llvm-cov coverable lines"

echo "{}" > "$OUT_JSON"

for SRC in "$FDLIBM_ROOT"/*.c; do
    echo "  Processing $(basename "$SRC")"

    mapfile -t LINES < <(
  "$LLVM_COV" show "$EXE" \
    -instr-profile="$PROFDATA" \
    --show-line-counts \
    "$SRC" \
  | awk -F'\\|' '
      # format: lineno | count | source
      # keep only if lineno is a number AND count contains a digit (i.e., executable)
      $1 ~ /^[[:space:]]*[0-9]+[[:space:]]*$/ && $2 ~ /[0-9]/ {
          gsub(/[[:space:]]/, "", $1);
          print $1
      }
    ' | sort -n | uniq
)



    TOTAL=${#LINES[@]}

    jq \
      --arg src "$SRC" \
      --argjson total "$TOTAL" \
      --argjson lines "$(printf '%s\n' "${LINES[@]}" | jq -R . | jq -s .)" \
      '
      .[$src] = {
        total_coverable_lines: $total,
        coverable_lines: $lines
      }
      ' "$OUT_JSON" > "$OUT_JSON.tmp"

    mv "$OUT_JSON.tmp" "$OUT_JSON"
done

############################
# CLEANUP (OPTIONAL)
############################

rm -f "$PROFRAW"

echo "==> DONE"
echo "Results written to: $OUT_JSON"
