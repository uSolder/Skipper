#!/usr/bin/env python3
"""Build the Boards Manager archive and package index.

    python tools/package.py --base-url URL [--index OLD_INDEX] [--out dist]

Writes dist/Skipper-STM32duino-<version>.tar.bz2 and
dist/package_usolder_index.json. If --index is given, the new version is
added to that index instead of starting a new one.
"""

import argparse
import hashlib
import json
import re
import tarfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONTENTS = ["boards.txt", "platform.txt", "variants", "libraries", "debugger", "README.md", "LICENSE"]
INDEX_NAME = "package_usolder_index.json"
# Tools this package installs, as published by their own releases
# (e.g. openocd-st.json from the OpenOCD release).
TOOL_ENTRIES = sorted((ROOT / "tools").glob("*.json"))
REPO_URL = "https://github.com/uSolder/Skipper/tree/main/Software/STM32duino"


def platform_version():
    text = (ROOT / "platform.txt").read_text()
    return re.search(r"^version=(.+)$", text, re.M).group(1).strip()


def board_names():
    text = (ROOT / "boards.txt").read_text()
    return [{"name": n.strip()} for n in re.findall(r"^\w+\.name=(.+)$", text, re.M)]


def make_archive(version, out):
    folder = f"Skipper-STM32duino-{version}"
    path = out / f"{folder}.tar.bz2"

    def clean(info):
        # Same archive bytes for the same files, whoever builds it.
        info.uid = info.gid = 0
        info.uname = info.gname = ""
        info.mtime = 0
        return info

    with tarfile.open(path, "w:bz2") as tar:
        for name in CONTENTS:
            top = ROOT / name
            for f in [top] + sorted(top.rglob("*")):
                arcname = f"{folder}/{f.relative_to(ROOT).as_posix()}"
                tar.add(f, arcname=arcname, recursive=False, filter=clean)
    return path


def new_index():
    return {
        "packages": [{
            "name": "usolder",
            "maintainer": "uSolder",
            "websiteURL": "https://usolder.com",
            "help": {"online": REPO_URL},
            "platforms": [],
            "tools": [],
        }]
    }


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--base-url", required=True, help="URL the archive will be downloaded from, without the file name")
    ap.add_argument("--index", type=Path, help="existing index to add this version to")
    ap.add_argument("--out", type=Path, default=ROOT / "dist")
    args = ap.parse_args()

    version = platform_version()
    args.out.mkdir(parents=True, exist_ok=True)
    archive = make_archive(version, args.out)
    data = archive.read_bytes()

    entry = {
        "name": "uSolder STM32 Boards",
        "architecture": "stm32",
        "version": version,
        "category": "Contributed",
        "url": f"{args.base_url.rstrip('/')}/{archive.name}",
        "archiveFileName": archive.name,
        "checksum": "SHA-256:" + hashlib.sha256(data).hexdigest(),
        "size": str(len(data)),
        "help": {"online": REPO_URL},
        "boards": board_names(),
        "toolsDependencies": [],
    }

    index = json.loads(args.index.read_text()) if args.index and args.index.exists() else new_index()
    package = index["packages"][0]
    package.setdefault("tools", [])

    for path in TOOL_ENTRIES:
        tool = json.loads(path.read_text())
        entry["toolsDependencies"].append(
            {"packager": package["name"], "name": tool["name"], "version": tool["version"]})
        package["tools"] = [t for t in package["tools"]
                            if (t["name"], t["version"]) != (tool["name"], tool["version"])] + [tool]

    platforms = package["platforms"]
    platforms[:] = [p for p in platforms if p["version"] != version] + [entry]

    (args.out / INDEX_NAME).write_text(json.dumps(index, indent=2) + "\n")
    print(f"{archive.name}  {entry['size']} bytes  {entry['checksum']}")


if __name__ == "__main__":
    main()
