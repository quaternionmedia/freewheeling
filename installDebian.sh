#!/bin/bash

# install script for freewheeling on debian

sudo apt-get build-dep -y freewheeling
aclocal &&\
./configure &&\
autoconf &&\
automake &&\
sudo make install -j4 &&\
echo "Freewheeling installed sucessfully!"
