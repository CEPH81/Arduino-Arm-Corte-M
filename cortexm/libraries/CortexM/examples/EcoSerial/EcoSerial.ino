/*
 * EcoSerial - devuelve por Serial todo lo que recibe y cada segundo
 * informa el chip, la frecuencia de la CPU y millis().
 * Serial usa interrupciones y buffers: print() no bloquea.
 */
#include <CortexM.h>

void setup() {
  Serial.begin(115200);
  Serial.print("Hola desde ");
  Serial.print(cortexmChip());
  Serial.print(" a ");
  Serial.print(SystemCoreClock / 1000000UL);
  Serial.println(" MHz");
}

void loop() {
  static unsigned long t = 0;
  while (Serial.available()) Serial.write(Serial.read());
  if (millis() - t >= 1000) {
    t = millis();
    Serial.print("millis = ");
    Serial.println(t);
  }
}
