#!/bin/bash
##
## This file is part of the sigrok-util project.
##
## Copyright (C) 2013-2014 Uwe Hermann <uwe@hermann-uwe.de>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
##

set -e

# Build target: "i686" (32bit) or "x86_64" (64bit).
TARGET="i686"

#git clone https://github.com/mxe/mxe.git
#make build-requirements -j 16
#make MXE_TARGETS=i686-w64-mingw32.static gcc wxwidgets -j16

# The path where your MXE directory is located.
MXE=$HOME/dev/mxe

# The path where the cross-compiled packages will be installed.
PREFIX=$HOME/dev/stmbl/term/win32/

# The path where to download files to and where to build packages.
BUILDDIR=$HOME/dev/stmbl/term/win32/build

# Edit this to enable/disable/modify parallel compiles.
PARALLEL="-j 2"

# You usually don't need to change anything below this line.

# -----------------------------------------------------------------------------

VER_LIBUSB_WIN32=1.2.6.0
VER_ZADIG=v2.0.1.160

SF_MIRROR=switch.dl.sourceforge.net

WGET="wget --quiet"
GIT_CLONE="git clone --depth=1"

# -----------------------------------------------------------------------------

# We need to find tools in the toolchain.
export PATH=$MXE/usr/bin:$PATH

TOOLCHAIN_TRIPLET="$TARGET-w64-mingw32.static"

P="$PREFIX/lib/pkgconfig"
P2="$MXE/usr/$TOOLCHAIN_TRIPLET/lib/pkgconfig"
C="--host=$TOOLCHAIN_TRIPLET --prefix=$PREFIX"
CM="-DCMAKE_TOOLCHAIN_FILE=$MXE/usr/$TOOLCHAIN_TRIPLET/share/cmake/mxe-conf.cmake"
L="--disable-shared --enable-static"

if [ $TARGET == "i686" ]; then
	export PKG_CONFIG_PATH_i686_w64_mingw32_static="$P:$P2"
else
	export PKG_CONFIG_PATH_x86_64_w64_mingw32_static="$P:$P2"
fi

# Remove build directory contents (if any) and create a new build dir.
rm -rf $BUILDDIR
mkdir -p $BUILDDIR
cd $BUILDDIR

# -----------------------------------------------------------------------------

mkdir -p $PREFIX

# libserialport
$GIT_CLONE git://sigrok.org/libserialport
cd libserialport
./autogen.sh
./configure $C $L
make $PARALLEL V=1
make install
cd ..

# Servoterm
mkdir term
cd term
cmake $CM -DCMAKE_INSTALL_PREFIX:PATH=$PREFIX -DDISABLE_WERROR=y ../../../
make $PARALLEL VERBOSE=1
make install
#makensis -DHOME=$HOME contrib/pulseview_cross.nsi
cd ..

rm -rf $BUILDDIR
rm -rf $PREFIX/lib $PREFIX/include
