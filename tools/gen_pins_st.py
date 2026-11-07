#!/usr/bin/env python3
"""
gen_pins_st.py - Genera cortexm/cores/arduino/hal/pins_st.h (pines con ADC y
PWM) a partir de los PeripheralPins.c de stm32duino/Arduino_Core_STM32.

  python3 tools/gen_pins_st.py <carpeta variants de Arduino_Core_STM32>

Para cada chip se toma, por pin, la primera entrada que no es _ALTx ni canal
complementario (CHxN). GD32F427 y AT32F437 usan la tabla del STM32F407
(mismo mapa de ADC y de funciones alternativas de los temporizadores).
"""
import os, re, sys

CHIPS = [
    # (macro de la familia/chip, carpeta, temporizadores excluidos)
    ("CM_PINS_F407", "STM32F4xx/F407V(E-G)T_F417V(E-G)T", set()),
    ("CM_PINS_L432", "STM32L4xx/L432K(B-C)U_L442KCU", set()),
    ("CM_PINS_G431", "STM32G4xx/G431C(6-8-B)U_G441CBU", set()),
    ("CM_PINS_WB55", "STM32WBxx/WB35C(C-E)UxA_WB55C(C-E-G)U", {17}),   # TIM17 = cmTimerStart
    ("CM_PINS_H503", "STM32H5xx/H503RBT", set()),
]

def pin_num(name):
    m = re.fullmatch(r"P([A-K])_(\d+)", name)
    if not m:
        return None
    return (ord(m.group(1)) - ord("A")) * 16 + int(m.group(2))

def table(src, name):
    m = re.search(r"PinMap_%s\[\]\s*=\s*\{(.*?)\n\};" % name, src, re.S)
    return m.group(1) if m else ""

def parse(path, excl):
    src = open(path).read()
    adc, tim = {}, {}
    for line in table(src, "ADC").splitlines():
        m = re.search(r"\{(P[A-K]_\d+),\s*ADC(\d),\s*STM_PIN_DATA_EXT\([^,]+,[^,]+,\s*\d+,\s*(\d+),", line)
        if not m:
            continue
        p = pin_num(m.group(1))
        if p is not None and p not in adc:
            adc[p] = (int(m.group(2)), int(m.group(3)))
    for line in table(src, "TIM").splitlines():
        m = re.search(r"\{(P[A-K]_\d+),\s*TIM(\d+),\s*STM_PIN_DATA_EXT\([^,]+,[^,]+,\s*GPIO_AF(\d+)_\w+,\s*(\d),\s*(\d)\)", line)
        if not m or m.group(5) != "0":
            continue
        p, t = pin_num(m.group(1)), int(m.group(2))
        if p is None or p in tim or t in excl:
            continue
        tim[p] = (t, int(m.group(4)), int(m.group(3)))
    return adc, tim

def pname(p):
    return "P%s%d" % (chr(ord("A") + p // 16), p % 16)

def main():
    base = sys.argv[1]
    out = ["/* Generado por tools/gen_pins_st.py desde stm32duino (PeripheralPins.c). No editar. */",
           "/* adc: {pin, (ADC << 5) | canal}   pwm: {pin, TIM, canal, AF} */"]
    first = True
    for macro, folder, excl in CHIPS:
        adc, tim = parse(os.path.join(base, folder, "PeripheralPins.c"), excl)
        out.append(("#if" if first else "#elif") + " defined(%s)" % macro)
        first = False
        out.append("static const cm_adc_pin_t cm_adc_pins[] = {")
        out += ["  {%s, (%d << 5) | %d}," % (pname(p), a, c) for p, (a, c) in sorted(adc.items())]
        out.append("};")
        out.append("static const cm_pwm_pin_t cm_pwm_pins[] = {")
        out += ["  {%s, %d, %d, %d}," % (pname(p), t, c, af) for p, (t, c, af) in sorted(tim.items())]
        out.append("};")
    out += ["#endif", ""]
    dst = os.path.join(os.path.dirname(__file__), "..", "cortexm", "cores", "arduino", "hal", "pins_st.h")
    open(dst, "w").write("\n".join(out))
    print("pins_st.h:", len(out), "líneas")

if __name__ == "__main__":
    main()
