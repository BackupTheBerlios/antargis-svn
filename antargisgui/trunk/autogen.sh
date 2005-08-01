#!/bin/bash
automake -a
autoconf
aclocal
automake -a
autoconf
aclocal
./configure --prefix=/usr --enable-debug

