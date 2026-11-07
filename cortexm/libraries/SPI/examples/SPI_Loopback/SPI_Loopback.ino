/*
 * SPI_Loopback - Prueba del SPI sin dispositivos externos.
 *
 * Una con un cable PIN_SPI_MOSI y PIN_SPI_MISO (ver variant.h de su placa).
 * Cada byte enviado debe volver igual. Resultado por Serial (115200 baudios).
 */
#include <SPI.h>

void setup() {
  Serial.begin(115200);
  SPI.begin();
}

void loop() {
  uint8_t errores = 0;
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  for (int i = 0; i < 256; i++) {
    uint8_t r = SPI.transfer((uint8_t)i);
    if (r != (uint8_t)i) errores++;
  }
  SPI.endTransaction();

  Serial.print("SPI loopback: ");
  Serial.println(errores == 0 ? "OK (256/256)" : "FALLO - revise el puente MOSI-MISO");
  delay(1000);
}
