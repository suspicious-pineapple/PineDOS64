export PREFIX="$HOME/repos/PineDOS64/cross/bin"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"


cd binutils-gdb
mkdir build
cd build
../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j 8
make install -j 8
