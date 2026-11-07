/*
 * 08_PotenciometroADC - Valor crudo de 12 bits (0-4095) de un potenciómetro
 * en A0, por Serial cada 500 ms, y una barra de texto.
 *
 * Potenciómetro: extremos a 3V3 y GND, cursor a A0.
 */
void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  uint16_t val = analogRead(A0);
  Serial.print("ADC: ");
  Serial.print(val);
  Serial.print("  |");
  for (uint16_t i = 0; i < 32; i++) Serial.print(i < val / 128 ? '#' : ' ');
  Serial.println('|');
  delay(500);
}
