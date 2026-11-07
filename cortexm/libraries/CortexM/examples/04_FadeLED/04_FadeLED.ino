/*
 * 04_FadeLED - Brillo del LED con PWM por hardware.
 *
 * Usa PIN_PWM1 (un pin con PWM de la placa, ver variant.h). Conecte un LED
 * con su resistencia de 330 ohm a GND. Si el LED de la placa está en un pin
 * con PWM, cambie PIN_PWM1 por LED_BUILTIN.
 * analogWrite() es de 8 bits (0..255) a 1 kHz por defecto.
 */
const uint8_t PIN_LED = PIN_PWM1;
int brillo = 0;
int paso = 5;

void setup() {
  analogWriteFrequency(1000);   // Hz (los pines del mismo temporizador la comparten)
}

void loop() {
  analogWrite(PIN_LED, brillo);
  brillo += paso;
  if (brillo <= 0 || brillo >= 255) paso = -paso;
  delay(20);
}
