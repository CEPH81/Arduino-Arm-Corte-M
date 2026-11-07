/*
 * VelocidadGPIO - compara digitalWrite() con digitalWriteFast().
 * Mide con micros() cuántos cambios de pin por segundo se consiguen.
 * Con un osciloscopio en LED_BUILTIN se ve la frecuencia real.
 */
#include <CortexM.h>

const uint32_t N = 100000;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(100);

  uint32_t t0 = micros();
  for (uint32_t i = 0; i < N; i++) { digitalWrite(LED_BUILTIN, HIGH); digitalWrite(LED_BUILTIN, LOW); }
  uint32_t lento = micros() - t0;

  t0 = micros();
  for (uint32_t i = 0; i < N; i++) { digitalWriteFast(LED_BUILTIN, HIGH); digitalWriteFast(LED_BUILTIN, LOW); }
  uint32_t rapido = micros() - t0;

  Serial.print(cortexmChip());
  Serial.print(" @ ");
  Serial.print(SystemCoreClock / 1000000UL);
  Serial.println(" MHz");
  Serial.print("digitalWrite:     "); Serial.print(2.0f * N / lento, 2); Serial.println(" M cambios/s");
  Serial.print("digitalWriteFast: "); Serial.print(2.0f * N / rapido, 2); Serial.println(" M cambios/s");
}

void loop() { }
