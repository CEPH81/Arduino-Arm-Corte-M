/*
 * 09_Boton - Enciende el LED mientras se mantiene pulsado un botón.
 *
 * El botón conecta el pin a GND (se usa el pull-up interno). Si el botón
 * de su placa conecta a 3V3 (activo en alto), cambie LOW por HIGH abajo.
 */
#ifdef USER_BTN
const uint8_t PIN_BOTON = USER_BTN;   // botón de la placa
#else
const uint8_t PIN_BOTON = PA0;        // su placa no define USER_BTN: botón entre PA0 y GND
#endif

void setup() {
  pinMode(PIN_BOTON, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, !LED_BUILTIN_ON);    // apagado al inicio
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (digitalRead(PIN_BOTON) == LOW) {           // pulsado
    delay(20);                                   // antirrebote simple
    if (digitalRead(PIN_BOTON) == LOW) {
      digitalWrite(LED_BUILTIN, LED_BUILTIN_ON);
      return;
    }
  }
  digitalWrite(LED_BUILTIN, !LED_BUILTIN_ON);
}
