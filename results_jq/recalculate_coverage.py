#!/usr/bin/env python3

import json
import sys
from copy import deepcopy

def recompute(model_blob: dict) -> dict:
    blob = deepcopy(model_blob)

    benches = blob.get("benchmarks", [])
    if not isinstance(benches, list):
        raise ValueError("Expected 'benchmarks' to be a list")

    total_benchmarks = len(benches)
    total_covered = 0
    total_coverable = 0
    percs = []

    for b in benches:
        covered = int(b.get("covered_lines", 0))
        coverable = int(b.get("total_coverable_lines", 0))

        # sanity
        covered = max(0, covered)
        coverable = max(0, coverable)
        if coverable > 0 and covered > coverable:
            covered = coverable

        pct = (covered / coverable * 100.0) if coverable else 0.0
        b["coverage_percent"] = round(pct, 2)

        total_covered += covered
        total_coverable += coverable
        percs.append(pct)

    avg_pct = (sum(percs) / total_benchmarks) if total_benchmarks else 0.0
    weighted_pct = (total_covered / total_coverable * 100.0) if total_coverable else 0.0

    blob["total_benchmarks"] = total_benchmarks
    blob["total_covered_lines"] = total_covered
    blob["total_coverable_lines"] = total_coverable
    blob["average_coverage_percent"] = round(avg_pct, 2)
    blob["weighted_coverage_percent"] = round(weighted_pct, 2)

    return blob


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <results.json>")
        sys.exit(1)

    path = sys.argv[1]

    with open(path, "r") as f:
        data = json.load(f)

    if not isinstance(data, dict):
        raise ValueError("Top-level JSON must be an object")

    updated = {}
    for model_name, model_blob in data.items():
        updated[model_name] = recompute(model_blob)

    # 🔁 overwrite the SAME file
    with open(path, "w") as f:
        json.dump(updated, f, indent=2)

    print(f"[OK] Coverage recomputed in-place: {path}")


if __name__ == "__main__":
    main()
