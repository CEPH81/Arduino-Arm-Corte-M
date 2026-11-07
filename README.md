# Arduino Arm Cortex-M (CEPH81)

Núcleo Arduino para microcontroladores **Arm Cortex-M de varios fabricantes** (ST, GigaDevice, Artery, HDSC y Synwit) en un solo paquete para Arduino IDE 2.x y arduino-cli. Lo instala el **Gestor de placas** junto con su compilador (xPack GCC 14) y OpenOCD.

Canal: [MikrocontrollerCephs](https://www.youtube.com/@mikrocontrollersceph)

## Placas

| Placa | Chip | Núcleo / reloj | Carga |
|---|---|---|---|
| STM32F407VET6 Black (F4VE) | STM32F407VE | M4F · 168 MHz | ST-LINK, CMSIS-DAP, J-Link, pyOCD, CubeProgrammer |
| Nucleo-32 L432KC | STM32L432KC | M4F · 80 MHz | ST-LINK (integrado), CMSIS-DAP, J-Link, pyOCD, CubeProgrammer |
| WeAct STM32G431CB | STM32G431CB | M4F · 170 MHz | ST-LINK, CMSIS-DAP, J-Link, pyOCD, CubeProgrammer |
| WeAct STM32WB55CG | STM32WB55CG | M4F · 64 MHz | ST-LINK, CMSIS-DAP, J-Link, pyOCD, CubeProgrammer |
| STM32H503 (genérica) | STM32H503RB | M33F · 250 MHz | CubeProgrammer, J-Link, pyOCD |
| GD32F427V-START | GD32F427VK | M4F · 200 MHz | CMSIS-DAP (GD-Link), J-Link, pyOCD, ST-LINK |
| WeAct AT32F403A | AT32F403ACG | M4F · 240 MHz | J-Link, pyOCD |
| AT-START-F437 | AT32F437ZM | M4F · 288 MHz | J-Link, pyOCD (AT-Link es CMSIS-DAP) |
| HC32F460JETA | HC32F460JE | M4F · 200 MHz | J-Link, pyOCD |
| Synwit SWM181CBT6 (placa azul) | SWM181xB | M0 · 48 MHz | J-Link, pyOCD |
| Synwit SWM34SVET6 | SWM341xE | M33F · 120 MHz | J-Link, pyOCD |
| CORE-Air105 (Hezhou / LuatOS) | Air105 (MH1903S) | M4F · 204 MHz | USB-serie de la placa (sin sonda) |

Pendiente: **CY8CKIT-043** (PSoC 4200M). Infineon no publica los headers de la PSoC 4200M fuera de PSoC Creator/ModusToolbox; se añadirá a partir de los archivos generados por esas herramientas.

En las placas marcadas "verificar" dentro de su `variant.h` (LED, botón o pines del USB-serie), los pines salen de la documentación del fabricante o de sus ejemplos, no de una prueba en la placa real.

## Instalación

1. Arduino IDE → **Archivo › Preferencias › URLs adicionales del Gestor de placas**:
   ```
   https://raw.githubusercontent.com/CEPH81/Arduino-Arm-Corte-M/main/package_ceph81_cortexm_index.json
   ```
2. **Herramientas › Placa › Gestor de placas** → buscar *Cortex-M (CEPH81)* → Instalar.
3. Elegir la placa y, en **Herramientas › Método de carga**, la sonda que uses.

> Si el repositorio es privado, el IDE no puede leer el índice: hay que hacerlo público o instalar a mano (ver *Desarrollo*).

### Sondas

| Sonda | Qué elegir |
|---|---|
| ST-LINK V2 (también clones) | *ST-LINK (OpenOCD)* — solo STM32 y GD32F4 |
| WeAct Mini Debugger, DAPLink, GD-Link, AT-Link | *CMSIS-DAP / DAPLink (OpenOCD)* o *pyOCD* |
| SEGGER J-Link | *SEGGER J-Link* (instalar el *J-Link Software* de SEGGER) |
| Cualquiera, chips no-ST | *pyOCD*: `pip install pyocd` y una vez `pyocd pack install <objetivo>` |

**CORE-Air105:** se carga por el mismo USB-C de la placa (conversor USB-serie). Elegir *USB-serie (cargador de la ROM del Air105)*, el puerto COM y **Subir**. La herramienta `air105load` reinicia el chip con la línea RTS y habla con el cargador de la ROM; no hace falta pulsar BOOT. El Monitor Serie debe estar cerrado durante la carga. Grabar un programa de Arduino borra LuatOS; se restaura con *Luatools* de Hezhou. LED integrado: PD14 (también PD15 y PC3). `Serial` = UART0 en PA0 (RX) / PA1 (TX).

Los objetivos de pyOCD de cada placa están en `upload.pyocd_target` de `boards.txt` (p.ej. `gd32f427vk`, `hc32f460xe`, `swm341xe`).

**Synwit:** J-Link no trae de fábrica el SWM181 ni el SWM341. Hay que añadir el soporte que publica Synwit (entrada en `JLinkDevices.xml` y algoritmo de flash), o usar pyOCD con el `.pack` de Synwit (`pyocd load --pack <archivo.pack> ...`).

## Qué funciona

| Función | Estado |
|---|---|
| `pinMode`, `digitalWrite/Read`, `digitalToggle` | ✓ todas |
| `digitalWriteFast/ReadFast/ToggleFast` (1–2 instrucciones) | ✓ todas |
| `millis`, `micros`, `delay`, `delayMicroseconds` (DWT en M3+) | ✓ todas |
| `Serial` por interrupciones, buffers RX 256 / TX 128, `print()` no bloquea | ✓ todas |
| `attachInterrupt` (RISING, FALLING, CHANGE; LOW/HIGH donde el hardware lo permite) | ✓ todas |
| `shiftIn/Out`, `pulseIn`, `String`, `Print`, `Stream` (ArduinoCore-API) | ✓ todas |
| Reloj máximo + modo seguro (oscilador interno) desde el menú | ✓ todas |
| Si el cristal no arranca, se sigue con el oscilador interno | ✓ STM32F407, GD32, AT32, HC32 |
| `analogRead` (12 bits), `analogReadMillivolts`, `analogReadResolution` | ✓ todas |
| `analogWrite` (PWM por hardware), `analogWriteResolution` (1–16 bits), `analogWriteFrequency` | ✓ todas |
| `tone` / `noTone` (cualquier pin) | ✓ todas |
| `printf` por `Serial` | ✓ todas |
| Temporizador periódico `cmTimerStart(us, función)` | ✓ todas |
| `analogReadTemperature` (sensor interno) | ✓ STM32, GD32, AT32 |
| `SPI` | ✓ todas (hardware o software, ver tabla) |
| `Wire` (I2C maestro por software, cualquier pin) | ✓ todas |
| `EEPROM` (1 KB emulada en flash) | ✓ todas menos SWM181 |
| USB nativo (`SerialUSB`) | siguiente fase |

### Pines de periféricos por placa

Los nombres de abajo están en el `variant.h` de cada placa; los ejemplos los usan para funcionar en todas sin cambios.

| Placa | A0… | PWM1 / PWM2 / PWM3 · TONE | SPI (SCK MISO MOSI SS) | I2C (SDA SCL) | EEPROM |
|---|---|---|---|---|---|
| F407VE Black | PA1 PA2 PA3 PA4 PA5 PB1 | PC6 PC7 PC8 · PC9 | hw SPI1: PB3 PB4 PB5 PB0 | PB9 PB8 | sector 7 (128 KB) |
| Nucleo L432KC | A0–A7 del Nucleo | PA8 PA9 PA10 · PA11 | hw SPI1: PB3 PB4 PB5 PA11 | PB7 PB6 | 2 KB |
| WeAct G431 | PA0 PA1 PA2 PA3 PB0 PB1 | PB4 PB5 PB6 · PB10 | hw SPI1: PA5 PA6 PA7 PA4 | PB7 PA15 | 2 KB |
| WeAct WB55 | PA0 PA1 PA2 PA3 | PA8 PA15 PB3 · PB4 | hw SPI1: PA5 PA6 PA7 PA4 | PB9 PB8 | 4 KB |
| STM32H503 | PA0 PA1 PA3 PB0 PC1 PC0 | PC6 PC7 PC8 · PB10 | sw: PA5 PA6 PA7 PA4 | PB7 PB6 | 8 KB |
| GD32F427V-START | PA1 PA2 PA3 PC0 PC1 PC2 | PC7 PC8 PC9 · PB10 | hw SPI0: PA5 PA6 PA7 PA4 | PB7 PB6 | sector 27 (256 KB) |
| WeAct AT32F403A | PA1 PA2 PA3 PB0 | PB8 PB9 PA8 · PB1 | hw SPI1: PA5 PA6 PA7 PA4 | PB7 PB6 | 2 KB |
| AT-START-F437 | PA0 PA1 PA4 PB0 PC1 PC0 | PC6 PC7 PC8 · PB10 | hw SPI1: PA5 PA6 PA7 PA4 | PB9 PB8 | 4 KB |
| HC32F460JETA | PA1 PA2 PA3 PB0 PB1 PC0 | PB13 PB14 PB15 · PA8 | sw: PA5 PA6 PA7 PA4 | PB7 PB6 | 8 KB |
| SWM181CBT6 | PA12 PA15 PC6 PC7 | PB9 PB8 PB10 · PB11 | sw: PB4 PB5 PB6 PB7 | PB2 PB3 | — |
| SWM34SVET6 | PC6 PC5 PC4 PC1 PC0 PA15 | PB3 PB1 PB15 · PD9 | sw: PB4 PB5 PB6 PB7 | PA0 PA1 | 4 KB |
| CORE-Air105 | PC0 PC1 PC4 PC5 | PB0 PB1 PC6 · PC7 | sw: PB2 PB5 PB4 PB3 | PE7 PE6 | 4 KB (QSPI) |

- **PWM:** los pines de un mismo temporizador comparten la frecuencia. Lista completa de pines con PWM y ADC: `hal/pins_st.h` (ST, GD32, AT32) y `hal/analog_*.c`.
- **ADC:** tope = VDDA (3.3 V); en el Air105 el divisor interno da 0–3.76 V.
- **EEPROM:** la página reservada ya no cuenta como espacio del programa. Cada byte que cambia se añade como registro y la página solo se borra cuando se llena (en STM32F4/GD32F4 el borrado de un sector grande para la CPU ~1–2 s).
- **Temporizador libre** (`cmTimerStart`, y `tone()` en pines sin PWM): TIM6 (TIM17 en WB55), TIMER5 en GD32, TMR6 en AT32, Timer A4 en HC32, TIMR0 en Synwit, TIM6 en Air105.

Menús: *Reloj de CPU*, *Método de carga*, *Optimización* (-Os, -O2, -O3, -Og) y *LTO*.

## Ejemplos

**Archivo › Ejemplos › CortexM** (todas las placas, sin cambiar pines):
01 Parpadeo · 02 EcoSerial · 03 AnalogRead · 04 FadeLED · 05 Interrupcion · 06 Tono · 07 Printf · 08 PotenciometroADC · 09 Boton · 10 HolaSerial · 11 EcoLineas · 12 PWM_Resolucion · 13 Temporizador · 14 MedirPulsos · 15 EscanerI2C · 16 RGB_Potenciometro · 17 TemperaturaInterna · 18 AhorroMemoria · 19 VelocidadGPIO

**Bibliotecas incluidas**, cada una con su ejemplo: `SPI` (SPI_Loopback), `Wire` (I2C_Scanner), `EEPROM` (EEPROM_Contador), `OLED_SSD1306` (OLED_Hola), `LCD_HD44780` (LCD_PCF8574, LCD_HC164), `NTC_Thermistor` (NTC_OLED), `HCSR04` (HCSR04_OLED), `CM_Log` (LogDemo).

## Cómo está organizado

```
cortexm/
  boards.txt              generado por tools/gen_boards.py
  platform.txt            compilador, recetas, carga y depuración
  cores/arduino/
    api/                  ArduinoCore-API (oficial)
    startup/              arranque común en C + tablas de vectores generadas
    hal/                  capa de hardware por "estilo" de periférico
  variants/<PLACA>/       pines, reloj, linker script y system_*.c del fabricante
  system/<Fabricante>/    headers CMSIS y drivers oficiales usados
  libraries/CortexM/      ejemplos (Archivo › Ejemplos › CortexM)
  libraries/<otras>/      SPI, Wire, EEPROM, OLED_SSD1306, LCD_HD44780, NTC_Thermistor, HCSR04, CM_Log
tools/                    generadores y script de publicación
```

Muchos chips copian los periféricos de ST, así que el código se agrupa por **estilo de periférico** y no por marca:

- **GPIO estilo 1** (MODER/BSRR): STM32 F4/L4/G4/WB/H5, GD32F4, AT32F43x.
- **GPIO estilo 2** (CRL/CRH): AT32F403A.
- **GPIO estilo 3**: HC32F460 · **estilo 4**: Synwit.
- **USART estilo 1** (SR/DR): STM32F4, GD32F4, AT32 · **estilo 2** (ISR/RDR/TDR): STM32 L4/G4/WB/H5.

El número de pin Arduino es `puerto*16 + bit` (`PA0 = 0`, `PB0 = 16`…), así `digitalWrite` no usa tablas y con constantes compila a un solo store.

Todas las familias arrancan con el mismo `Reset_Handler` en C (`startup/cm_startup.c`): FPU → `.data`/`.bss` → `SystemInit()` del fabricante → VTOR → constructores C++ → `main()`.

## Añadir una placa

1. Crear `cortexm/variants/<PLACA>/` con `variant.h`, `variant.cpp` (Serial y `SystemClock_Config`), `ldscript.ld` y, si hace falta, el `system_*.c` del fabricante.
2. Si el chip es nuevo, generar su tabla de vectores:
   `python3 tools/gen_vectors.py <startup.s> <MACRO_DEL_CHIP> cortexm/cores/arduino/startup/vectors/vectors_<chip>.c`
3. Añadir la placa a `BOARDS` en `tools/gen_boards.py` y ejecutar `python3 tools/gen_boards.py`.

## Desarrollo

- Instalación manual: copiar o enlazar `cortexm/` a `<Sketchbook>/hardware/CEPH81/cortexm` y crear `cortexm/platform.local.txt` con `compiler.path=<ruta a arm-none-eabi-gcc>/bin/`.
- Cada push compila los ejemplos en todas las placas (GitHub Actions, *Compilar placas*).
- Para publicar una versión basta con subir `version=` en `cortexm/platform.txt` y hacer push a `main`: el flujo *Publicar versión* crea la etiqueta, la Release con el zip y actualiza el índice.

## Licencia

Código propio del núcleo: LGPL-2.1 (igual que ArduinoCore-API). Los archivos de los fabricantes mantienen su licencia; ver [THIRD_PARTY.md](THIRD_PARTY.md).
