# Componentes de terceros

| Componente | Carpeta | Licencia |
|---|---|---|
| ArduinoCore-API | `cortexm/cores/arduino/api` | LGPL-2.1 |
| CMSIS 5 (Arm) | `cortexm/system/CMSIS` | Apache-2.0 |
| Headers CMSIS de STMicroelectronics (F4, L4, G4, WB, H5) | `cortexm/system/ST`, `system_stm32*.c` | Apache-2.0 |
| GD32F4xx Firmware Library (GigaDevice) | `cortexm/system/GigaDevice`, `system_gd32f4xx.c` | BSD-3-Clause |
| AT32F403A/F435_437 Firmware Library (Artery) | `cortexm/system/Artery`, `system_at32*.c` | Licencia de Artery (uso con MCUs Artery) |
| HC32F460 DDL 2.2 (HDSC) | `cortexm/system/HDSC`, `system_hc32f460.c` | BSD-3-Clause |
| SWM181 / SWM341 Lib (Synwit) | `cortexm/system/Synwit`, `system_SWM*.c` | Apache-2.0 |

Archivos del fabricante modificados (marcados con `CEPH81` en el código):
- `variants/GD32F427V_START/system_gd32f4xx.c`: reloj elegible desde el menú y
  arranque con IRC16M si el cristal no responde (antes: `while(1)`).
- `variants/SWM*/system_SWM*.c`: `SYS_CLK` y `__HSE` elegibles desde el menú.

Las tablas de vectores de `cortexm/cores/arduino/startup/vectors` se generan con
`tools/gen_vectors.py` a partir de los startups oficiales de cada fabricante.
