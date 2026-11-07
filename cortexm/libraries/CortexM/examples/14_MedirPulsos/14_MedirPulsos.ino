/*
 * 14_MedirPulsos - Mide el tiempo en alto y en bajo de una señal con una
 * interrupción por cambio (CHANGE) y micros().
 *
 * Prueba sin equipo: el ejemplo genera PWM de 1 kHz al 25 % en PIN_PWM2;
 * una con un cable PIN_PWM2 y PIN_ENTRADA. Debería medir ~250 us en alto y
 * ~750 us en bajo. Señal de 0 a 3.3 V (no conecte 5 V directo).
 * Resolución: 1 us; útil hasta unos 20-50 kHz.
 */
const uint8_t PIN_ENTRADA = PIN_SPI_MISO;   // cualquier pin con interrupción

volatile uint32_t ultimoFlanco = 0, tAlto = 0, tBajo = 0;
volatile bool nuevo = false;

void alCambiar() {
  uint32_t ahora = micros();
  uint32_t d = ahora - ultimoFlanco;
  ultimoFlanco = ahora;
  if (digitalRead(PIN_ENTRADA)) tBajo = d;     // acaba de subir: terminó un tramo en bajo
  else { tAlto = d; nuevo = true; }            // acaba de bajar: terminó un tramo en alto
}

void setup() {
  Serial.begin(115200);
  analogWriteFrequency(1000);
  analogWrite(PIN_PWM2, 64);                   // 25 % de 255
  pinMode(PIN_ENTRADA, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_ENTRADA), alCambiar, CHANGE);
}

void loop() {
  if (nuevo) {
    noInterrupts();
    uint32_t a = tAlto, b = tBajo;
    nuevo = false;
    interrupts();
    Serial.print("Alto: ");
    Serial.print(a);
    Serial.print(" us   Bajo: ");
    Serial.print(b);
    Serial.print(" us   Frecuencia: ");
    Serial.print((a + b) ? 1000000UL / (a + b) : 0);
    Serial.println(" Hz");
    delay(500);
  }
}
