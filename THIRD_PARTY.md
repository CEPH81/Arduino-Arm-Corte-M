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
| Air105 SDK MDK (MegaHunt / Hezhou) | `cortexm/system/MegaHunt` | Apache-2.0 |
| Protocolo del cargador del Air105 (air105-uploader, racerxdl) | `tools/air105load` (reimplementado en Go) | Apache-2.0 |
| Tablas de pines ADC/PWM de STM32 (PeripheralPins.c de stm32duino, base CubeMX) | `cortexm/cores/arduino/hal/pins_st.h` (generado con `tools/gen_pins_st.py`) | BSD-3-Clause |
| AT32 ADC driver (Artery) | `cortexm/system/Artery/*/Driver/*_adc.c` | Licencia de Artery |
| HC32F460 DDL: ADC y Timer A (HDSC) | `cortexm/system/HDSC/HC32F460/Driver` | BSD-3-Clause |
| SWM341 CSL: ADC y PWM (Synwit, vía RT-Thread) | `cortexm/system/Synwit/SWM341/Driver` | Apache-2.0 |
| Air105 SDK: QSPI y caché (MegaHunt / LuatOS) | `cortexm/system/MegaHunt/Air105/Driver` | Apache-2.0 |
| Secuencias de ADC y PWM del Air105 (BSP de LuatOS, core_adc.c / core_hwtimer.c) | `cortexm/cores/arduino/hal/analog_air105.c` (reescrito) | MIT |

Archivos del fabricante modificados (marcados con `CEPH81` en el código):
- `variants/GD32F427V_START/system_gd32f4xx.c`: reloj elegible desde el menú y
  arranque con IRC16M si el cristal no responde (antes: `while(1)`).
- `variants/SWM*/system_SWM*.c`: `SYS_CLK` y `__HSE` elegibles desde el menú.

Los pines de Timer A y ADC del HC32F460 (`hal/analog_hc32.c`) salen de la tabla
de funciones del datasheet de HDSC; los drivers DDL de ADC y Timer A no traen
`ddl_config.h`, así que se quitó su guarda `DDL_xxx_ENABLE` (marcado `CEPH81`).

Las tablas de vectores de `cortexm/cores/arduino/startup/vectors` se generan con
`tools/gen_vectors.py` a partir de los startups oficiales de cada fabricante.
