OPTIONS="--toolchain=msvc \
         --arch=x86_64 \
         --enable-yasm  \
         --enable-asm \
         --enable-shared \
         --disable-static \
         --disable-programs \
         --enable-swresample \
         --enable-swscale \
         --enable-libx264 \
         --enable-gpl \
         "
X264_INCLUDE=C:/install/x264/bin/include
X264_LIB=C:/install/x264/bin/lib
 
CC=cl ./configure $OPTIONS --extra-cflags="-I$X264_INCLUDE" --extra-ldflags="-LIBPATH:$X264_LIB" --prefix=./bin
make -j 16
make install
make clean