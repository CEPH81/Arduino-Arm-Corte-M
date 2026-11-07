/*
 * 17_TemperaturaInterna - Sensor de temperatura interno del chip, por
 * Serial cada segundo.
 *
 * analogReadTemperature() devuelve centésimas de °C:
 *   STM32 (F4, L4, G4, WB, H5)  calibración de fábrica grabada en el chip
 *   GD32F4, AT32                valores típicos del fabricante (±5 °C)
 *   HC32F460, Synwit, Air105    sin sensor accesible: CM_TEMP_NONE
 * El chip se calienta unos grados por encima del ambiente. Para afinar,
 * mida el ambiente con un termómetro y ajuste AJUSTE_C.
 */
const float AJUSTE_C = 0.0f;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int32_t t = analogReadTemperature();
  if (t == CM_TEMP_NONE) {
    Serial.println("Este chip no tiene sensor de temperatura accesible.");
    delay(5000);
    return;
  }
  /* Promedio de 16 lecturas: el ruido baja varios décimos */
  int32_t suma = 0;
  for (int i = 0; i < 16; i++) suma += analogReadTemperature();
  float c = suma / 1600.0f + AJUSTE_C;
  Serial.print("Temperatura interna: ");
  Serial.print(c, 1);
  Serial.println(" C");
  delay(1000);
}
