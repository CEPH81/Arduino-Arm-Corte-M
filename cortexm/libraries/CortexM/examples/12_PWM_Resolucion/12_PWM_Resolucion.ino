/*
 * 12_PWM_Resolucion - PWM de alta resolución: 16 bits a 1 kHz, barrido de
 * 0 % a 100 % y vuelta en PIN_PWM1 (LED o osciloscopio).
 *
 * analogWriteResolution(bits) cambia la escala de analogWrite() (1..16);
 * analogWriteFrequency(Hz) cambia la frecuencia. La resolución real la da
 * el temporizador: reloj / frecuencia pasos (p. ej. 84 MHz / 1 kHz = 84000).
 */
const uint8_t PIN_SALIDA = PIN_PWM1;
const uint32_t MAXIMO = 65535;       // 16 bits

void setup() {
  analogWriteResolution(16);
  analogWriteFrequency(1000);
}

void loop() {
  for (uint32_t duty = 0; duty <= 100; duty++) {
    analogWrite(PIN_SALIDA, (int)(MAXIMO * duty / 100));
    delay(10);
  }
  for (uint32_t duty = 100; duty > 0; duty--) {
    analogWrite(PIN_SALIDA, (int)(MAXIMO * duty / 100));
    delay(10);
  }
}
