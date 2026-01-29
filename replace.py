#!/usr/bin/env python3
import os

TARGET_MACRO = "#ifdef STDLIBX_ENABLE_COMPAT_NAMES"

def process_file(path):
    with open(path, "r") as f:
        lines = f.readlines()

    out = []
    skip = False
    depth = 0
    changed = False

    for line in lines:
        stripped = line.strip()

        if stripped.startswith(TARGET_MACRO):
            skip = True
            depth = 1
            changed = True
            continue

        if skip:
            if stripped.startswith("#ifdef"):
                depth += 1
            elif stripped.startswith("#endif"):
                depth -= 1
                if depth == 0:
                    skip = False
            continue

        out.append(line)

    if changed:
        with open(path, "w") as f:
            f.writelines(out)
        print(f"Modified: {path}")

def main():
    for root, _, files in os.walk("."):
        for name in files:
            if name.endswith(".cppm"):
                process_file(os.path.join(root, name))

if __name__ == "__main__":
    main()
