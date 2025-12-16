#!/usr/bin/env bash
set -euo pipefail

DIR1="$1"
DIR2="$2"

mkdir -p "$DIR2"

find "$DIR1" -type f | while read -r f; do
    name="$(basename "$f")"

    if ! find "$DIR2" -type f -name "$name" -print -quit | grep -q .; then
        cp "$f" "$DIR2/$name"
        echo "Copied: $name"
    fi
done
