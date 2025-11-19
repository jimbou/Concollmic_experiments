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
    if not text:
        return (False, False)

    # Ignore the literal escaped form: "Logic bomb triggered\n"
    cleaned = text.replace("Logic bomb triggered\\n", "")

    has_logic = "Logic bomb triggered" in cleaned
    has_normal = "Path without the bomb" in cleaned

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


def find_all_queue_dirs(root):
    """Recursively find ALL directories named 'queue'."""
    queue_dirs = []
    for dirpath, dirnames, _ in os.walk(root):
        if os.path.basename(dirpath) == "queue":
            queue_dirs.append(dirpath)
    return queue_dirs


def process_queue_dir(queue_dir):
    """Compute bomb coverage classification for a single queue directory."""
    yaml_files = [
        os.path.join(queue_dir, f)
        for f in os.listdir(queue_dir)
        if f.endswith(".yaml")
    ]

    if not yaml_files:
        print(f"⚠️ No YAML files found in: {queue_dir}")
        return "both_missing"

    any_logic = False
    any_normal = False

    for ypath in yaml_files:
        data = safe_load_yaml(ypath)
        if not data:
            continue

        exec_trace = data.get("execution_trace", "")
        src_exec_trace = data.get("src_execution_trace", "")

        l1, n1 = classify_trace(exec_trace)
        l2, n2 = classify_trace(src_exec_trace)

        if l1 or l2:
            any_logic = True
        if n1 or n2:
            any_normal = True

    return combine_classification(any_logic, any_normal)


def main():
    if len(sys.argv) != 3:
        print("Usage: summarise_coverage.py <OUT_DIR> <SUMMARY_JSON>")
        sys.exit(1)

    OUT_DIR = os.path.abspath(sys.argv[1])
    SUMMARY_JSON = os.path.abspath(sys.argv[2])

    queue_dirs = find_all_queue_dirs(OUT_DIR)

    if not queue_dirs:
        print(f"❌ No queue directories found anywhere inside: {OUT_DIR}")
        sys.exit(1)

    print(f"✓ Found {len(queue_dirs)} queue dirs")

    results = []

    for qdir in queue_dirs:
        print(f"→ Processing queue: {qdir}")
        label = process_queue_dir(qdir)

        results.append({
            "queue_dir": qdir,
            "result": label
        })

    # Ensure JSON file exists
    if not os.path.exists(SUMMARY_JSON):
        with open(SUMMARY_JSON, "w") as f:
            json.dump([], f, indent=2)

    # Load existing entries
    with open(SUMMARY_JSON, "r") as f:
        arr = json.load(f)

    # Add new entries
    arr.extend(results)

    # Save
    with open(SUMMARY_JSON, "w") as f:
        json.dump(arr, f, indent=2)

    print(f"✔ Added {len(results)} entries to {SUMMARY_JSON}")


if __name__ == "__main__":
    main()
