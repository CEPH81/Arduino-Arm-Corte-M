/*
 * LCD_PCF8574 - LCD 16x2 con modulo I2C PCF8574.
 *
 * Conexiones: SDA = PIN_WIRE_SDA, SCL = PIN_WIRE_SCL (ver variant.h) (pull-up de 4.7k recomendadas).
 * Direccion del PCF8574: 0x26 (use el ejemplo I2C_Scanner si no la sabe).
 */
#include <Wire.h>
#include <LCD_PCF8574.h>

LCD_PCF8574 lcd(0x26);

void setup() {
  // Wire.setPins(SDA, SCL);  // otros pines (por defecto PIN_WIRE_SDA/SCL)
  Wire.begin();
  lcd.begin();

  lcd.setCursor(0, 0);
  lcd.print("Hola Cortex-M");
  lcd.setCursor(0, 1);
  lcd.print("Val: ");
  lcd.print(23.456, 2);     // -> "23.46"
  lcd.print(" cnt=");
  lcd.print(42);
}

void loop() {
  // el mensaje queda fijo en pantalla
}
