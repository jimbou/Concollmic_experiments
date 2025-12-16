#!/usr/bin/env bash
set -euo pipefail

DIR="$1"

find "$DIR" -type f -name "*.c" | while read -r file; do
  awk '
    BEGIN {
      state = 0        # 0=outside, 1=seen main signature (before first {), 2=inside main body
      depth = 0
      changed = 0
    }

    function comment_line(line) {
      # If already //-commented (ignoring leading whitespace), keep as-is.
      if (line ~ /^[[:space:]]*\/\//) return line
      return "// " line
    }

    {
      line = $0

      # Detect main start, even if already commented: optional leading // then int|void main(
      if (state == 0 && line ~ /^[[:space:]]*(\/\/[[:space:]]*)?(int|void)[[:space:]]+main[[:space:]]*\(/) {
        state = 1
        changed = 1
      }

      if (state != 0) {
        # Count braces on the ORIGINAL line (not the commented one) to track scope correctly
        opens  = gsub(/{/, "{", line)
        closes = gsub(/}/, "}", line)

        # Comment the line we output
        print comment_line($0)

        # Update depth and state transitions
        if (state == 1) {
          # Wait until we actually see the opening brace of the function body
          if (opens > 0) {
            depth += opens - closes
            state = 2
            # Handle weird case: main body opens and closes immediately on same line
            if (depth <= 0) { state = 0; depth = 0 }
          }
        } else if (state == 2) {
          depth += opens - closes
          if (depth <= 0) { state = 0; depth = 0 }
        }

        next
      }

      # Normal line outside main
      print $0
    }
  ' "$file" > "$file.tmp"

  if ! diff -q "$file" "$file.tmp" >/dev/null; then
    mv "$file.tmp" "$file"
    echo "Commented main in: $file"
  else
    rm -f "$file.tmp"
  fi
done
