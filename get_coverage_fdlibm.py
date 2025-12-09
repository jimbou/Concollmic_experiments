#!/usr/bin/env python3

import json
import re
import argparse
from pathlib import Path


# -------------------------------------------------
# Detect model name from log path
# -------------------------------------------------
def detect_model_name(log_path: Path) -> str:
    p = str(log_path).lower()
    if "deepseek" in p:
        return "deepseek"
    if "claude" in p:
        return "claude"
    if "gpt" in p:
        return "gpt"
    return "unknown"


# -------------------------------------------------
# Remove fprintf(stderr, enter/exit) lines
# -------------------------------------------------
def filter_unwanted_lines(lines: list[str]) -> list[str]:
    filtered = []

    for line in lines:
        low = line.lower()
        if (
            "fprintf" in low
            and "stderr" in low
            and ("enter" in low or "exit" in low)
        ):
            continue
        filtered.append(line)

    return filtered


# -------------------------------------------------
# Extract coverable line numbers + content
# (AFTER filtering and renumbering)
# -------------------------------------------------
def extract_coverable_lines_renumbered(lines: list[str]) -> dict[int, str]:
    coverable = {}

    inside_block_comment = False
    reached_main = False

    for idx, raw in enumerate(lines, start=1):
        line = raw.strip()

        if re.search(r"\bint\s+main\s*\(", raw):
            reached_main = True

        if reached_main:
            break

        if inside_block_comment:
            if "*/" in line:
                inside_block_comment = False
            continue

        if line.startswith("/*"):
            if "*/" not in line:
                inside_block_comment = True
            continue

        if line.startswith("//"):
            continue

        if line.startswith("#include"):
            continue

        if not line:
            continue

        coverable[idx] = raw.rstrip("\n")

    return coverable


# -------------------------------------------------
# Extract newly covered lines from ACE log
# -------------------------------------------------
def extract_covered_lines_from_log(log_path: Path, coverable: dict[int, str]) -> dict[int, str]:
    covered = {}
    pattern = re.compile(r"^\s*(\d+):")

    with open(log_path, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            m = pattern.match(line)
            if not m:
                continue

            lineno = int(m.group(1))
            if lineno in coverable:
                covered[lineno] = coverable[lineno]

    return covered


# -------------------------------------------------
# MAIN
# -------------------------------------------------
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("dir", type=Path, help="Benchmark directory containing src/ and instr/")
    parser.add_argument("log", type=Path, help="ACE run log file")

    args = parser.parse_args()

    base_dir = args.dir
    log_path = args.log

    instr_dir = base_dir / "instr"

    if not instr_dir.exists():
        raise RuntimeError(f"Missing instr directory: {instr_dir}")

    c_files = list(instr_dir.glob("*.c"))
    if not c_files:
        raise RuntimeError(f"No C file found inside: {instr_dir}")

    c_file = c_files[0]

    # -----------------------------------------
    # Read -> filter -> renumber
    # -----------------------------------------
    with open(c_file, "r", encoding="utf-8", errors="ignore") as f:
        original_lines = f.readlines()

    filtered_lines = filter_unwanted_lines(original_lines)

    # -----------------------------------------
    # Compute coverable lines
    # -----------------------------------------
    coverable = extract_coverable_lines_renumbered(filtered_lines)

    # -----------------------------------------
    # Extract covered lines from log
    # -----------------------------------------
    covered = extract_covered_lines_from_log(log_path, coverable)

    # -----------------------------------------
    # Metrics
    # -----------------------------------------
    total_coverable = len(coverable)
    total_covered = len(covered)

    coverage_pct = round(
        (total_covered / total_coverable) * 100, 2
    ) if total_coverable > 0 else 0.0

    # -----------------------------------------
    # Detect model name
    # -----------------------------------------
    model = detect_model_name(log_path)

    # -----------------------------------------
    # Write result JSON
    # -----------------------------------------
    result = {
        "file": c_file.name,
        "total_coverable_lines": total_coverable,
        "coverable_lines": [
            {"line": ln, "content": txt} for ln, txt in sorted(coverable.items())
        ],
        "total_covered_lines": total_covered,
        "covered_lines": [
            {"line": ln, "content": txt} for ln, txt in sorted(covered.items())
        ],
        "coverage_percent": coverage_pct,
    }

    out_file = instr_dir / f"results_{model}.json"

    with open(out_file, "w", encoding="utf-8") as f:
        json.dump(result, f, indent=2)

    print(f"Saved coverage results to: {out_file}")


if __name__ == "__main__":
    main()
