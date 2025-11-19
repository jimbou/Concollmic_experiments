#!/usr/bin/env python3
import json
import os
import sys

if len(sys.argv) != 3:
    print("Usage: python3 check_results.py <ace_results.json> <klee_results.json>")
    sys.exit(1)

ace_json = sys.argv[1]
klee_json = sys.argv[2]

# ---------------------------------------------------------
# Load both JSON files
# ---------------------------------------------------------
with open(ace_json, "r") as f:
    ace_data = json.load(f)

with open(klee_json, "r") as f:
    klee_data = json.load(f)

# ---------------------------------------------------------
# Helper: extract name from ACE path
# "/.../stack_cp_l1_klee_instr/out/queue" -> "stack_cp_l1"
# ---------------------------------------------------------
def extract_ace_name(queue_dir):
    base = os.path.basename(os.path.dirname(os.path.dirname(queue_dir)))  
    # base = "stack_cp_l1_klee_instr"
    if base.endswith("_klee_instr"):
        base = base[:-len("_klee_instr")]
    return base

# ---------------------------------------------------------
# Helper: extract name from KLEE path
# "src/.../rand_ef_l2_klee_annotated.c" -> "rand_ef_l2"
# ---------------------------------------------------------
def extract_klee_name(path):
    file = os.path.basename(path)
    if file.endswith("_klee_annotated.c"):
        return file[:-len("_klee_annotated.c")]
    return file

# ---------------------------------------------------------
# Build mapping from KLEE names to whether any solver == triggered_both
# ---------------------------------------------------------
klee_triggered_both = {}

for path, result_dict in klee_data.items():
    name = extract_klee_name(path)
    has_triggered_both = any(status == "triggered_both" for status in result_dict.values())
    klee_triggered_both[name] = has_triggered_both

# ---------------------------------------------------------
# Scan ACE entries: only entries where result != "both"
# Check if KLEE has triggered_both for that name
# ---------------------------------------------------------
print("Entries where ACE did NOT get 'both', but KLEE DID trigger_both:")
print("--------------------------------------------------------------")

for entry in ace_data:
    ace_name = extract_ace_name(entry["queue_dir"])
    ace_result = entry["result"]

    if ace_result == "both":
        continue  # ignore successful ones

    # Check if this name exists in KLEE results
    if ace_name in klee_triggered_both:
        if klee_triggered_both[ace_name]:
            print(f"{ace_name}: ACE={ace_result}, KLEE=triggered_both")

