/*
 * Wire.h - I2C maestro por software para el núcleo Arduino-Arm-Cortex-M.
 *
 * El bus se genera por software en cualquier par de pines, igual en las 12
 * placas (los controladores I2C de cada fabricante son distintos entre sí y
 * algunos tienen erratas). Pines por defecto: PIN_WIRE_SDA y PIN_WIRE_SCL
 * de la variante; se cambian con Wire.setPins(sda, scl) antes de begin().
 * Velocidad: 100 kHz por defecto; setClock(400000) da ~300-400 kHz.
 *
 * Las líneas son de drenaje abierto: hacen falta pull-ups de 4.7 kOhm a
 * 3.3 V (casi todos los módulos I2C ya las traen).
 *
 * Solo modo maestro: begin(direccion), onReceive() y onRequest() se aceptan
 * por compatibilidad pero el modo esclavo no esta implementado.
 */
#ifndef CM_WIRE_H
#define CM_WIRE_H

#include <Arduino.h>
#include "api/HardwareI2C.h"

#ifndef WIRE_BUFFER_LENGTH
#define WIRE_BUFFER_LENGTH 64
#endif
#define BUFFER_LENGTH WIRE_BUFFER_LENGTH

class TwoWire : public arduino::HardwareI2C
{
public:
    TwoWire(pin_size_t sda, pin_size_t scl);

    void setPins(pin_size_t sda, pin_size_t scl);
    void begin() override;
    void begin(uint8_t address) override;
    void end() override;
    void setClock(uint32_t freq) override;
    void setTimeout(uint32_t us) { _timeout_us = us; }

    void beginTransmission(uint8_t address) override;
    uint8_t endTransmission(bool stopBit) override;
    uint8_t endTransmission(void) override { return endTransmission(true); }

    size_t requestFrom(uint8_t address, size_t len, bool stopBit) override;
    size_t requestFrom(uint8_t address, size_t len) override { return requestFrom(address, len, true); }

    size_t write(uint8_t data) override;
    size_t write(const uint8_t *data, size_t len) override;
    using Print::write;

    int available(void) override;
    int read(void) override;
    int peek(void) override;
    void flush(void) override { }

    void onReceive(void (*cb)(int)) override { _onReceive = cb; }
    void onRequest(void (*cb)(void)) override { _onRequest = cb; }

private:
    /* primitivas del bus */
    inline void sdaLow(void);
    inline void sdaRelease(void);
    inline void sclLow(void);
    bool sclRelease(void);          /* espera clock stretching */
    inline bool sdaRead(void);
    inline bool sclRead(void);
    inline void halfDelay(void);
    bool start(void);
    void stop(void);
    bool writeByte(uint8_t b);      /* true = ACK */
    uint8_t readByte(bool ack);

    pin_size_t _sda, _scl;
    uint16_t   _half_us;
    uint32_t   _timeout_us;
    bool       _busHeld;            /* sin STOP tras la ultima transaccion */
    bool       _timedOut;

    uint8_t _txAddr;
    uint8_t _txBuf[WIRE_BUFFER_LENGTH];
    size_t  _txLen;
    bool    _txActive;
    uint8_t _rxBuf[WIRE_BUFFER_LENGTH];
    size_t  _rxLen, _rxPos;

    void (*_onReceive)(int);
    void (*_onRequest)(void);
};

extern TwoWire Wire;

#endif
