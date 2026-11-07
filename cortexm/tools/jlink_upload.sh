#!/bin/sh
# jlink_upload.sh <JLinkExe> <dispositivo> <kHz> <archivo.hex>
# Genera un script de J-Link Commander, programa, verifica y reinicia.
JLINK="$1"; DEV="$2"; SPEED="$3"; HEX="$4"
SCRIPT="$(dirname "$HEX")/jlink_upload.jlink"
cat > "$SCRIPT" <<EOS
r
h
loadfile "$HEX"
r
g
qc
EOS
"$JLINK" -device "$DEV" -if SWD -speed "$SPEED" -autoconnect 1 -NoGui 1 -ExitOnError 1 -CommandFile "$SCRIPT"
