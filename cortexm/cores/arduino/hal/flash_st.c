/*
 * flash_st.c - Borrado y escritura de la flash interna (EEPROM emulada).
 *
 *  F4 (STM32F4, GD32F4)  sectores grandes; escritura por palabras (32 bits)
 *  L4/G4/WB              páginas de 2/4 KB; escritura por dobles palabras (64 bits)
 *  H5                    sectores de 8 KB; escritura por cuádruples palabras (128 bits)
 *  AT32                  driver oficial de Artery (palabras)
 *
 * La CPU se detiene mientras la flash está ocupada (el código corre desde
 * la misma flash); las interrupciones esperan. Un borrado de un sector de
 * 128 KB del F4 tarda ~1-2 s.
 */
#include "Arduino.h"

#if defined(CM_VENDOR_ST) || defined(CM_VENDOR_GD) || defined(CM_VENDOR_AT)

#define KEY1 0x45670123UL
#define KEY2 0xCDEF89ABUL

#if defined(CM_VENDOR_AT)
/* ------------------------------------------------------------- Artery */
bool cmFlashErase(uint32_t addr)
{
  flash_unlock();
  flash_status_type s = flash_sector_erase(addr);
  flash_lock();
  return s == FLASH_OPERATE_DONE;
}

bool cmFlashWrite(uint32_t addr, const void *data, uint32_t len)
{
  const uint8_t *p = (const uint8_t *)data;
  bool ok = true;
  flash_unlock();
  for (uint32_t i = 0; i + 4u <= len && ok; i += 4u) {
    uint32_t w;
    memcpy(&w, p + i, 4);
    ok = flash_word_program(addr + i, w) == FLASH_OPERATE_DONE;
  }
  flash_lock();
  return ok;
}

#elif defined(STM32F4xx) || defined(CM_VENDOR_GD)
/* ---------------------------------- STM32F4 y GD32F4 (mismos registros) */
typedef struct { volatile uint32_t ACR, KEYR, OPTKEYR, SR, CR; } cm_flash_t;
#define FL        ((cm_flash_t *)0x40023C00UL)
#define CR_PG     (1UL << 0)
#define CR_SER    (1UL << 1)
#define CR_PSIZE  (2UL << 8)       /* 32 bits (VDD 2.7-3.6 V) */
#define CR_STRT   (1UL << 16)
#define CR_LOCK   (1UL << 31)
#define SR_ERR    0xF2UL           /* OPERR WRPERR PGAERR PGPERR PGSERR */
#define SR_BSY    (1UL << 16)

static void fl_unlock(void)
{
  if (FL->CR & CR_LOCK) { FL->KEYR = KEY1; FL->KEYR = KEY2; }
  FL->SR = SR_ERR | 1u;
}

static bool fl_wait(void)
{
  while (FL->SR & SR_BSY) { }
  return (FL->SR & SR_ERR) == 0;
}

static void fl_cache_reset(void)
{
#if defined(FLASH_ACR_DCRST)
  if (FL->ACR & FLASH_ACR_DCEN) {
    FL->ACR &= ~FLASH_ACR_DCEN;
    FL->ACR |= FLASH_ACR_DCRST;
    FL->ACR &= ~FLASH_ACR_DCRST;
    FL->ACR |= FLASH_ACR_DCEN;
  }
#endif
}

bool cmFlashErase(uint32_t addr)
{
  (void)addr;                               /* un solo sector reservado: CM_EEPROM_SECTOR */
  fl_unlock();
  FL->CR = CR_SER | CR_PSIZE | ((uint32_t)CM_EEPROM_SECTOR << 3);
  FL->CR |= CR_STRT;
  bool ok = fl_wait();
  FL->CR = CR_LOCK;
  fl_cache_reset();
  return ok;
}

bool cmFlashWrite(uint32_t addr, const void *data, uint32_t len)
{
  const uint8_t *p = (const uint8_t *)data;
  bool ok = true;
  fl_unlock();
  FL->CR = CR_PG | CR_PSIZE;
  for (uint32_t i = 0; i + 4u <= len && ok; i += 4u) {
    uint32_t w;
    memcpy(&w, p + i, 4);
    *(volatile uint32_t *)(addr + i) = w;
    ok = fl_wait();
  }
  FL->CR = CR_LOCK;
  fl_cache_reset();
  return ok;
}

#elif defined(STM32H5xx)
/* ---------------------------------------------------------------- H5 */
#define H5_ERR  (FLASH_SR_WRPERR | FLASH_SR_PGSERR | FLASH_SR_STRBERR | FLASH_SR_INCERR | FLASH_SR_OPTCHANGEERR)

static void fl_unlock(void)
{
  if (FLASH->NSCR & FLASH_CR_LOCK) { FLASH->NSKEYR = KEY1; FLASH->NSKEYR = KEY2; }
  FLASH->NSCCR = H5_ERR | FLASH_SR_EOP;
}

