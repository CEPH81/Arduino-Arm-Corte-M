#!/usr/bin/env python3
"""
gen_boards.py - Genera cortexm/boards.txt a partir de la lista BOARDS.

Cada placa declara solo lo que la distingue; los menús comunes (reloj,
optimización, LTO, método de carga) se generan igual para todas.
Uso:  python3 tools/gen_boards.py   (escribe cortexm/boards.txt)
"""
import os

M4F = "-mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16"
M33F = "-mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16"
M0 = "-mcpu=cortex-m0 -mfloat-abi=soft"

# Métodos de carga disponibles (clave -> (texto del menú, herramienta, propiedades extra))
UPLOAD = {
    "stlink":   ("ST-LINK (OpenOCD)", "openocd", {"upload.openocd_interface": "interface/stlink.cfg"}),
    "daplink":  ("CMSIS-DAP / DAPLink / WeAct Mini Debugger (OpenOCD)", "openocd",
                 {"upload.openocd_interface": "interface/cmsis-dap.cfg"}),
    "jlink":    ("SEGGER J-Link", "jlink", {}),
    "pyocd":    ("pyOCD (CMSIS-DAP, ST-LINK, J-Link; usa el pack del fabricante)", "pyocd", {}),
    "cubeprog": ("ST-LINK (STM32CubeProgrammer)", "stm32cubeprog", {}),
}

