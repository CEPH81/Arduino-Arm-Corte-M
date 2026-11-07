/*
 * Interrupcion - cambia el LED cada vez que se pulsa el botón de la placa.
 * Si tu variante no define USER_BTN, cambia BOTON por el pin que uses.
 */
#ifdef USER_BTN
#define BOTON USER_BTN
#else
#define BOTON PA0
#endif

volatile uint32_t pulsaciones = 0;

void alPulsar() {
  pulsaciones++;
  digitalToggle(LED_BUILTIN);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BOTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BOTON), alPulsar, FALLING);
}

void loop() {
  static uint32_t anterior = 0;
  if (pulsaciones != anterior) {
    anterior = pulsaciones;
    Serial.print("Pulsaciones: ");
    Serial.println(anterior);
  }
}
