/*
 * OLED_Hola - Texto en una OLED SSD1306 128x64.
 *
 * Conexiones: SDA = PIN_WIRE_SDA, SCL = PIN_WIRE_SCL (ver variant.h). Direccion 0x3C (algunos modulos: 0x3D).
 */
#include <Wire.h>
#include <OLED_SSD1306.h>

OLED_SSD1306 oled(0x3C);

void setup() {
  // Wire.setPins(SDA, SCL);  // otros pines (por defecto PIN_WIRE_SDA/SCL)
  Wire.begin();
  Wire.setClock(400000);    // la OLED admite 400 kHz

  oled.begin();
  oled.clear();
  oled.setCursor(0, 0);     // pagina 0, columna 0
  oled.print("HOLA Cortex-M");
  oled.setCursor(2, 0);
  oled.print("OLED SSD1306");
}

void loop() {
}
