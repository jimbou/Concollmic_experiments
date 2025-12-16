#!/usr/bin/env python3

import json
import re
import argparse
from pathlib import Path
from typing import Optional


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
# Load coverable-lines JSON
# Build map: bench_name -> { total, lines }
# -------------------------------------------------
def load_coverable_map(path: Path) -> dict[str, dict]:
    with open(path, "r", encoding="utf-8") as f:
        raw = json.load(f)

    out: dict[str, dict] = {}

    for k, v in raw.items():
        stem = Path(k).stem  # e_acos from /.../e_acos.c

        raw_lines = v.get("coverable_lines", [])
        lines: list[int] = []

        for x in raw_lines:
            if not x:
                continue
            try:
                lines.append(int(x))
            except ValueError:
                continue

        lines = sorted(set(lines))
        total = int(v.get("total_coverable_lines", len(lines)))

        out[stem] = {
            "total": total,
            "lines": lines,
        }

    return out


# -------------------------------------------------
# Find log: logs_root/bench/out/*log
# Pick newest if multiple
# -------------------------------------------------
def find_bench_log(log_root: Path, bench: str) -> Optional[Path]:
    out_dir = log_root / bench / "out"
    if not out_dir.exists():
        return None

    logs = list(out_dir.glob("*log"))
    if not logs:
        return None

    logs.sort(key=lambda p: p.stat().st_mtime, reverse=True)
    return logs[0]


# -------------------------------------------------
# Extract covered lines from log
# -------------------------------------------------
def extract_covered_lines(log: Path, coverable: set[int]) -> set[int]:
    covered: set[int] = set()
    pattern = re.compile(r"^\s*(\d+):")

    with open(log, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            m = pattern.match(line)
            if not m:
                continue
            ln = int(m.group(1))
            if ln in coverable:
                covered.add(ln)

    return covered


# -------------------------------------------------
# Build line -> content map from C file
# -------------------------------------------------
def get_line_content(c_file: Path, lines: list[int]) -> dict[int, str]:
    with open(c_file, "r", encoding="utf-8", errors="ignore") as f:
        all_lines = f.readlines()

    out: dict[int, str] = {}
    n = len(all_lines)
    for ln in lines:
        out[ln] = all_lines[ln - 1].rstrip("\n") if 1 <= ln <= n else ""
    return out


# -------------------------------------------------
# Process one benchmark
# -------------------------------------------------
def process_bench(
    bench_dir: Path,
    log_root: Path,
    coverable_map: dict[str, dict],
) -> Optional[dict]:

    bench = bench_dir.name
    instr = bench_dir / "instr"

    if not instr.exists():
        print(f"[SKIP] {bench}: missing instr/")
        return None

    c_files = list(instr.glob("*.c"))
    if not c_files:
        print(f"[SKIP] {bench}: no .c file in instr/")
        return None

    c_file = c_files[0]

    log = find_bench_log(log_root, bench)
    if log is None:
        print(f"[SKIP] {bench}: no log found")
        return None

    if bench not in coverable_map:
        print(f"[SKIP] {bench}: not in coverable-lines JSON")
        return None

    coverable_lines = coverable_map[bench]["lines"]
    total_coverable = coverable_map[bench]["total"]
    coverable_set = set(coverable_lines)

    covered_set = extract_covered_lines(log, coverable_set)
    covered_lines = sorted(covered_set)

    total_covered = len(covered_lines)
    coverage_pct = round(
        (total_covered / total_coverable) * 100, 2
    ) if total_coverable > 0 else 0.0

    model = detect_model_name(log)

    coverable_content = get_line_content(c_file, coverable_lines)
    covered_content = {ln: coverable_content.get(ln, "") for ln in covered_lines}

    result = {
        "bench": bench,
        "file": c_file.name,
        "log": str(log),
        "total_coverable_lines": total_coverable,
        "total_covered_lines": total_covered,
        "coverage_percent": coverage_pct,
        "coverable_lines": [
            {"line": ln, "content": coverable_content[ln]}
            for ln in coverable_lines
        ],
        "covered_lines": [
            {"line": ln, "content": covered_content[ln]}
            for ln in covered_lines
        ],
    }

    out_file = instr / f"results_{model}.json"
    with open(out_file, "w", encoding="utf-8") as f:
        json.dump(result, f, indent=2)

    print(f"[OK] {bench}: {total_covered}/{total_coverable} ({coverage_pct}%)")

    return result


# -------------------------------------------------
# MAIN
# -------------------------------------------------
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--benches_root", type=Path)
    parser.add_argument("--logs_root", type=Path)
    parser.add_argument(
        "--coverable-json",
        type=Path,
        default=Path("/home/jim/ConcoLLMic/fdlibm_coverable_lines_no_comments_no_brace.json"),
    )

    args = parser.parse_args()

    coverable_map = load_coverable_map(args.coverable_json)

    benches = sorted(p for p in args.benches_root.iterdir() if p.is_dir())

    all_results: dict[str, list[dict]] = {}

    for bench_dir in benches:
        res = process_bench(bench_dir, args.logs_root, coverable_map)
        if res is None:
            continue
        model = detect_model_name(Path(res["log"]))
        all_results.setdefault(model, []).append(res)

    # Write combined JSON per model
    for model, results in all_results.items():
        combined = {
            "model": model,
            "total_benches": len(results),
            "average_coverage_percent": round(
                sum(r["coverage_percent"] for r in results) / len(results), 2
            ) if results else 0.0,
            "benches": results,
        }

        out = args.benches_root / f"coverage_all_{model}.json"
        with open(out, "w", encoding="utf-8") as f:
            json.dump(combined, f, indent=2)

        print(f"[ALL] Wrote {out}")


if __name__ == "__main__":
    main()
