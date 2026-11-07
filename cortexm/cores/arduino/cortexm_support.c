/*
 * cortexm_support.c - Llamadas al sistema mínimas para newlib-nano,
 * itoa/ltoa/utoa/ultoa que pide la API de Arduino y manejo de fallos.
 */
#include <errno.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdint.h>
#include "cortexm_device.h"

/* ---------- Heap: desde el fin de .bss hasta el límite de la pila ------- */
extern char end;               /* definido por el linker script */
extern char _estack;
extern char _Min_Stack_Size;   /* símbolo absoluto del linker script */
static char *heap_top = &end;

void *_sbrk(ptrdiff_t incr)
{
  char *limit = &_estack - (uint32_t)&_Min_Stack_Size;
  char *prev = heap_top;
  if (heap_top + incr > limit) {
    errno = ENOMEM;
    return (void *)-1;
  }
  heap_top += incr;
  return prev;
}

int  _close(int fd)                     { (void)fd; return -1; }
int  _fstat(int fd, struct stat *st)    { (void)fd; st->st_mode = S_IFCHR; return 0; }
int  _isatty(int fd)                    { (void)fd; return 1; }
int  _lseek(int fd, int p, int d)       { (void)fd; (void)p; (void)d; return 0; }
int  _read(int fd, char *b, int n)      { (void)fd; (void)b; (void)n; return 0; }
int  _write(int fd, const char *b, int n){ (void)fd; (void)b; return n; }
int  _getpid(void)                      { return 1; }
int  _kill(int pid, int sig)            { (void)pid; (void)sig; errno = EINVAL; return -1; }
void _exit(int code)                    { (void)code; for (;;) { __disable_irq(); } }

/* ---------- itoa y familia ---------------------------------------------- */
static char *cm_utoa_core(unsigned long v, char *s, int radix, int neg)
{
  char tmp[33];
  int i = 0;
  if (radix < 2 || radix > 36) { s[0] = 0; return s; }
  do {
    unsigned d = (unsigned)(v % (unsigned)radix);
    tmp[i++] = (char)(d < 10 ? '0' + d : 'a' + d - 10);
    v /= (unsigned)radix;
  } while (v);
  int j = 0;
  if (neg) s[j++] = '-';
  while (i) s[j++] = tmp[--i];
  s[j] = 0;
  return s;
}
char *ultoa(unsigned long v, char *s, int r) { return cm_utoa_core(v, s, r, 0); }
char *utoa(unsigned v, char *s, int r)       { return cm_utoa_core(v, s, r, 0); }
char *ltoa(long v, char *s, int r)
{
  if (r == 10 && v < 0) return cm_utoa_core(0UL - (unsigned long)v, s, r, 1);
  return cm_utoa_core((unsigned long)v, s, r, 0);
}
char *itoa(int v, char *s, int r) { return ltoa(v, s, r); }

