/*
 * 13_Temporizador - Interrupción periódica cada 500 ms que cambia el LED.
 * Todo ocurre en la interrupción; loop() queda libre.
 *
 * cmTimerStart(microsegundos, función) usa un temporizador del chip que el
 * núcleo deja libre (TIM6/TIM17, TIMER5, TMR6, Timer A4, TIMR0 o TIM6 del
 * Air105, según la placa). La función corre dentro de la interrupción:
 * hágala corta. tone() en un pin sin PWM usa el mismo temporizador.
 */
volatile uint32_t ticks = 0;

void alVencer() {
  ticks++;
  digitalToggle(LED_BUILTIN);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  if (!cmTimerStart(500000, alVencer)) Serial.println("No se pudo iniciar el temporizador");
}

void loop() {
  static uint32_t antes = 0;
  if (ticks != antes) {
    antes = ticks;
    Serial.print("Interrupciones: ");
    Serial.println(antes);
  }
}
