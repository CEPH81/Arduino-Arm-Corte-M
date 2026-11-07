/*
 * cortexm_periph.h - Servicios de periféricos que implementa cada familia
 * (hal/analog_*.c, hal/flash_*.c) y que usan wiring_analog.cpp, tone(),
 * EEPROM, SPI y los ejemplos.
 *
 *  ADC   todos los chips de este núcleo tienen ADC de 12 bits.
 *  PWM   un temporizador por pin; los pines de un mismo temporizador
 *        comparten la frecuencia.
 *  Timer un temporizador libre (no se usa para PWM) para cmTimerStart().
 *  Flash borrado por página y escritura en unidades de CM_FLASH_UNIT bytes.
 */
#ifndef CORTEXM_PERIPH_H
#define CORTEXM_PERIPH_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- ADC ------------------------------------------------------- */
#define CM_ADC_BITS     12
#define CM_ADC_NONE     (-1)
#define CM_TEMP_NONE    (-100000)   /* cmAdcTemperature(): el chip no tiene sensor */

int      cmAdcRead(uint8_t pin);           /* 0..4095 o CM_ADC_NONE */
int32_t  cmAdcTemperature(void);           /* centésimas de °C o CM_TEMP_NONE */
uint32_t cmAdcRefMv(void);                 /* mV que corresponden a 4095 */

/* ---------- PWM ------------------------------------------------------- */
/* value: 0 .. (1 << bits); (1 << bits) = 100 %. Devuelve false si el pin no tiene PWM */
bool     cmPwmWrite(uint8_t pin, uint32_t freq, uint32_t value, uint8_t bits);
void     cmPwmStop(uint8_t pin);
bool     cmPwmPin(uint8_t pin);

/* ---------- Temporizador periódico ------------------------------------ */
bool     cmTimerStart(uint32_t periodUs, void (*cb)(void));
void     cmTimerStop(void);

/* ---------- Flash (EEPROM emulada) ------------------------------------ */
/* La variante (o la familia) define CM_EEPROM_ADDR, CM_EEPROM_PAGE y
 * CM_FLASH_UNIT. Las funciones devuelven false si el hardware dio error. */
bool     cmFlashErase(uint32_t addr);
bool     cmFlashWrite(uint32_t addr, const void *data, uint32_t len);

/* Gancho de 1 ms (tone() con duración); lo llama SysTick_Handler */
extern void (*volatile cm_tick_hook)(void);

#ifdef __cplusplus
}
#endif

#endif /* CORTEXM_PERIPH_H */
