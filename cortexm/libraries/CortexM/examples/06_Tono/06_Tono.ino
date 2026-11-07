/*
 * 06_Tono - Melodía con un zumbador pasivo.
 *
 * Conecte el zumbador entre PIN_TONE y GND (ver variant.h). En un pin con
 * PWM, tone() usa el temporizador del pin; en cualquier otro pin usa el
 * temporizador interno de cmTimerStart() (un tono a la vez).
 */
const uint8_t PIN_ZUMBADOR = PIN_TONE;
const unsigned int notas[] = { 262, 294, 330, 349, 392, 440, 494, 523 };  // Do..Do

void setup() { }

void loop() {
  for (unsigned int i = 0; i < sizeof(notas) / sizeof(notas[0]); i++) {
    tone(PIN_ZUMBADOR, notas[i], 200);
    delay(250);
  }
  noTone(PIN_ZUMBADOR);
  delay(1500);
}
