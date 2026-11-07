// air105load - carga programas en el Air105 (MegaHunt MH1903S) por el
// puerto serie, usando el cargador de su ROM. Sin dependencias: un solo
// ejecutable para Windows y Linux (lo usa el núcleo Arduino-Arm-Cortex-M).
//
//	air105load [-v] [-b 115200] [-a 0x1001000] -p PUERTO programa.bin
//
// Protocolo (tomado de air105-uploader de racerxdl, Apache-2.0, que a su vez
// se obtuvo del cargador oficial de MegaHunt):
//   - RTS corta la alimentación del chip en la placa CORE-Air105: RTS activo
//     = apagado. Al soltarlo se envía 0xF8 sin parar hasta que la ROM
//     contesta con su número de serie (paquete 0x81); luego 0x7C (0x82).
//   - Paquete: 0x02, orden, largo (16 bits LE), datos, CRC-16/CCITT-FALSE (LE).
//   - Cabecera del programa (orden 0x20): 0x5555AAAA, 0, dirección, largo,
//     versión 0, opción 2, SHA-256 (64 bytes con ceros), CRC-32 de lo anterior
//     sin la marca. La ROM la guarda en 0x01000000 y la verifica al arrancar.
//   - Borrado (0x22): dirección 0, sectores, 4096. Datos (0x21): dirección
//     absoluta (LE) + 4096 bytes. Cada orden se contesta con 0x80 (ACK);
//     si el primer byte es ')' es un error.
package main

import (
	"crypto/sha256"
	"encoding/binary"
	"flag"
	"fmt"
	"hash/crc32"
	"os"
	"strconv"
	"time"
)

const (
	cFWHeader = 0x20
	cFWData   = 0x21
	cErase    = 0x22
	cFlashID  = 0x23
	cAck      = 0x80
	cChipSN   = 0x81
	cDeviceSN = 0x82
	cFlashRes = 0xA3
	sector    = 4096
)

var verboso bool

func fallo(f string, a ...interface{}) {
	fmt.Fprintf(os.Stderr, "air105load: "+f+"\n", a...)
	os.Exit(1)
}

func info(f string, a ...interface{}) { fmt.Printf(f+"\n", a...) }

func detalle(f string, a ...interface{}) {
	if verboso {
		fmt.Printf("  "+f+"\n", a...)
	}
}

// CRC-16/CCITT-FALSE: polinomio 0x1021, inicio 0xFFFF, sin reflejar
func crc16(b []byte) uint16 {
	c := uint16(0xFFFF)
	for _, x := range b {
		c ^= uint16(x) << 8
		for i := 0; i < 8; i++ {
			if c&0x8000 != 0 {
				c = c<<1 ^ 0x1021
			} else {
				c <<= 1
			}
		}
	}
	return c
}

func paquete(orden byte, datos []byte) []byte {
	b := []byte{0x02, orden, byte(len(datos)), byte(len(datos) >> 8)}
	b = append(b, datos...)
	c := crc16(b)
	return append(b, byte(c), byte(c>>8))
}

type rom struct {
	p   *puerto
	buf []byte
}

// recibir: siguiente paquete válido en 'plazo' (descarta basura y CRC malos)
func (r *rom) recibir(plazo time.Duration) (byte, []byte, bool) {
	fin := time.Now().Add(plazo)
	tmp := make([]byte, 1024)
	for {
		for len(r.buf) > 0 && r.buf[0] != 0x02 {
			r.buf = r.buf[1:]
		}
		if len(r.buf) >= 4 {
			n := int(r.buf[2]) | int(r.buf[3])<<8
			if n > 2048 {
				r.buf = r.buf[1:]
				continue
			}
			if len(r.buf) >= n+6 {
				pk := r.buf[:n+6]
				if crc16(pk[:n+4]) == binary.LittleEndian.Uint16(pk[n+4:]) {
					orden := pk[1]
					datos := append([]byte(nil), pk[4:4+n]...)
					r.buf = r.buf[n+6:]
					return orden, datos, true
				}
				r.buf = r.buf[1:]
				continue
			}
		}
		resta := time.Until(fin)
		if resta <= 0 {
			return 0, nil, false
		}
		if resta > 20*time.Millisecond {
			resta = 20 * time.Millisecond
		}
		k := r.p.leer(tmp, resta)
		r.buf = append(r.buf, tmp[:k]...)
	}
}

