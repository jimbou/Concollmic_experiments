#!/usr/bin/env python3
import json
from pathlib import Path

INPUT_JSON = "/home/jim/ConcoLLMic/results_coverage_deepseek.json"
OUTPUT_JSON = "/home/jim/ConcoLLMic/results_coverage_deepseek_numeric.json"

# Scoring rules
POINTS = {
    "both": 2,
    "both_missing": 2,
    "no_bomb": 1,
    "no_normal": 1
}

def main():
    data = json.loads(Path(INPUT_JSON).read_text())

    total_items = len(data)

    # Count each result type
    counts = {
        "both": 0,
        "both_missing": 0,
        "no_bomb": 0,
        "no_normal": 0
    }

    total_points = 0
    for item in data:
        result = item["result"]
        if result not in counts:
            print(f"Warning: unknown result type {result}")
            continue
        counts[result] += 1
        total_points += POINTS[result]

    potential_points = total_items * 2
    percentage = (total_points / potential_points * 100) if potential_points > 0 else 0

    out = {
        "total_items": total_items,
        "counts": counts,
        "total_points": total_points,
        "potential_points": potential_points,
        "percentage": percentage
    }

    Path(OUTPUT_JSON).write_text(json.dumps(out, indent=2))
    print(f"Saved summary to {OUTPUT_JSON}")


if __name__ == "__main__":
    main()