static bool fl_wait(void)
{
  while (FLASH->NSSR & (FLASH_SR_BSY | FLASH_SR_WBNE | FLASH_SR_DBNE)) { }
  bool ok = (FLASH->NSSR & H5_ERR) == 0;
  FLASH->NSCCR = H5_ERR | FLASH_SR_EOP;
  return ok;
}

static void fl_icache_inv(void)
{
  if (ICACHE->CR & ICACHE_CR_EN) {
    ICACHE->CR |= ICACHE_CR_CACHEINV;
    while (ICACHE->SR & ICACHE_SR_BUSYF) { }
  }
}

bool cmFlashErase(uint32_t addr)
{
  uint32_t off = addr - FLASH_BASE;
  uint32_t bank = off / FLASH_BANK_SIZE, sect = (off % FLASH_BANK_SIZE) / FLASH_SECTOR_SIZE;
  fl_unlock();
  FLASH->NSCR = FLASH_CR_SER | (sect << FLASH_CR_SNB_Pos) | (bank ? FLASH_CR_BKSEL : 0u);
  FLASH->NSCR |= FLASH_CR_START;
  bool ok = fl_wait();
  FLASH->NSCR = FLASH_CR_LOCK;
  fl_icache_inv();
  return ok;
}

bool cmFlashWrite(uint32_t addr, const void *data, uint32_t len)
{
  const uint8_t *p = (const uint8_t *)data;
  bool ok = true;
  fl_unlock();
  FLASH->NSCR = FLASH_CR_PG;
  for (uint32_t i = 0; i + 16u <= len && ok; i += 16u) {
    uint32_t w[4];
    memcpy(w, p + i, 16);
    volatile uint32_t *d = (volatile uint32_t *)(addr + i);
    d[0] = w[0]; d[1] = w[1]; d[2] = w[2]; d[3] = w[3];
    ok = fl_wait();
  }
  FLASH->NSCR = FLASH_CR_LOCK;
  fl_icache_inv();
  return ok;
}

#else
/* --------------------------------------------------------- L4, G4, WB */
#define SR_ERR  (FLASH_SR_OPERR | FLASH_SR_PROGERR | FLASH_SR_WRPERR | FLASH_SR_PGAERR | \
                 FLASH_SR_SIZERR | FLASH_SR_PGSERR | FLASH_SR_MISERR | FLASH_SR_FASTERR)
#if defined(FLASH_SR_CFGBSY)
  #define SR_BUSY (FLASH_SR_BSY | FLASH_SR_CFGBSY)
#else
  #define SR_BUSY FLASH_SR_BSY
#endif
#if defined(STM32WBxx)
  #define PAGE_SIZE_ 0x1000UL
#else
  #define PAGE_SIZE_ 0x800UL
#endif

static void fl_unlock(void)
{
  while (FLASH->SR & SR_BUSY) { }
  if (FLASH->CR & FLASH_CR_LOCK) { FLASH->KEYR = KEY1; FLASH->KEYR = KEY2; }
  FLASH->SR = SR_ERR | FLASH_SR_EOP;
}

static bool fl_wait(void)
{
  while (FLASH->SR & SR_BUSY) { }
  bool ok = (FLASH->SR & SR_ERR) == 0;
  FLASH->SR = SR_ERR | FLASH_SR_EOP;
  return ok;
}

static void fl_cache_reset(void)
{
  if (FLASH->ACR & FLASH_ACR_DCEN) {
    FLASH->ACR &= ~FLASH_ACR_DCEN;
    FLASH->ACR |= FLASH_ACR_DCRST;
    FLASH->ACR &= ~FLASH_ACR_DCRST;
    FLASH->ACR |= FLASH_ACR_DCEN;
  }
}

bool cmFlashErase(uint32_t addr)
{
  uint32_t page = (addr - FLASH_BASE) / PAGE_SIZE_;
  fl_unlock();
  FLASH->CR = FLASH_CR_PER | (page << FLASH_CR_PNB_Pos);
  FLASH->CR |= FLASH_CR_STRT;
  bool ok = fl_wait();
  FLASH->CR = FLASH_CR_LOCK;
  fl_cache_reset();
  return ok;
}

bool cmFlashWrite(uint32_t addr, const void *data, uint32_t len)
{
  const uint8_t *p = (const uint8_t *)data;
  bool ok = true;
  fl_unlock();
  FLASH->CR = FLASH_CR_PG;
  for (uint32_t i = 0; i + 8u <= len && ok; i += 8u) {
    uint32_t w[2];
    memcpy(w, p + i, 8);
    volatile uint32_t *d = (volatile uint32_t *)(addr + i);
    d[0] = w[0];
    __ISB();
    d[1] = w[1];
    ok = fl_wait();
  }
  FLASH->CR = FLASH_CR_LOCK;
  fl_cache_reset();
  return ok;
}
#endif

#endif
