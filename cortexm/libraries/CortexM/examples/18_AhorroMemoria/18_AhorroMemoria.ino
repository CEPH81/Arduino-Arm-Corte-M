/*
 * 18_AhorroMemoria - Mide la memoria y muestra, con números reales, qué
 * técnicas ahorran RAM. Monitor Serie a 115200.
 *
 * Reglas que se comprueban aquí:
 *  1. Las tablas que no cambian van con "const": se quedan en la flash y no
 *     gastan RAM. Sin const se copian a la RAM al arrancar.
 *  2. Un búfer grande y local va en la pila solo mientras se usa la función;
 *     si es "static" o global ocupa la RAM siempre.
 *  3. String reserva y libera memoria dinámica (heap) y la fragmenta: en
 *     programas que corren días, mejor char[] de tamaño fijo y snprintf.
 *  4. En ARM no hace falta F("texto"): los textos ya quedan en la flash.
 *  5. printf con %f (float) agrega varios KB: imprima enteros si puede.
 */
#include <stdlib.h>
#include <CortexM.h>

extern "C" {
extern char _sdata, _edata, _sbss, _ebss, _estack, _sidata;
extern const char cm_vector_table[];
void *_sbrk(int incr);
}

/* 1. misma tabla: con const (flash) y sin const (RAM) */
const uint16_t tablaFlash[64] = { 1, 2, 3, 4, 5, 6, 7, 8 };
uint16_t tablaRAM[64] = { 1, 2, 3, 4, 5, 6, 7, 8 };

/* RAM libre entre el final del heap y la pila (lo que queda de verdad) */
unsigned long ramLibre(void) {
  char marca;                                  /* está en la pila */
  char *finHeap = (char *)_sbrk(0);
  return (unsigned long)(&marca - finHeap);
}

void linea(const char *texto, unsigned long valor, const char *unidad) {
  Serial.print(texto);
  Serial.print(valor);
  Serial.println(unidad);
}

bool enRam(const void *p) { return (const char *)p >= &_sdata && (const char *)p < &_estack; }

void direccion(const char *texto, const void *p) {
  Serial.print(texto);
  Serial.print("0x");
  Serial.print((unsigned long)p, HEX);
  Serial.println(enRam(p) ? "  (RAM)" : "  (flash: no usa RAM)");
}

/* 2. búfer local: solo ocupa pila mientras se ejecuta la función */
unsigned long usarBuferLocal(void) {
  volatile char bufer[2048];                   /* volatile: que el compilador no lo quite */
  for (int i = 0; i < 2048; i++) bufer[i] = (char)i;
  return ramLibre() + (unsigned long)(bufer[2047] & 0);   /* se lee: el búfer existe de verdad */
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.print("\r\n=== ");
  Serial.print(cortexmChip());
  Serial.println(": AhorroMemoria ===");

  Serial.println("-- Uso fijo (del mapa de memoria) --");
  linea("Flash: código y constantes  ", (unsigned long)(&_sidata - cm_vector_table) + (&_edata - &_sdata), " bytes");
  linea("RAM: variables con valor    ", &_edata - &_sdata, " bytes (.data)");
  linea("RAM: variables en cero      ", &_ebss - &_sbss, " bytes (.bss)");
  linea("RAM libre al arrancar       ", ramLibre(), " bytes");

  Serial.println("-- 1. const --");
  direccion("tablaFlash (const): ", tablaFlash);
  direccion("tablaRAM (sin const): ", tablaRAM);
  linea("tablaRAM gasta ", sizeof(tablaRAM), " bytes de RAM; tablaFlash 0");

  Serial.println("-- 2. búfer local de 2048 bytes --");
  unsigned long antes = ramLibre();
  unsigned long durante = usarBuferLocal();
  linea("RAM libre antes   ", antes, "");
  linea("RAM libre durante ", durante, "  (2 KB menos solo mientras se usa)");
  linea("RAM libre después ", ramLibre(), "  (se devuelve al salir)");

  Serial.println("-- 3. String frente a char[] --");
  antes = ramLibre();
  {
    String s = "";
    for (int i = 0; i < 50; i++) s += String(i) + ",";   /* crece de a poco */
    linea("String de 140 caracteres: heap usado ", antes - ramLibre(), " bytes");
  }
  linea("Después de liberarlo la RAM libre queda en ", ramLibre(), " (el heap no se encoge)");
  char texto[160];
  int n = 0;
  for (int i = 0; i < 50; i++) n += snprintf(texto + n, sizeof(texto) - n, "%d,", i);
  linea("Con char[160] y snprintf: ", n, " caracteres, 0 bytes de heap");

  Serial.println("Listo. Pruebe también Herramientas > Optimización > Tamaño (-Os).");
}

void loop() {
}
