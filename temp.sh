#!/usr/bin/env bash
set -euo pipefail

# Usage: ./prepare_and_emit_instrument_script.sh /path/to/dir
TARGET_DIR="${1:-$(pwd)}"
TARGET_DIR="$(cd "$TARGET_DIR" && pwd)"

OUT_SCRIPT="./run_instrument.sh"

echo "Preparing per-file dirs under: $TARGET_DIR"
echo "Writing instrument commands to: $OUT_SCRIPT"
echo

# header for the generated script
cat > "$OUT_SCRIPT" <<'HEADER'
#!/usr/bin/env bash
set -euo pipefail

# Auto-generated instrument commands.
# Run this script to invoke ACE instrumentation for each file.
HEADER

# counter
count=0

# process only files at top-level (not recursive). Use -print0 to be safe with names.
find "$TARGET_DIR" -maxdepth 1 -type f -print0 | while IFS= read -r -d '' file; do
  filename="$(basename "$file")"
  # folder name: basename without extension; change if you want to keep extension
  foldername="${filename%.*}"
  [ -z "$foldername" ] && foldername="$filename"

  dest_root="$TARGET_DIR/$foldername"
  src_dir="$dest_root/src"
  instr_dir="$dest_root/instr"

#   mkdir -p "$src_dir" "$instr_dir"

  # copy original file into src (preserve attributes)
#   cp -a -- "$file" "$src_dir/"

  # canonical absolute paths
  src_dir_abs="$(cd "$src_dir" && pwd)"
  instr_dir_abs="$(cd "$instr_dir" && pwd)"

  # shell-escape the paths for safe embedding into the generated script
  esc_src="$(printf '%q' "$src_dir_abs")"
  esc_instr="$(printf '%q' "$instr_dir_abs")"

  # build the command line
  cmd="python3 ACE.py instrument \\
    --src_dir $esc_src \\
    -out_dir $esc_instr \\
    --instr_languages c,cpp,python,java"
    # append to the output script
    {
        printf '\n# For file: %s\n' "$filename"
        printf '%s\n' "$cmd"
    } >> "$OUT_SCRIPT"

    count=$((count + 1))
    done

    # make the generated script executable
    chmod +x "$OUT_SCRIPT"

    echo "Done. Wrote $count command(s) to $OUT_SCRIPT"
    echo "Run: $OUT_SCRIPT  to execute the instrumentation commands (script is executable)."