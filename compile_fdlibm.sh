# #!/usr/bin/env bash
# set -euo pipefail

# FDLIBM_ROOT="/home/jim/ConcoLLMic/fdlibm/"
# INCLUDE_ROOT="/home/jim/ConcoLLMic/fdlibm"
# echo "🔨 Compiling all instr C files into temp_executable"
# echo

# find "$FDLIBM_ROOT" -type d -name instr | while read -r instr_dir; do
#   echo "📂 Entering: $instr_dir"

#   cfile=$(find "$instr_dir" -maxdepth 1 -name "*.c" | head -n 1)

#   if [[ -z "$cfile" ]]; then
#     echo "⚠️  No .c file found in $instr_dir"
#     echo
#     continue
#   fi

#   echo " Found source: $cfile"

#   output="$instr_dir/temp_executable"

#   gcc \
#     -I"$INCLUDE_ROOT" \
#     "$cfile" \
#     -lm \
#     -o "$output"

#   echo " Compiled -> $output"
#   echo
# done

# echo " All instr files compiled successfully."

set -euo pipefail

FDLIBM_ROOT="/home/jim/ConcoLLMic/fdlibm"
INCLUDE_ROOT="/home/jim/ConcoLLMic/fdlibm_original"

echo "Compiling all instr C files into temp_executable"
echo

find "$FDLIBM_ROOT" -type d -name instr | while read -r instr_dir; do
    echo "Entering: $instr_dir"

    # the instrumented file
    instr_file=$(find "$instr_dir" -maxdepth 1 -name "*.c" | head -n 1)
    if [[ -z "$instr_file" ]]; then
        echo "No .c file found in $instr_dir"
        echo
        continue
    fi

    base=$(basename "$instr_file")   # e.g. e_acos.c
    output="$instr_dir/temp_executable"

    echo "Found instrumented file: $instr_file"
    echo "Excluding original file: $base"

    # collect all FDLIBM .c files except the one being replaced
    other_sources=$(find "$INCLUDE_ROOT" -maxdepth 1 -name '*.c' ! -name "$base")

    gcc \
      -I"$INCLUDE_ROOT" \
      "$instr_file" \
      $other_sources \
      -o "$output"

    echo "Compiled -> $output"
    echo
done

echo "All instr files compiled successfully."
