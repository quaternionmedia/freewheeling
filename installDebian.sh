#!/bin/bash

# install script for freewheeling on debian

if [[ $(cat /etc/issue) =~ "Linux Mint" ]]; then
	echo "installing freewheeling on linux mint"

	DEPS="libgnutls-dev libfreetype6-dev libsdl-gfx1.2-dev libsdl-ttf2.0-dev liblo-dev libxml2-dev libvorbis-dev libsndfile1-dev libjack-jackd2-dev libfluidsynth2-devel autoconf automake nettle-dev"

	for i in $DEPS; do
		sudo apt-get install -y $i
	done

	aclocal &&\
	./configure &&\
	autoconf &&\
	automake &&\
	sudo make install &&\
	echo "Freewheeling installed sucessfully!"

else
	echo "distro issue does not match: $(cat /etc/issue)"

fi
