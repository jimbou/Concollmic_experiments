import os
import re

HEADER = "fdlibm.h"

# ---------- STEP 1: PARSE HEADER PROTOTYPES -----------------

proto_re = re.compile(
    r"extern\s+([a-zA-Z_][a-zA-Z0-9_\s\*]+?)\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*__P\(\((.*?)\)\);"
)

protos = {}

with open(HEADER) as f:
    for line in f:
        m = proto_re.search(line)
        if not m:
            continue

        ret = m.group(1).strip()
        name = m.group(2).strip()
        args_raw = m.group(3).strip()

        if args_raw == "void" or args_raw == "":
            args = []
        else:
            args = [a.strip() for a in args_raw.split(",")]

        protos[name] = (ret, args)

print(f"Parsed {len(protos)} prototypes from fdlibm.h")

# ---------- STEP 2: BLACKLIST -------------------------------

BLACKLIST = {
    "s_signgam.c",
    "s_matherr.c",
    "s_lib_version.c",
    "k_rem_pio2.c",
}

# ---------- STEP 3: FUNCTION DEFINITION MATCHER -------------

FUNC_DEF_RE = re.compile(
    r"^\s*(?:double|int)\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(",
    re.MULTILINE
)

# ---------- STEP 4: PROCESS FILES ---------------------------

for fname in os.listdir("."):
    if not fname.endswith(".c"):
        continue

    if fname in BLACKLIST:
        print(f"SKIP (blacklisted): {fname}")
        continue

    with open(fname) as f:
        src = f.read()

    if re.search(r"\bmain\s*\(", src):
        print(f"SKIP (already has main): {fname}")
        continue

    m = FUNC_DEF_RE.search(src)
    if not m:
        print(f"SKIP (no function def found): {fname}")
        continue

    func = m.group(1)

    if func not in protos:
        print(f"SKIP (not in header): {fname} -> {func}")
        continue

    ret, args = protos[func]

    cli_decls = []
    call_args = []
    driver_args = []
    unsupported = False

    for i, a in enumerate(args):
        a = a.replace("const", "").strip()
        v = f"a{i}"

        if a == "double":
            cli_decls.append(f"    double {v} = atof(argv[{i+1}]);")
            call_args.append(v)
            driver_args.append("1.0")

        elif a == "int":
            cli_decls.append(f"    int {v} = atoi(argv[{i+1}]);")
            call_args.append(v)
            driver_args.append("1")

        elif a == "double*":
            cli_decls.append(f"    double {v} = 0.0;")
            call_args.append(f"&{v}")
            driver_args.append("1.0")

        elif a == "int*":
            cli_decls.append(f"    int {v} = 0;")
            call_args.append(f"&{v}")
            driver_args.append("1")

        else:
            print(f"SKIP (unsupported full signature): {fname} -> {a}")
            unsupported = True
            break

    if unsupported:
        continue

    argc_check = f"""
    if (argc != {len(args)+1}) {{
        printf("Usage: %s {' '.join(['<arg>']*len(args))}\\n", argv[0]);
        return 1;
    }}
"""

    if ret == "void":
        call = f"    {func}({', '.join(call_args)});"
    else:
        call = f"    {ret} r = {func}({', '.join(call_args)});\n    printf(\"%f\\n\", r);"

    main = f"""

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {{
{argc_check}
{chr(10).join(cli_decls)}

{call}

    return 0;
}}
"""

    with open(fname, "w") as f:
        f.write(src.rstrip() + main)

    print(f"PATCHED: {fname}  ->  CLI main calls {func}({', '.join(call_args)})")

    # ---------- STEP 5: GENERATE DRIVER SCRIPT ---------------------------

    base = fname.replace(".c", "")
    driver_name = f"{base}_driver.sh"

    driver_cmd = f"./temp_executable {' '.join(driver_args)}"

    driver_script = f"""#!/usr/bin/env bash
# Auto-generated driver for {fname}
# Expected executable name: ./temp_executable

{driver_cmd}
"""

    with open(driver_name, "w") as f:
        f.write(driver_script)

    os.chmod(driver_name, 0o755)

    print(f"DRIVER:  {driver_name}  ->  {driver_cmd}")
