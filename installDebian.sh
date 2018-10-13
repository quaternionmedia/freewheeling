#!/bin/bash

# install script for freewheeling on debian

if [ "$EUID" -ne 0 ]; then
	echo "This script must be run as sudo. Please try again"
	exit 1
fi

if [[ $(cat /etc/issue) =~ "Linux Mint 18" ]]; then
	echo "installing freewheeling on linux mint"

	DEPS="libgnutls-dev libfreetype6-dev libsdl-gfx1.2-dev libsdl-ttf2.0-dev liblo-dev libxml2-dev libvorbis-dev libsndfile1-dev libjack-jackd2-dev libfluidsynth2-devel autoconf automake"

	for i in $DEPS; do
		sudo apt-get install -y $i
	done

	./configure &&\
	autoconf &&\
	automake &&\
	sudo make install &&\
	echo "Freewheeling installed sucessfully!"

else
	echo "distro issue does not match: $(cat /etc/issue)"

fi
