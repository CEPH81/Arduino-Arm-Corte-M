/*
 * 07_Printf - printf() de C por Serial.
 *
 * En este núcleo printf(), puts() y putchar() escriben por Serial después
 * de Serial.begin(). %f (float) agrega unos KB de código: si no hace falta,
 * imprima enteros.
 */
#include <CortexM.h>

void setup() {
  Serial.begin(115200);
  delay(100);
  printf("Chip: %s\r\n", cortexmChip());
  printf("Reloj de la CPU: %lu Hz\r\n", (unsigned long)SystemCoreClock);
}

void loop() {
  printf("millis = %lu\r\n", millis());
  delay(1000);
}
