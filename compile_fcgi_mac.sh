export LDFLAGS="-L/opt/homebrew/opt/jpeg/lib -L/opt/homebrew/opt/libtiff/lib -L/opt/homebrew/opt/openslide/lib/ -L/opt/homebrew/opt/openjpeg/lib"
export CPPFLAGS="-I/opt/homebrew/opt/jpeg/include -I/opt/homebrew/opt/libtiff/include -I/opt/homebrew/opt/openslide/include/openslide/"
cd fcgi
#./autogen.sh
./configure
export DESTDIR="$(pwd)/.."
make
make install