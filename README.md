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
| `analogRead`, `analogWrite` (PWM), `Wire`, `SPI`, USB | siguiente fase |

Menús: *Reloj de CPU*, *Método de carga*, *Optimización* (-Os, -O2, -O3, -Og) y *LTO*.

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
