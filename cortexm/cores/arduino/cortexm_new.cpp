/* Operadores new/delete ligeros (sin excepciones) y soporte C++ mínimo */
#include <stdlib.h>
#include <stddef.h>

void *operator new(size_t n)            { return malloc(n); }
void *operator new[](size_t n)          { return malloc(n); }
void  operator delete(void *p)          noexcept { free(p); }
void  operator delete[](void *p)        noexcept { free(p); }
void  operator delete(void *p, size_t)  noexcept { free(p); }
void  operator delete[](void *p, size_t) noexcept { free(p); }

extern "C" void __cxa_pure_virtual(void) { for (;;) { } }

namespace __gnu_cxx {
  void __verbose_terminate_handler() { for (;;) { } }
}
