/*
 * 15_EscanerI2C - Busca dispositivos en el bus I2C y muestra su dirección.
 *
 * Pines: PIN_WIRE_SDA y PIN_WIRE_SCL (ver variant.h); con Wire.setPins()
 * se usan otros. Pull-ups de 4.7k a 3V3 (los módulos suelen traerlas).
 */
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  // Wire.setPins(SDA, SCL);
  Wire.begin();
  Wire.setClock(100000);
}

void loop() {
  int encontrados = 0;
  Serial.print("Buscando en SDA=");
  Serial.print(PIN_WIRE_SDA);
  Serial.print(" SCL=");
  Serial.print(PIN_WIRE_SCL);
  Serial.println(" (número de pin = puerto*16 + bit)");
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    uint8_t r = Wire.endTransmission();
    if (r == 0) {
      Serial.print("  Dispositivo en 0x");
      if (addr < 16) Serial.print('0');
      Serial.println(addr, HEX);
      encontrados++;
    } else if (r == 5) {
      Serial.println("  SCL se queda en bajo: ¿faltan las pull-ups?");
      break;
    }
  }
  Serial.print("Total: ");
  Serial.println(encontrados);
  Serial.println();
  delay(3000);
}
