/*
 * 03_AnalogRead - Lee un potenciómetro en A0 y muestra el valor crudo y
 * los milivoltios.
 *
 * Potenciómetro: extremos a 3V3 y GND, cursor a A0 (ver A0 en variant.h).
 * El ADC es de 12 bits: con analogReadResolution(12) el valor va de 0 a
 * 4095; por defecto (10 bits, como Arduino UNO) va de 0 a 1023.
 * El tope es VDDA (3.3 V); en el Air105, 3.76 V (divisor interno).
 */
void setup() {
  Serial.begin(115200);
  analogReadResolution(12);       // 0..4095
}

void loop() {
  int crudo = analogRead(A0);
  uint32_t mv = analogReadMillivolts(A0);
  Serial.print("A0 = ");
  Serial.print(crudo);
  Serial.print("  (");
  Serial.print(mv);
  Serial.println(" mV)");
  delay(250);
}
