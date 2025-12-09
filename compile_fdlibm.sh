#!/usr/bin/env bash
set -euo pipefail

FDLIBM_ROOT="/home/jim/ConcoLLMic/fdlibm/"
INCLUDE_ROOT="/home/jim/ConcoLLMic/fdlibm"
echo "🔨 Compiling all instr C files into temp_executable"
echo

find "$FDLIBM_ROOT" -type d -name instr | while read -r instr_dir; do
  echo "📂 Entering: $instr_dir"

  cfile=$(find "$instr_dir" -maxdepth 1 -name "*.c" | head -n 1)

  if [[ -z "$cfile" ]]; then
    echo "⚠️  No .c file found in $instr_dir"
    echo
    continue
  fi

  echo "✅ Found source: $cfile"

  output="$instr_dir/temp_executable"

  gcc \
    -I"$INCLUDE_ROOT" \
    "$cfile" \
    -lm \
    -o "$output"

  echo "✅ Compiled -> $output"
  echo
done

echo "🎉 All instr files compiled successfully."