func (r *rom) enviar(orden byte, datos []byte) {
	if err := r.p.escribir(paquete(orden, datos)); err != nil {
		fallo("no se pudo escribir en el puerto: %v", err)
	}
}

// orden con respuesta ACK; error si contesta ')' o no contesta
func (r *rom) ordenAck(orden byte, datos []byte, plazo time.Duration) error {
	r.enviar(orden, datos)
	fin := time.Now().Add(plazo)
	for time.Now().Before(fin) {
		c, d, ok := r.recibir(time.Until(fin))
		if !ok {
			break
		}
		if c != cAck {
			detalle("respuesta inesperada 0x%02X", c)
			continue
		}
		if len(d) > 0 && d[0] == ')' {
			return fmt.Errorf("la ROM rechazó la orden 0x%02X (%q)", orden, d)
		}
		return nil
	}
	return fmt.Errorf("la ROM no contestó a la orden 0x%02X", orden)
}

// alimentacion: RTS activo = placa apagada (CORE-Air105)
func (r *rom) alimentacion(encendida bool) { r.p.lineas(true, !encendida) }

func (r *rom) conectar() {
	r.alimentacion(false)
	time.Sleep(200 * time.Millisecond)
	r.p.vaciarEntrada()
	r.buf = nil
	r.alimentacion(true)
	f8 := make([]byte, 64)
	for i := range f8 {
		f8[i] = 0xF8
	}
	inicio := time.Now()
	for {
		r.p.escribir(f8)
		c, d, ok := r.recibir(100 * time.Millisecond)
		if ok && c == cChipSN {
			if len(d) >= 37 && d[29] >= '0' && d[29] <= 'z' {
				detalle("chip MH%s", string([]byte{d[32], d[31], d[30], d[29]})) // "S030" al revés: MH1903S
			}
			break
		}
		if time.Since(inicio) > 10*time.Second {
			fallo("el Air105 no contesta: revise el puerto, que la placa sea CORE-Air105 (RTS reinicia el chip) y que el Monitor Serie esté cerrado")
		}
	}
	// etapa 2
	time.Sleep(20 * time.Millisecond)
	r.p.vaciarEntrada()
	r.buf = nil
	x := make([]byte, 16)
	for i := range x {
		x[i] = 0x7C
	}
	r.p.escribir(x)
	inicio = time.Now()
	for {
		c, _, ok := r.recibir(time.Second)
		if ok && c == cDeviceSN {
			break
		}
		if !ok || time.Since(inicio) > 2*time.Second {
			fallo("la ROM no pasó a modo de programación (sin respuesta a 0x7C)")
		}
	}
}

func (r *rom) idFlash() {
	r.enviar(cFlashID, nil)
	c, d, ok := r.recibir(time.Second)
	if !ok || c != cFlashRes || len(d) < 3 {
		fallo("no se pudo leer la identificación de la flash")
	}
	tam := d[0]
	if tam >= 0x13 && tam < 0x20 {
		detalle("flash: fabricante 0x%02X, %d KB", d[2], (1<<tam)/1024)
	}
}

