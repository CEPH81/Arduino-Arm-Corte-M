/*
 * 10_HolaSerial - Envía un saludo por Serial cada segundo.
 */
#include <CortexM.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print("Hola desde ");
  Serial.print(cortexmChip());
  Serial.print("\r\n");
  delay(1000);
}
