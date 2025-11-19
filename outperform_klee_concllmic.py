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
# Build mapping: KLEE name -> did ANY solver trigger_both?
# ---------------------------------------------------------
klee_triggered_both = {}

for path, solvers in klee_data.items():
    name = extract_klee_name(path)
    did_trigger = any(status == "triggered_both" for status in solvers.values())
    klee_triggered_both[name] = did_trigger

# ---------------------------------------------------------
# 1) Cases where ACE < KLEE
# ACE != both  AND  KLEE triggered_both
# ---------------------------------------------------------
print("\n==============================================================")
print("Cases where ConcoLLMic is WORSE than KLEE")
print("(ACE != 'both' but some KLEE solver triggered_both)")
print("==============================================================\n")

for entry in ace_data:
    ace_name = extract_ace_name(entry["queue_dir"])
    ace_result = entry["result"]

    if ace_result == "both":
        continue

    if ace_name in klee_triggered_both and klee_triggered_both[ace_name]:
        print(f"{ace_name}: ACE={ace_result}, KLEE=triggered_both")


# ---------------------------------------------------------
# 2) Cases where ACE > KLEE
# ACE == both  AND  NO KLEE solver triggered_both
# ---------------------------------------------------------
print("\n==============================================================")
print("Cases where ConcoLLMic OUTPERFORMS KLEE")
print("(ACE == 'both' but NO KLEE solver triggered_both)")
print("==============================================================\n")

for entry in ace_data:
    ace_name = extract_ace_name(entry["queue_dir"])
    ace_result = entry["result"]

    if ace_result != "both":
        continue

    # If KLEE has no triggered_both
    if ace_name in klee_triggered_both and not klee_triggered_both[ace_name]:
        print(f"{ace_name}: ACE=both, KLEE=no_triggered_both")

print()