func cabecera(dir uint32, img []byte) []byte {
	h := make([]byte, 24, 24+64+4)
	binary.LittleEndian.PutUint32(h[0:], 0x5555AAAA)
	binary.LittleEndian.PutUint32(h[4:], 0)
	binary.LittleEndian.PutUint32(h[8:], dir)
	binary.LittleEndian.PutUint32(h[12:], uint32(len(img)))
	binary.LittleEndian.PutUint32(h[16:], 0) // versión
	binary.LittleEndian.PutUint32(h[20:], 2) // opción 2: SHA-256
	s := sha256.Sum256(img)
	hash := make([]byte, 64)
	copy(hash, s[:])
	h = append(h, hash...)
	c := crc32.ChecksumIEEE(h[4:])
	return binary.LittleEndian.AppendUint32(h, c)
}

func main() {
	nombrePuerto := flag.String("p", "", "puerto serie (COM5, /dev/ttyUSB0...)")
	baud := flag.Int("b", 115200, "velocidad")
	dirTexto := flag.String("a", "0x1001000", "dirección donde se graba el programa")
	flag.BoolVar(&verboso, "v", false, "mostrar detalles")
	flag.Parse()
	if *nombrePuerto == "" || flag.NArg() != 1 {
		fmt.Fprintln(os.Stderr, "uso: air105load [-v] [-b 115200] [-a 0x1001000] -p PUERTO programa.bin")
		os.Exit(2)
	}
	dir64, err := strconv.ParseUint(*dirTexto, 0, 32)
	if err != nil {
		fallo("dirección no válida: %s", *dirTexto)
	}
	dir := uint32(dir64)

	img, err := os.ReadFile(flag.Arg(0))
	if err != nil {
		fallo("%v", err)
	}
	if len(img) == 0 {
		fallo("el programa está vacío")
	}
	if r := len(img) % sector; r != 0 {
		img = append(img, make([]byte, sector-r)...)
	}

	p, err := abrirPuerto(*nombrePuerto, *baud, false)
	if err != nil {
		fallo("no se pudo abrir %s: %v", *nombrePuerto, err)
	}
	defer p.cerrar()
	r := &rom{p: p}

	info("Conectando con el Air105 en %s...", *nombrePuerto)
	r.conectar()
	r.idFlash()

	if err := r.ordenAck(cFWHeader, cabecera(dir, img), 2*time.Second); err != nil {
		fallo("cabecera: %v", err)
	}
	sectores := len(img)/sector + 2
	info("Borrando %d KB...", sectores*4)
	borrar := make([]byte, 12)
	binary.LittleEndian.PutUint32(borrar[0:], 0)
	binary.LittleEndian.PutUint32(borrar[4:], uint32(sectores))
	binary.LittleEndian.PutUint32(borrar[8:], sector)
	if err := r.ordenAck(cErase, borrar, 2*time.Second+time.Duration(sectores)*60*time.Millisecond); err != nil {
		fallo("borrado: %v", err)
	}

	info("Grabando %d bytes en 0x%08X...", len(img), dir)
	inicio := time.Now()
	paso := 25
	for off := 0; off < len(img); off += sector {
		d := make([]byte, 4, 4+sector)
		binary.LittleEndian.PutUint32(d, dir+uint32(off))
		d = append(d, img[off:off+sector]...)
		var err error
		for intento := 0; intento < 5; intento++ {
			if err = r.ordenAck(cFWData, d, 3*time.Second); err == nil {
				break
			}
			detalle("reintento %d en 0x%08X: %v", intento+1, dir+uint32(off), err)
			time.Sleep(100 * time.Millisecond)
		}
		if err != nil {
			fallo("grabando 0x%08X: %v", dir+uint32(off), err)
		}
		if pc := (off + sector) * 100 / len(img); pc >= paso && pc < 100 {
			info("  %d%%", pc)
			for paso <= pc {
				paso += 25
			}
		}
	}
	info("  100%% en %.1f s", time.Since(inicio).Seconds())

	// reinicio: corta y vuelve a dar alimentación; el programa arranca solo
	r.alimentacion(false)
	time.Sleep(100 * time.Millisecond)
	r.alimentacion(true)
	info("Listo: el programa ya está corriendo.")
}