BOARDS = [
    # ------------------------------------------------------------- ST
    dict(id="f407ve_black", name="STM32F407VET6 Black (F4VE)", variant="BLACK_F407VE",
         mcu=M4F, flags="-DSTM32F4xx -DSTM32F407xx", vendor="ST/F4",
         hse=("HSE_VALUE", 8000000), flash=512 * 1024, ram=128 * 1024,
         jlink="STM32F407VE", openocd="stm32f4x", pyocd="stm32f407ve",
         clocks=[("fast", "168 MHz (HSE 8 MHz + PLL)", ""),
                 ("safe", "16 MHz (HSI) - modo seguro", "-DCM_CLOCK_SAFE")],
         upload=["stlink", "daplink", "jlink", "pyocd", "cubeprog"]),
    dict(id="nucleo_l432kc", name="Nucleo-32 STM32L432KC", variant="NUCLEO_L432KC",
         mcu=M4F, flags="-DSTM32L4xx -DSTM32L432xx", vendor="ST/L4",
         hse=("HSE_VALUE", 8000000), flash=256 * 1024, ram=64 * 1024,
         jlink="STM32L432KC", openocd="stm32l4x", pyocd="stm32l432kc",
         clocks=[("fast", "80 MHz (MSI + PLL)", ""),
                 ("safe", "4 MHz (MSI) - modo seguro", "-DCM_CLOCK_SAFE")],
         upload=["stlink", "daplink", "jlink", "pyocd", "cubeprog"]),
    dict(id="weact_g431", name="WeAct STM32G431CBU6", variant="WEACT_G431",
         mcu=M4F, flags="-DSTM32G4xx -DSTM32G431xx", vendor="ST/G4",
         hse=("HSE_VALUE", 8000000), flash=128 * 1024, ram=22 * 1024,
         jlink="STM32G431CB", openocd="stm32g4x", pyocd="stm32g431cbux",
         clocks=[("fast", "170 MHz (HSE 8 MHz + PLL, boost)", ""),
                 ("safe", "16 MHz (HSI) - modo seguro", "-DCM_CLOCK_SAFE")],
         upload=["stlink", "daplink", "jlink", "pyocd", "cubeprog"]),
    dict(id="weact_wb55", name="WeAct STM32WB55CGU6", variant="WEACT_WB55",
         mcu=M4F, flags="-DSTM32WBxx -DSTM32WB55xx", vendor="ST/WB",
         hse=("HSE_VALUE", 32000000), flash=512 * 1024, ram=192 * 1024,
         jlink="STM32WB55CG", openocd="stm32wbx", pyocd="stm32wb55xg",
         clocks=[("fast", "64 MHz (HSE 32 MHz + PLL)", ""),
                 ("safe", "4 MHz (MSI) - modo seguro", "-DCM_CLOCK_SAFE")],
         upload=["stlink", "daplink", "jlink", "pyocd", "cubeprog"]),
    dict(id="generic_h503", name="STM32H503 (genérica)", variant="GENERIC_H503",
         mcu=M33F, flags="-DSTM32H5xx -DSTM32H503xx", vendor="ST/H5",
         hse=("HSE_VALUE", 25000000), flash=128 * 1024, ram=32 * 1024,
         jlink="STM32H503RB", openocd=None, pyocd="stm32h503rbtx",
         clocks=[("fast", "250 MHz (HSI + PLL1, VOS0)", ""),
                 ("safe", "32 MHz (HSI/2) - modo seguro", "-DCM_CLOCK_SAFE")],
         upload=["cubeprog", "jlink", "pyocd"]),
    # ------------------------------------------------------ GigaDevice
    dict(id="gd32f427v_start", name="GD32F427V-START", variant="GD32F427V_START",
         mcu=M4F, flags="-DGD32F4XX -DGD32F427", vendor="GigaDevice/F4",
         hse=("HXTAL_VALUE", 25000000), flash=3072 * 1024, ram=192 * 1024,
         jlink="GD32F427VK", openocd="stm32f4x", pyocd="gd32f427vk",
         clocks=[("c200", "200 MHz (cristal 25 MHz + PLL)", "-D__SYSTEM_CLOCK_200M_PLL_25M_HXTAL=200000000"),
                 ("c168", "168 MHz (cristal 25 MHz + PLL)", "-D__SYSTEM_CLOCK_168M_PLL_25M_HXTAL=168000000"),
                 ("i200", "200 MHz (IRC16M interno + PLL, sin cristal)", "-D__SYSTEM_CLOCK_200M_PLL_IRC16M=200000000"),
                 ("safe", "16 MHz (IRC16M) - modo seguro", "-D__SYSTEM_CLOCK_IRC16M=16000000")],
         upload=["daplink", "jlink", "pyocd", "stlink"]),
    # ---------------------------------------------------------- Artery
    dict(id="weact_at32f403a", name="WeAct AT32F403ACGU7", variant="WEACT_AT32F403A",
         mcu=M4F, flags="-DAT32F403A_407 -DAT32F403ACGU7 -DUSE_STDPERIPH_DRIVER", vendor="Artery/F403A",
         hse=("HEXT_VALUE", 8000000), flash=1024 * 1024, ram=96 * 1024,
         jlink="AT32F403ACGU7", openocd=None, pyocd="_at32f403acgu7",
         clocks=[("fast", "240 MHz (HEXT 8 MHz + PLL)", ""),
                 ("safe", "8 MHz (HICK) - modo seguro", "-DCM_CLOCK_SAFE")],
         upload=["jlink", "pyocd"]),
    dict(id="atstart_f437", name="AT-START-F437 (AT32F437ZMT7)", variant="ATSTART_F437",
         mcu=M4F, flags="-DAT32F435_437 -DAT32F437ZMT7 -DUSE_STDPERIPH_DRIVER", vendor="Artery/F43x",
         hse=("HEXT_VALUE", 8000000), flash=4032 * 1024, ram=384 * 1024,
         jlink="AT32F437ZMT7", openocd=None, pyocd="_at32f437zmt7",
         clocks=[("fast", "288 MHz (HEXT 8 MHz + PLL)", ""),
                 ("safe", "8 MHz (HICK) - modo seguro", "-DCM_CLOCK_SAFE")],
         upload=["jlink", "pyocd"]),
    # ------------------------------------------------------------ HDSC
    dict(id="hc32f460jeta", name="HC32F460JETA (placa de desarrollo)", variant="HC32F460JETA",
         mcu=M4F, flags="-DHC32F460", vendor="HDSC/HC32F460",
         hse=("XTAL_VALUE", 8000000), flash=512 * 1024, ram=188 * 1024,
         jlink="HC32F460xE", openocd=None, pyocd="hc32f460xe",
         clocks=[("fast", "200 MHz (XTAL 8 MHz + MPLL)", ""),
                 ("safe", "8 MHz (MRC) - modo seguro", "-DCM_CLOCK_SAFE")],
         upload=["jlink", "pyocd"]),
    # ---------------------------------------------------------- Synwit
    dict(id="swm181cb", name="Synwit SWM181CBT6 (placa azul USB-C)", variant="SWM181CB_BLUE",
         mcu=M0, flags="-DCHIP_SWM181", vendor="Synwit/SWM181",
         hse=None, flash=64 * 1024, ram=16 * 1024,
         jlink="SWM181xB", openocd=None, pyocd="swm181xb",
         clocks=[("c48", "48 MHz (RC interno)", "-DSYS_CLK=2"),
                 ("c24", "24 MHz (RC interno)", "-DSYS_CLK=0"),
                 ("c12", "12 MHz (RC interno)", "-DSYS_CLK=3")],
         upload=["jlink", "pyocd"]),
    dict(id="swm34svet6", name="Synwit SWM34SVET6 (SWM341)", variant="SWM34SVET6_DEV",
         mcu=M33F, flags="-DCHIP_SWM341", vendor="Synwit/SWM341",
         hse=("__HSE", 12000000), flash=512 * 1024, ram=64 * 1024,
         jlink="SWM341xE", openocd=None, pyocd="swm341xe",
         clocks=[("pll", "120 MHz (cristal 12 MHz + PLL)", "-DSYS_CLK=6"),
                 ("rc40", "40 MHz (RC interno, sin cristal)", "-DSYS_CLK=2"),
                 ("rc20", "20 MHz (RC interno) - modo seguro", "-DSYS_CLK=0")],
         upload=["jlink", "pyocd"]),
]

