import os
import json
from collections import defaultdict

COV_DIR = "/home/jim/logic_bombs/libexpat/build/klee-out-4"
COVERABLE_JSON = "/home/jim/logic_bombs/libexpat/build/xmlwf_coverable_lines.json"

print("[debug] COV_DIR =", COV_DIR)

# --------------------------------------------------
# Load coverable lines
# --------------------------------------------------
with open(COVERABLE_JSON) as f:
    coverable = json.load(f)

coverable_lines = {}
for path, data in coverable.items():
    base = os.path.basename(path)
    coverable_lines[base] = set(data["coverable_lines"])

print("[debug] Coverable files:", sorted(coverable_lines.keys()))
print()

# --------------------------------------------------
# Parse .cov files
# --------------------------------------------------
covered = defaultdict(set)
total_lines = 0
parsed_lines = 0
skipped = 0

print("[debug] Scanning .cov files...")

for root, _, files in os.walk(COV_DIR):
    for f in files:
        if not f.endswith(".cov"):
            continue

        cov_path = os.path.join(root, f)
        print(f"\n[debug] Reading {cov_path}")

        with open(cov_path, "r", errors="replace") as fh:
            for line in fh:
                total_lines += 1
                line = line.strip()

                if not line:
                    continue

                if ":" not in line:
                    skipped += 1
                    print("  [skip:no colon]", line)
                    continue

                try:
                    src, lineno = line.rsplit(":", 1)
                    lineno = int(lineno)
                except Exception:
                    skipped += 1
                    print("  [skip:parse fail]", line)
                    continue

                base = os.path.basename(src)
                parsed_lines += 1

                print(f"  [OK] {base}:{lineno}")

                covered[base].add(lineno)

print("\n[debug] TOTAL LINES READ:", total_lines)
print("[debug] PARSED LINES:", parsed_lines)
print("[debug] SKIPPED LINES:", skipped)

print("\n[debug] Files with coverage:")
for k in sorted(covered):
    print(" ", k, "=>", len(covered[k]), "lines")

# --------------------------------------------------
# Coverage computation
# --------------------------------------------------
print("\n=== COVERAGE SUMMARY ===\n")

for fname in sorted(coverable_lines):
    cov = coverable_lines[fname]
    hit = covered.get(fname, set())

    print(fname)
    print(f"  Covered: {len(hit)} / {len(cov)}")

    if hit:
        print(f"  Sample hits: {sorted(list(hit))[:10]}")
    else:
        print("  Sample hits: NONE")

    print()

print("=== DONE ===")
