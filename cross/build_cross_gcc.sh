export PREFIX="$HOME/repos/PineDOS64/cross/bin"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"

# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH

mkdir build-gcc
cd build-gcc
../gcc/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx
make all-gcc -j 9
make all-target-libgcc -j 9
make all-target-libstdc++-v3 -j 9
make install-gcc -j 9
make install-target-libgcc -j 9
make install-target-libstdc++-v3 -j 9
