#!/usr/bin/env bash
set -euo pipefail

BASE_DIR="/home/jim/ConcoLLMic/fdlibm"

echo "✅ Processing all src/*.c files under:"
echo "   $BASE_DIR"
echo

find "$BASE_DIR" -type f -path "*/src/*.c" | while read -r src_file; do
  dir="$(dirname "$src_file")"
  base="$(basename "$src_file" .c)"
  temp_file="$dir/${base}_temp.c"

  echo "🛠 Processing: $src_file"
  echo "   → Backup: $temp_file"

  # 1️⃣ Create exact backup
  cp "$src_file" "$temp_file"

  # 2️⃣ Strip comments IN PLACE on the original
  perl -0777 -i -pe '
    s{/\*.*?\*/}{}gs;
    s{//.*$}{}gm;
  ' "$src_file"

done

echo
echo "✅ Done:"
echo "   - Original .c files are now comment-free"
echo "   - Full backups exist as *_temp.c"
