#!/bin/bash
$FONT_DIR=/usr/local/share/fweelin/truetype/ttf-bitstream-vera
sudo mkdir -p $FONT_DIR
sudo cp $(dirname "$0")/../data/vera.ttf $FONT_DIR/Vera.ttf
sudo cp $(dirname "$0")/../data/verabd.ttf $FONT_DIR/VeraBd.ttf
