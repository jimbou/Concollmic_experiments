#!/usr/bin/env bash
set -euo pipefail

BASE_DIR="/home/jim/ConcoLLMic/fdlibm"
OUT_CMD_FILE="$BASE_DIR/run_all_instrument_cmds.sh"

# ----------------------------
# BLACKLIST
# ----------------------------
BLACKLIST=(
  "s_matherr.c"
  "s_lib_version.c"
  "s_signgam.c"
  "k_rem_pio2.c"
)

is_blacklisted() {
  local f
  for f in "${BLACKLIST[@]}"; do
    [[ "$1" == "$f" ]] && return 0
  done
  return 1
}

# ----------------------------
# RESET OUTPUT COMMAND FILE
# ----------------------------
: > "$OUT_CMD_FILE"
chmod +x "$OUT_CMD_FILE"

echo "✅ Writing instrument commands to:"
echo "   $OUT_CMD_FILE"
echo

# ----------------------------
# MAIN LOOP
# ----------------------------
for cfile in "$BASE_DIR"/*.c; do
  fname="$(basename "$cfile")"

  if is_blacklisted "$fname"; then
    echo "⏭ Skipping blacklisted: $fname"
    continue
  fi

  base="${fname%.c}"
  target_dir="$BASE_DIR/$base"
  src_dir="$target_dir/src"
  instr_dir="$target_dir/instr"

  echo "📁 Creating layout for: $base"

  mkdir -p "$src_dir" "$instr_dir"
  cp "$cfile" "$src_dir/$fname"

  echo "run_instrument \"$src_dir\" \"$instr_dir\"" >> "$OUT_CMD_FILE"
done

echo
echo "✅ Done."
echo "👉 Now run all instrumentation with:"
echo "   $OUT_CMD_FILE"
