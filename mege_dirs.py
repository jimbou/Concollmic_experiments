#!/usr/bin/env python3
import os
import sys
import shutil
from pathlib import Path

def copy_all_subdirs(src, dst):
    """
    Copy all subdirectories from src into dst.
    If a directory already exists in dst, skip it.
    """
    for entry in os.scandir(src):
        if entry.is_dir():
            target = Path(dst) / entry.name
            if target.exists():
                print(f"⚠️  Skipping existing dir: {target}")
            else:
                print(f"📁 Copying: {entry.path} -> {target}")
                shutil.copytree(entry.path, target)

def main():
    if len(sys.argv) != 4:
        print("Usage: merge_copy.py <dir1> <dir2> <dest>")
        sys.exit(1)

    dir1 = Path(sys.argv[1])
    dir2 = Path(sys.argv[2])
    dest = Path(sys.argv[3])

    # Validate input
    if not dir1.is_dir():
        print(f"❌ dir1 is not a directory: {dir1}")
        sys.exit(1)
    if not dir2.is_dir():
        print(f"❌ dir2 is not a directory: {dir2}")
        sys.exit(1)

    # Create dest if not exists
    dest.mkdir(parents=True, exist_ok=True)

    # Copy all from dir1
    print("\n==== Copying from dir1 ====")
    copy_all_subdirs(dir1, dest)

    # Get names that dir1 used
    dir1_subdir_names = {d.name for d in dir1.iterdir() if d.is_dir()}

    # Copy from dir2 except overlaps
    print("\n==== Copying from dir2 ====")
    for entry in os.scandir(dir2):
        if entry.is_dir():
            if entry.name in dir1_subdir_names:
                print(f"⛔ Skipping (already exists in dir1): {entry.name}")
                continue
            target = dest / entry.name
            print(f"📁 Copying: {entry.path} -> {target}")
            shutil.copytree(entry.path, target)

    print("\n✅ Done.")

if __name__ == "__main__":
    main()
