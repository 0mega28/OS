#!/bin/bash
# Build cross-compiler for i686
# Original author of script https://github.com/acmiitr/KSOS/blob/master/script.sh

sudo apt install build-essential bison flex libgmp3-dev libmpc-dev \
				libmpfr-dev texinfo 

export TARGET=i686-elf
export PREFIX="$HOME/opt/cross"
export PATH="$PREFIX/bin:$PATH"

echo "Building OS tools. Patience......."

mkdir -p /tmp/src
cd /tmp/src

wget "https://ftp.gnu.org/gnu/binutils/binutils-2.34.tar.gz"
tar xvfz binutils-2.34.tar.gz
rm binutils-2.34.tar.gz

mkdir binutils-build
cd binutils-build
../binutils-2.34/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j4
make -j4 install

cd /tmp/src

wget "https://ftp.gnu.org/gnu/gcc/gcc-7.5.0/gcc-7.5.0.tar.gz"
tar xvfz gcc-7.5.0.tar.gz

rm gcc-7.5.0.tar.gz

which -- $TARGET-as || echo $TARGET-as is not in the PATH
mkdir gcc-build
cd gcc-build
../gcc-7.5.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers

make -j4 all-gcc
make -j4 all-target-libgcc
make -j4 install-gcc
make -j4 install-target-libgcc

echo -e "\nexport PATH=\"\$PATH:\$HOME/opt/cross/bin\"" >> $HOME/.bashrc
