/*
 * Parpadeo - enciende y apaga el LED de la placa cada 500 ms.
 * LED_BUILTIN y LED_BUILTIN_ON (nivel que lo enciende) vienen de la variante.
 */
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ON);
  delay(500);
  digitalWrite(LED_BUILTIN, !LED_BUILTIN_ON);
  delay(500);
}
