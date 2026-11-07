#!/usr/bin/env python3
"""
make_release.py - Empaqueta el núcleo y actualiza el índice del Gestor de placas.

  python3 tools/make_release.py            -> versión de cortexm/platform.txt
  python3 tools/make_release.py 0.3.0      -> fija esa versión en platform.txt

Genera dist/cortexm-<versión>.zip y añade (o reemplaza) esa versión en
package_ceph81_cortexm_index.json. El flujo de GitHub Actions
(.github/workflows/release.yml) lo ejecuta al publicar una etiqueta vX.Y.Z
y sube el zip a la Release.
"""
import hashlib, json, os, re, sys, zipfile

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
CORE = os.path.join(ROOT, "cortexm")
INDEX = os.path.join(ROOT, "package_ceph81_cortexm_index.json")
REPO = "https://github.com/CEPH81/Arduino-Arm-Corte-M"
GCC = ("xpack-arm-none-eabi-gcc", "14.2.1-1.1")
OCD = ("xpack-openocd", "0.12.0-6")
SKIP = {"platform.local.txt", "boards.local.txt"}


def platform_version(new=None):
    p = os.path.join(CORE, "platform.txt")
    s = open(p, encoding="utf-8").read()
    if new:
        s = re.sub(r"^version=.*$", "version=" + new, s, flags=re.M)
        open(p, "w", encoding="utf-8", newline="\n").write(s)
    return re.search(r"^version=(.*)$", s, re.M).group(1).strip()


def board_names():
    names = []
    for line in open(os.path.join(CORE, "boards.txt"), encoding="utf-8"):
        m = re.match(r"^[\w]+\.name=(.*)$", line.strip())
        if m:
            names.append({"name": m.group(1)})
    return names


def build_zip(version):
    os.makedirs(os.path.join(ROOT, "dist"), exist_ok=True)
    name = "cortexm-%s.zip" % version
    path = os.path.join(ROOT, "dist", name)
    with zipfile.ZipFile(path, "w", zipfile.ZIP_DEFLATED) as z:
        for base, dirs, files in os.walk(CORE):
            dirs.sort()
            for f in sorted(files):
                if f in SKIP or f.endswith(".pyc"):
                    continue
                full = os.path.join(base, f)
                arc = os.path.join("cortexm-" + version, os.path.relpath(full, CORE))
                info = zipfile.ZipInfo(arc.replace(os.sep, "/"), date_time=(2026, 1, 1, 0, 0, 0))
                info.compress_type = zipfile.ZIP_DEFLATED
                ejecutable = f.endswith(".sh") or f == "air105load"
                info.external_attr = (0o755 if ejecutable else 0o644) << 16
                z.writestr(info, open(full, "rb").read())
    data = open(path, "rb").read()
    return name, path, "SHA-256:" + hashlib.sha256(data).hexdigest(), str(len(data))


def main():
    if len(sys.argv) > 1 and not __import__("re").fullmatch(r"\d+\.\d+\.\d+", sys.argv[1]):
        sys.exit("uso: make_release.py [X.Y.Z]")
    version = platform_version(sys.argv[1] if len(sys.argv) > 1 else None)
    name, path, checksum, size = build_zip(version)
    tools = json.load(open(os.path.join(ROOT, "tools", "xpack_tools.json")))

    if os.path.exists(INDEX):
        idx = json.load(open(INDEX, encoding="utf-8"))
    else:
        idx = {"packages": [{
            "name": "CEPH81", "maintainer": "CEPH81 (MikrocontrollerCephs)",
            "websiteURL": REPO, "email": "", "help": {"online": REPO + "/issues"},
            "platforms": [], "tools": [
                {"name": GCC[0], "version": GCC[1], "systems": tools["arm-none-eabi-gcc"]},
                {"name": OCD[0], "version": OCD[1], "systems": tools["openocd"]},
            ]}]}
    pkg = idx["packages"][0]
    pkg["platforms"] = [p for p in pkg["platforms"] if p["version"] != version]
    pkg["platforms"].append({
        "name": "Arduino Arm Cortex-M (CEPH81)",
        "architecture": "cortexm",
        "version": version,
        "category": "Contributed",
        "help": {"online": REPO},
        "url": "%s/releases/download/v%s/%s" % (REPO, version, name),
        "archiveFileName": name,
        "checksum": checksum,
        "size": size,
        "boards": board_names(),
        "toolsDependencies": [
            {"packager": "CEPH81", "name": GCC[0], "version": GCC[1]},
            {"packager": "CEPH81", "name": OCD[0], "version": OCD[1]},
        ],
    })
    pkg["platforms"].sort(key=lambda p: [int(x) for x in re.findall(r"\d+", p["version"])])
    with open(INDEX, "w", encoding="utf-8", newline="\n") as f:
        json.dump(idx, f, indent=2, ensure_ascii=False)
        f.write("\n")
    print("%s  %s  %s bytes" % (path, checksum, size))


if __name__ == "__main__":
    main()
