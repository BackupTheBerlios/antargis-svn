#!/bin/bash
aclocal
automake -a
autoconf
automake -a
autoconf
automake
./configure --prefix=/usr --enable-debug

