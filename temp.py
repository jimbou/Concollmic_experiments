#!/usr/bin/env python3
import sys
import json
from pathlib import Path

def extract_project_name(queue_dir):
    """
    Extract the project folder from a queue_dir path.
    Example: /.../stack_cp_l1_klee_instr/out/queue -> stack_cp_l1_klee_instr
    """
    p = Path(queue_dir)
    # The project dir is the parent of "out"
    return p.parent.parent.name


def main():
    if len(sys.argv) != 4:
        print("Usage: filter_bash_by_results.py <bash_script> <json_results> <output_bash>")
        sys.exit(1)

    bash_path = Path(sys.argv[1])
    json_path = Path(sys.argv[2])
    out_path = Path(sys.argv[3])

    if not bash_path.is_file():
        print(f"❌ Bash script not found: {bash_path}")
        sys.exit(1)
    if not json_path.is_file():
        print(f"❌ JSON file not found: {json_path}")
        sys.exit(1)

    # Load bash lines
    with open(bash_path, "r") as f:
        bash_lines = f.readlines()

    # Load JSON entries
    with open(json_path, "r") as f:
        entries = json.load(f)

    # Identify project names with result != "both"
    bad_projects = set()
    for entry in entries:
        if entry.get("result") != "both":
            proj = extract_project_name(entry["queue_dir"])
            bad_projects.add(proj)

    print("❗ Projects with result != 'both':")
    for p in bad_projects:
        print("   ", p)

    # Filter bash lines
    selected_lines = []
    for line in bash_lines:
        for proj in bad_projects:
            if proj in line:
                selected_lines.append(line)
                break

    # Write output bash
    with open(out_path, "w") as f:
        f.write("#!/usr/bin/env bash\n")
        f.write("# Auto-generated filtered commands\n\n")
        for l in selected_lines:
            f.write(l)

    print(f"\n✅ Done! Written {len(selected_lines)} commands to: {out_path}")

if __name__ == "__main__":
    main()
