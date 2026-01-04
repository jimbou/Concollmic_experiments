#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

EXPAT_SRC="$ROOT/expat"
BUILD_DIR="$ROOT/build"

TARGETS=(
  xmlparse
  xmltok
  xmltok_impl
  xmltok_ns
  xmlrole
)

echo "=== ConcoLLMic Build Script ==="

for TARGET in "${TARGETS[@]}"; do
  echo ""
  echo ">>> Processing $TARGET"

  ORIG="$EXPAT_SRC/lib/$TARGET.c"
  INSTR_DIR="$ROOT/$TARGET/instr"
  INSTR_FILE="$INSTR_DIR/$TARGET.c"
  OUT_BIN="$INSTR_DIR/temp_executable"
  BACKUP="$ORIG.bak"

  if [[ ! -f "$INSTR_FILE" ]]; then
    echo "  Skipping $TARGET (missing instrumented file)"
    continue
  fi

  echo "  → Injecting instrumented source"
  cp "$ORIG" "$BACKUP"
  cp "$INSTR_FILE" "$ORIG"

  echo "  → Building"
  cmake --build "$BUILD_DIR" -- -j$(nproc)

  echo "  → Saving executable → $OUT_BIN"
  cp "$BUILD_DIR/xmlwf/xmlwf" "$OUT_BIN"
  chmod +x "$OUT_BIN"

  echo "  → Restoring original source"
  mv "$BACKUP" "$ORIG"

  echo "  ✓ Done: $OUT_BIN"
done

echo ""
echo "=== All ConcoLLMic executables built successfully ==="
