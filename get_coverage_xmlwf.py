#!/usr/bin/env python3
import json
import re
import argparse
from pathlib import Path
from typing import Dict, List, Set


LINE_RE = re.compile(r"^\s*(\d+):")


# -------------------------------------------------
# Load coverable-lines JSON
# Map: xmlrole -> {total, set(lines)}
# -------------------------------------------------
def load_coverable_map(path: Path) -> Dict[str, Dict]:
    with open(path, "r", encoding="utf-8") as f:
        raw = json.load(f)

    out = {}
    for filename, info in raw.items():
        stem = Path(filename).stem  # xmlrole.c -> xmlrole

        lines = set()
        for x in info.get("coverable_lines", []):
            try:
                lines.add(int(x))
            except ValueError:
                pass

        total = int(info.get("total_coverable_lines", len(lines)))

        out[stem] = {
            "total": total,
            "lines": lines,
        }

    return out


# -------------------------------------------------
# Pick newest ConcoLLMic log
# -------------------------------------------------
def find_log(out_dir: Path) -> Path | None:
    logs = sorted(
        out_dir.glob("ConcoLLMic_*.log"),
        key=lambda p: p.stat().st_mtime,
        reverse=True,
    )
    return logs[0] if logs else None


# -------------------------------------------------
# Extract covered lines
# -------------------------------------------------
def extract_covered_lines(log: Path, coverable: Set[int]) -> Set[int]:
    covered = set()
    with open(log, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            m = LINE_RE.match(line)
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
        default=Path("/home/jim/ConcoLLMic/libexpat/xmlwf_coverable_lines.json"),
    )
    ap.add_argument("--out", type=Path, default=Path("coverage_expat.json"))
    args = ap.parse_args()

    coverable_map = load_coverable_map(args.coverable_json)

    results = {}
    totals = {}

    for model_dir in sorted(p for p in args.logs_root.iterdir() if p.is_dir()):
        model = model_dir.name
        results[model] = []
        totals[model] = {"covered": 0, "total": 0}

        for bench_dir in sorted(p for p in model_dir.iterdir() if p.is_dir()):
            bench = bench_dir.name
            out_dir = bench_dir / "out"
            if not out_dir.is_dir():
                continue

            if bench not in coverable_map:
                print(f"[SKIP] {bench}: not in coverable JSON")
                continue

            log = find_log(out_dir)
            if log is None:
                print(f"[SKIP] {model}/{bench}: no log")
                continue

            coverable = coverable_map[bench]["lines"]
            total_coverable = coverable_map[bench]["total"]

            covered = extract_covered_lines(log, coverable)
            n_covered = len(covered)

            pct = round(
                (n_covered / total_coverable) * 100, 2
            ) if total_coverable > 0 else 0.0

            entry = {
                "benchmark": bench,
                "log": str(log),
                "covered_lines": n_covered,
                "total_coverable_lines": total_coverable,
                "coverage_percent": pct,
            }

            results[model].append(entry)
            totals[model]["covered"] += n_covered
            totals[model]["total"] += total_coverable

            print(
                f"[OK] {model}/{bench}: "
                f"{n_covered}/{total_coverable} ({pct}%)"
            )

    # -------------------------------------------------
    # Aggregate
    # -------------------------------------------------
    final = {}
    for model, benches in results.items():
        total_cov = totals[model]["covered"]
        total_tot = totals[model]["total"]

        avg = round(
            sum(b["coverage_percent"] for b in benches) / len(benches), 2
        ) if benches else 0.0

        weighted = round(
            (total_cov / total_tot) * 100, 2
        ) if total_tot > 0 else 0.0

        final[model] = {
            "total_benchmarks": len(benches),
            "average_coverage_percent": avg,
            "total_covered_lines": total_cov,
            "total_coverable_lines": total_tot,
            "weighted_coverage_percent": weighted,
            "benchmarks": benches,
        }

    with open(args.out, "w", encoding="utf-8") as f:
        json.dump(final, f, indent=2)

    print(f"\n[WRITE] {args.out.resolve()}")


if __name__ == "__main__":
    main()
