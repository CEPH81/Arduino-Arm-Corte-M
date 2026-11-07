#!/usr/bin/env python3
"""
gen_vectors.py - Genera la tabla de vectores en C para el núcleo
Arduino-Arm-Cortex-M a partir del startup oficial del fabricante (.s/.S)
o de la lista de IRQ de su header.

  gen_vectors.py <startup.s> <MACRO_DEL_CHIP> <salida.c> [comentario]
  gen_vectors.py --irq-count <N> --name-fmt "IRQ{:03d}_Handler" <MACRO> <salida.c>

Así todas las familias usan el mismo arranque (cores/arduino/startup/
cm_startup.c) y los mismos nombres de manejadores que la documentación del
fabricante, sin depender del ensamblador de cada uno.
"""
import re
import sys

CORE = [
    None,                   # 0  pila inicial (_estack)
    "Reset_Handler",        # 1
    "NMI_Handler",          # 2
    "HardFault_Handler",    # 3
    "MemManage_Handler",    # 4
    "BusFault_Handler",     # 5
    "UsageFault_Handler",   # 6
    "SecureFault_Handler",  # 7  (solo Armv8-M Mainline)
    None, None, None,       # 8-10
    "SVC_Handler",          # 11
    "DebugMon_Handler",     # 12
    None,                   # 13
    "PendSV_Handler",       # 14
    "SysTick_Handler",      # 15
]

TABLE_LABELS = ("g_pfnVectors", "__Vectors", "__isr_vector", "__vector_table", "vector_table")


def parse_startup(path):
    words = []
    in_table = False
    with open(path, encoding="latin-1") as f:
        for raw in f:
            line = raw.split("@")[0].split("//")[0].split(";")[0]
            line = re.sub(r"/\*.*?\*/", "", line).strip()
            if not in_table:
                if any(re.match(r"^%s\s*:" % lbl, line) for lbl in TABLE_LABELS):
                    in_table = True
                    rest = line.split(":", 1)[1].strip()
                    if rest:
                        line = rest
                    else:
                        continue
                else:
                    continue
            if not line:
                continue
            m = re.match(r"^\.(word|long|4byte)\s+(.+)$", line)
            if not m:
                if words:
                    break
                continue
            for item in m.group(2).split(","):
                item = item.strip()
                words.append(item)
    if len(words) < 16:
        sys.exit("no se encontró la tabla de vectores en %s" % path)
    return words


def normalize(words):
    out = []
    for i, w in enumerate(words):
        if i == 0:
            out.append(None)
            continue
        if i < 16:
            is_zero = w in ("0", "0x0", "0x00000000", "0x00")
            out.append(None if is_zero else CORE[i])
            continue
        if w in ("0", "0x0", "0x00000000", "0x00") or not re.match(r"^[A-Za-z_]\w*$", w):
            out.append(None)
        else:
            out.append(w)
    return out


def emit(names, macro, out_path, comment):
    handlers = []
    for n in names[1:]:
        if n and n != "Reset_Handler" and n not in handlers:
            handlers.append(n)
    with open(out_path, "w", newline="\n") as f:
        f.write("/*\n * Tabla de vectores generada por tools/gen_vectors.py\n")
        f.write(" * Origen: %s\n" % comment)
        f.write(" * No editar a mano.\n */\n")
        f.write("#if defined(%s)\n\n" % macro)
        f.write('#include "../cm_startup.h"\n\n')
        for h in handlers:
            f.write("void %s(void) CM_WEAK_ALIAS;\n" % h)
        f.write("\nCM_VECTOR_TABLE(%d) = {\n" % len(names))
        f.write("  (cm_vector_t)&_estack,\n")
        for i, n in enumerate(names[1:], start=1):
            f.write("  %-32s /* %3d%s */\n" % ((n if n else "0") + ",", i, "" if i < 16 else "  IRQ %d" % (i - 16)))
        f.write("};\n\n#endif\n")


def main():
    args = sys.argv[1:]
    if args and args[0] == "--irq-count":
        count = int(args[1])
        fmt = args[3]
        macro, out_path = args[4], args[5]
        names = [None] + CORE[1:7] + [None] * 4 + ["SVC_Handler", "DebugMon_Handler", None,
                                                   "PendSV_Handler", "SysTick_Handler"]
        names[7] = None
        names += [fmt.format(i) for i in range(count)]
        emit(names, macro, out_path, "%d IRQ con nombre %s" % (count, fmt))
        return
    if len(args) < 3:
        sys.exit(__doc__)
    startup, macro, out_path = args[0], args[1], args[2]
    comment = args[3] if len(args) > 3 else startup.split("/")[-1]
    emit(normalize(parse_startup(startup)), macro, out_path, comment)


if __name__ == "__main__":
    main()
