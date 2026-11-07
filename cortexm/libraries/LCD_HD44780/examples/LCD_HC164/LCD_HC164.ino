/*
 * LCD_HC164 - LCD HD44780 controlada por un 74HC164 (2 hilos).
 *
 * Pines: DATA = PIN_SPI_MOSI, CLK = PIN_SPI_SCK (cámbielos si los usa).
 * Mapeo asumido del 164: Q0=RS Q1=RW Q2=E Q3=luz Q4..Q7=D4..D7.
 * Si sale basura en pantalla, el adaptador usa otro orden de bits.
 */
#include <LCD_HC164.h>

LCD_HC164 lcd(PIN_SPI_MOSI, PIN_SPI_SCK);   // DATA, CLK

void setup() {
  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.print("Hola Cortex-M");
  lcd.setCursor(0, 1);
  lcd.print("LCD via HC164!");
}

void loop() {
}
