#!/usr/bin/env python3
"""Print the Boards Manager "tools" entry for a set of OpenOCD archives.

    python tool_entry.py --version V --base-url URL DIST_DIR > openocd-st.json
"""

import argparse
import hashlib
import json
from pathlib import Path

# build.sh target -> Arduino host name
HOSTS = {
    "win32-x64": "x86_64-mingw32",
    "linux-x64": "x86_64-linux-gnu",
    "linux-arm64": "aarch64-linux-gnu",
    "darwin-x64": "x86_64-apple-darwin",
    "darwin-arm64": "arm64-apple-darwin",
}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--version", required=True)
    ap.add_argument("--base-url", required=True)
    ap.add_argument("dist", type=Path)
    args = ap.parse_args()

    systems = []
    for target, host in HOSTS.items():
        matches = sorted(args.dist.glob(f"openocd-st-{args.version}-{target}.*"))
        if len(matches) != 1:
            raise SystemExit(f"expected one archive for {target}, found {matches}")
        archive = matches[0]
        data = archive.read_bytes()
        systems.append({
            "host": host,
            "url": f"{args.base_url.rstrip('/')}/{archive.name}",
            "archiveFileName": archive.name,
            "checksum": "SHA-256:" + hashlib.sha256(data).hexdigest(),
            "size": str(len(data)),
        })

    print(json.dumps({"name": "openocd-st", "version": args.version, "systems": systems}, indent=2))


if __name__ == "__main__":
    main()
