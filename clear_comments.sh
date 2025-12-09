#!/usr/bin/env bash
set -euo pipefail

BASE_DIR="/home/jim/ConcoLLMic/fdlibm"

echo "🧹 Removing all C comments from every src/*.c file..."
echo

find "$BASE_DIR" -type d -name src | while read -r srcdir; do
  find "$srcdir" -type f -name "*.c" | while read -r cfile; do
    echo "🧼 Cleaning: $cfile"

    perl -0777 -pe '
      s{/\*.*?\*/}{}gs;   # remove block comments
      s{//.*$}{}gm;       # remove line comments
    ' "$cfile" > "$cfile.tmp"

    mv "$cfile.tmp" "$cfile"
  done
done

echo
echo "✅ All comments removed successfully."
