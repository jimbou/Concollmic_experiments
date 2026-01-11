#!/usr/bin/env python3
import json
import re
import argparse
from pathlib import Path
from typing import Dict, Set


# Match "52:" anywhere in line
LINE_RE = re.compile(r"\b(\d+):")


# -------------------------------------------------
# Normalize bc benchmark → source file stem
# bc_parse → parse
# -------------------------------------------------
def normalize_bc_bench(name: str) -> str:
    if name.startswith("bc_"):
        return name[len("bc_"):]
    return name


# -------------------------------------------------
# Load coverable-lines JSON (bc schema)
# Map: stem -> {total, set(lines)}
# -------------------------------------------------
def load_coverable_map(path: Path) -> Dict[str, Dict]:
    with open(path, "r", encoding="utf-8") as f:
        raw = json.load(f)

    out = {}
    for filename, info in raw.items():
        stem = Path(filename).stem  # parse.c -> parse

        lines = set(
            map(
                int,
                info.get(
                    "coverable_line_numbers",
                    info.get("coverable_linenos", []),
                ),
            )
        )

        total = int(info.get("total_coverable_lines", len(lines)))

        out[stem] = {
            "total": total,
            "lines": lines,
        }

    return out


# -------------------------------------------------
# Pick newest ConcoLLMic log
# -------------------------------------------------
def find_latest_log(out_dir: Path) -> Path | None:
    logs = sorted(
        out_dir.glob("ConcoLLMic_*.log"),
        key=lambda p: p.stat().st_mtime,
        reverse=True,
    )
    return logs[0] if logs else None


# -------------------------------------------------
# Extract covered lines from log
# -------------------------------------------------
def extract_covered_lines(log: Path, coverable: Set[int]) -> Set[int]:
    covered = set()

    with open(log, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            m = LINE_RE.search(line)
            if not m:
                continue

            ln = int(m.group(1))
            if ln in coverable:
                covered.add(ln)

    return covered


# -------------------------------------------------
# Main
# -------------------------------------------------
def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--logs-root", type=Path, required=True)
    ap.add_argument(
        "--coverable-json",
        type=Path,
        default=Path("/home/jim/ConcoLLMic/bc_all_coverable_lines.json"),
    )
    ap.add_argument("--out", type=Path, default=Path("coverage_bc.json"))
    args = ap.parse_args()

    coverable_map = load_coverable_map(args.coverable_json)

    model = args.logs_root.name
    results = []
    total_covered = 0
    total_coverable = 0

    for bench_dir in sorted(p for p in args.logs_root.iterdir() if p.is_dir()):
        bench_raw = bench_dir.name
        bench = normalize_bc_bench(bench_raw)
        out_dir = bench_dir / "out"

        if not out_dir.is_dir():
            continue

        if bench not in coverable_map:
            print(f"[SKIP] {bench_raw}: no coverable data")
            continue

        log = find_latest_log(out_dir)
        if log is None:
            print(f"[SKIP] {bench_raw}: no log")
            continue

        coverable = coverable_map[bench]["lines"]
        total = coverable_map[bench]["total"]

        covered = extract_covered_lines(log, coverable)
        n_covered = len(covered)

        pct = round((n_covered / total) * 100, 2) if total > 0 else 0.0

        results.append({
            "benchmark": bench_raw,
            "source_file": f"{bench}.c",
            "log": str(log),
            "covered_lines": n_covered,
            "total_coverable_lines": total,
            "coverage_percent": pct,
        })

        total_covered += n_covered
        total_coverable += total

        print(f"[OK] {bench_raw}: {n_covered}/{total} ({pct}%)")

    avg = round(
        sum(b["coverage_percent"] for b in results) / len(results), 2
    ) if results else 0.0

    weighted = round(
        (total_covered / total_coverable) * 100, 2
    ) if total_coverable > 0 else 0.0

    final = {
        model: {
            "total_benchmarks": len(results),
            "average_coverage_percent": avg,
            "total_covered_lines": total_covered,
            "total_coverable_lines": total_coverable,
            "weighted_coverage_percent": weighted,
            "benchmarks": results,
        }
    }

    with open(args.out, "w", encoding="utf-8") as f:
        json.dump(final, f, indent=2)

    print(f"\n[WRITE] {args.out.resolve()}")


if __name__ == "__main__":
    main()