COMMON_MENUS = [
    ("opt", [("os", "Más pequeño (-Os) [por defecto]", "build.opt", "-Os"),
             ("o2", "Rápido (-O2)", "build.opt", "-O2"),
             ("o3", "Más rápido (-O3)", "build.opt", "-O3"),
             ("og", "Depuración (-Og)", "build.opt", "-Og")]),
    ("lto", [("off", "Desactivado", "build.lto", ""),
             ("on", "Activado (-flto)", "build.lto", "-flto")]),
]


def gen():
    out = ["# Arduino-Arm-Cortex-M (CEPH81) - placas",
           "# ARCHIVO GENERADO por tools/gen_boards.py: editar allí, no aquí.", "",
           "menu.clock=Reloj de CPU",
           "menu.upload_method=Método de carga",
           "menu.opt=Optimización",
           "menu.lto=LTO (optimización al enlazar)", ""]
    for b in BOARDS:
        i = b["id"]
        p = lambda k, v: out.append("%s.%s=%s" % (i, k, v))
        out.append("# " + "-" * 60)
        out.append("# " + b["name"])
        p("name", b["name"])
        p("build.core", "arduino")
        p("build.board", b["variant"])
        p("build.variant", b["variant"])
        p("build.ldscript", "ldscript.ld")
        p("build.mcu", b["mcu"])
        p("build.series_flags", b["flags"])
        p("build.vendor_dir", b["vendor"])
        p("build.hse_flags", ("-D%s=%dUL" % b["hse"]) if b.get("hse") else "")
        p("upload.maximum_size", b["flash"])
        p("upload.maximum_data_size", b["ram"])
        p("upload.jlink_device", b["jlink"])
        if b.get("openocd"):
            p("upload.openocd_target", b["openocd"])
        if b.get("pyocd"):
            p("upload.pyocd_target", b["pyocd"].lstrip("_"))
        for key, label, flags in b["clocks"]:
            p("menu.clock.%s" % key, label)
            p("menu.clock.%s.build.clock_flags" % key, flags)
        for key in b["upload"]:
            label, tool, extra = UPLOAD[key]
            p("menu.upload_method.%s" % key, label)
            p("menu.upload_method.%s.upload.tool" % key, tool)
            p("menu.upload_method.%s.upload.tool.default" % key, tool)
            for k, v in extra.items():
                p("menu.upload_method.%s.%s" % (key, k), v)
        for menu, opts in COMMON_MENUS:
            for key, label, prop, val in opts:
                p("menu.%s.%s" % (menu, key), label)
                p("menu.%s.%s.%s" % (menu, key, prop), val)
        out.append("")
    path = os.path.join(os.path.dirname(__file__), "..", "cortexm", "boards.txt")
    with open(path, "w", newline="\n") as f:
        f.write("\n".join(out))
    print("boards.txt: %d placas" % len(BOARDS))


if __name__ == "__main__":
    gen()
