#!/bin/bash

# Root directory (you can change this or pass it as an argument)
ROOT_DIR="$(pwd)"
COPIES_DIR="$ROOT_DIR/copies"

# Create the copies directory if it doesn't exist
mkdir -p "$COPIES_DIR"

# Find all files ending in _annotated.c and copy them
find "$ROOT_DIR" -type f -name '*_annotated.c' | while read -r file; do
    # Extract filename without the _annotated.c suffix
    base_name=$(basename "$file" _annotated.c)
    
    # Copy the file to the copies directory with the new name
    cp "$file" "$COPIES_DIR/${base_name}.c"
done

echo "Copied all *_annotated.c files to $COPIES_DIR with renamed filenames."
