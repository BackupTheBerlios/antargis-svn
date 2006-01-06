#!/bin/bash
aclocal
automake -a -c
autoconf
automake -a -c
autoconf
automake
./configure --prefix=/usr --enable-debug

