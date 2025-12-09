#!/usr/bin/env bash
set -euo pipefail

BASE_DIR="/home/jim/ConcoLLMic/fdlibm"

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

  # ----------------------------
  # REMOVE ALL COMMENTS & COPY TO src
  # ----------------------------
  sed -E '
    :a
    s:/\*([^*]|\*+[^*/])*\*+/::g
    ta
    s://.*::g
  ' "$cfile" > "$src_dir/$fname"

  echo "🧹 Stripped comments → $src_dir/$fname"

  # ----------------------------
  # COPY DRIVER TO instr
  # ----------------------------
  driver="$BASE_DIR/${base}_driver.sh"

  if [[ -f "$driver" ]]; then
    cp "$driver" "$instr_dir/"
    chmod +x "$instr_dir/${base}_driver.sh"
    echo "🚗 Driver copied → $instr_dir/${base}_driver.sh"
  else
    echo "⚠️  Missing driver: ${base}_driver.sh"
  fi

done

echo
echo "✅ All files processed."
