/*
 * 16_RGB_Potenciometro - Rueda de colores: un potenciómetro en A0 recorre
 * los matices (hue) de un LED RGB con 3 canales PWM por hardware.
 *
 *   Rojo  -> PIN_PWM1     Verde -> PIN_PWM2     Azul -> PIN_PWM3
 * (cada uno con su resistencia; LED de cátodo común a GND).
 * Potenciómetro: extremos a 3V3/GND, cursor a A0.
 */
const uint8_t PIN_R = PIN_PWM1, PIN_G = PIN_PWM2, PIN_B = PIN_PWM3;

/* Matiz (0-359) a RGB de 8 bits con S = V = 255 */
void hsv2rgb(uint16_t h, uint8_t &r, uint8_t &g, uint8_t &b) {
  uint8_t region    = (uint8_t)(h / 60);
  uint8_t remainder = (uint8_t)(((h - (region * 60)) * 255) / 60);
  uint8_t v = 255, p = 0;
  uint8_t q = (uint8_t)((v * (255 - remainder)) / 255);
  uint8_t t = (uint8_t)((v * remainder) / 255);
  switch (region) {
    case 0:  r = v; g = t; b = p; break;
    case 1:  r = q; g = v; b = p; break;
    case 2:  r = p; g = v; b = t; break;
    case 3:  r = p; g = q; b = v; break;
    case 4:  r = t; g = p; b = v; break;
    default: r = v; g = p; b = q; break;
  }
}

void setup() {
  analogReadResolution(12);
  analogWriteFrequency(20000);    // 20 kHz: sin parpadeo ni zumbido
}

void loop() {
  uint16_t raw = analogRead(A0);                          // 0..4095
  uint16_t hue = (uint16_t)(((uint32_t)raw * 360u) / 4095u);
  if (hue >= 360) hue = 359;

  uint8_t r, g, b;
  hsv2rgb(hue, r, g, b);
  analogWrite(PIN_R, r);
  analogWrite(PIN_G, g);
  analogWrite(PIN_B, b);
  delay(20);
}
