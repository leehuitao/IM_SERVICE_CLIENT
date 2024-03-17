OPTIONS="--enable-shared"
 
CC=cl ./configure $OPTIONS --enable-shared --prefix=./bin
 
make -j 16
make install
make clean