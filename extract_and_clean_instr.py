#!/usr/bin/env python3

import argparse
from pathlib import Path


def should_remove_fprintf_line(line: str) -> bool:
    low = line.lower()
    return (
        "fprintf" in low
        and "stderr" in low
        and ("enter" in low or "exit" in low)
    )


def clean_c_file(lines: list[str]) -> list[str]:
    cleaned = []
    for line in lines:
        # Stop at int main
        if "int main" in line:
            break

        # Skip unwanted fprintf lines
        if should_remove_fprintf_line(line):
            continue

        cleaned.append(line)

    return cleaned


def process_bench(base_dir: Path, dest_dir: Path):
    for bench_dir in base_dir.iterdir():
        if not bench_dir.is_dir():
            continue

        bench_name = bench_dir.name
        instr_dir = bench_dir / "instr"
        src_file = instr_dir / f"{bench_name}.c"

        if not src_file.exists():
            print(f"[SKIP] Missing file: {src_file}")
            continue

        with src_file.open("r", encoding="utf-8") as f:
            lines = f.readlines()

        cleaned_lines = clean_c_file(lines)

        dest_file = dest_dir / f"{bench_name}.c"
        dest_file.parent.mkdir(parents=True, exist_ok=True)

        with dest_file.open("w", encoding="utf-8") as f:
            f.writelines(cleaned_lines)

        print(f"[OK] Wrote {dest_file}")


def main():
    parser = argparse.ArgumentParser(
        description="Extract and clean instr C files from benchmark directories"
    )
    parser.add_argument(
        "base_dir",
        type=Path,
        help="Base directory containing benchmark subdirectories",
    )
    parser.add_argument(
        "dest_dir",
        type=Path,
        help="Destination directory for cleaned C files",
    )

    args = parser.parse_args()

    if not args.base_dir.is_dir():
        raise ValueError(f"Base directory does not exist: {args.base_dir}")

    process_bench(args.base_dir, args.dest_dir)


if __name__ == "__main__":
    main()
