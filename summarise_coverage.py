#!/usr/bin/env python3
import os
import sys
import json
import yaml

def safe_load_yaml(path):
    """Load YAML safely, ignoring invalid testcases."""
    try:
        with open(path, "r", encoding="utf-8", errors="replace") as f:
            return yaml.safe_load(f)
    except Exception as e:
        print(f"⚠️  Warning: Failed to load {path}: {e}")
        return None


def classify_trace(text):
    """Check presence of both patterns in a given trace string."""
    if not text:
        return (False, False)

    has_logic = "Logic bomb triggered" in text
    has_normal = "Path without the bomb" in text
    return (has_logic, has_normal)


def combine_classification(logic_any, normal_any):
    if logic_any and normal_any:
        return "both"
    elif logic_any and not normal_any:
        return "no_normal"
    elif normal_any and not logic_any:
        return "no_bomb"
    else:
        return "both_missing"


def main():
    if len(sys.argv) != 3:
        print("Usage: summarize_bomb_coverage.py <OUT_DIR> <SUMMARY_JSON>")
        sys.exit(1)

    OUT_DIR = os.path.abspath(sys.argv[1])
    SUMMARY_JSON = os.path.abspath(sys.argv[2])

    QUEUE_DIR = os.path.join(OUT_DIR, "queue")

    if not os.path.isdir(QUEUE_DIR):
        print(f"❌ No queue directory found: {QUEUE_DIR}")
        sys.exit(1)

    yaml_files = [
        os.path.join(QUEUE_DIR, f)
        for f in os.listdir(QUEUE_DIR)
        if f.endswith(".yaml")
    ]

    if not yaml_files:
        print(f"⚠️ No YAML files found in: {QUEUE_DIR}")
        result_label = "both_missing"
    else:
        # Track if *any* testcase triggered each path
        any_logic = False
        any_normal = False

        for ypath in yaml_files:
            data = safe_load_yaml(ypath)
            if not data:
                continue

            exec_trace = data.get("execution_trace", "")
            # print(f"this is exec trace: {exec_trace} for yaml file {ypath}")
            src_exec_trace = data.get("src_execution_trace", "")
            # print(f"this is src exec trace: {src_exec_trace} for yaml file {ypath   }")

            l1, n1 = classify_trace(exec_trace)
            l2, n2 = classify_trace(src_exec_trace)

            if l1 or l2:
                any_logic = True
            if n1 or n2:
                any_normal = True

        result_label = combine_classification(any_logic, any_normal)

    # Prepare to append to JSON file
    new_entry = {
        "out_dir": OUT_DIR,
        "result": result_label
    }

    # Ensure JSON file exists
    if not os.path.exists(SUMMARY_JSON):
        with open(SUMMARY_JSON, "w") as f:
            json.dump([], f, indent=2)

    # Append
    with open(SUMMARY_JSON, "r") as f:
        arr = json.load(f)

    arr.append(new_entry)

    with open(SUMMARY_JSON, "w") as f:
        json.dump(arr, f, indent=2)

    print(f"✔ Added entry: {new_entry}")


if __name__ == "__main__":
    main()
