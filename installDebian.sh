#!/bin/bash

# install script for freewheeling on debian

if [[ $(cat /etc/issue) == "Linux Mint 18.3 Sylvia \n \l" ]]; then
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

fi

