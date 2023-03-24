# DICOM Meta-serve

*The coding challenge for a tile server*

Notes:

- I used C and FastCGI-SDK. The former allows efficient computations and the latter is to match "iipimage". I expect this to be a productive combination for a tiling engine, and I tried to use the same setup here. Hence despite that C with FastCGI for this coding challenge is overkill for only serving metadata, this seems to be a good starting point for a tile engine.
- I used file descriptors here to read a DICOM file. I concluded that for image serving, using mmap will be more efficient, so my plan is to determine where images are located using stream operations first, then to use mmap to map images without the overhead of syscalls.
- In the final tile server, I'll move "malloc" out of loops and reuse the scratch space rather than to reallocate repeatedly.
- In the final tile server, I'll use the response writer I coded at ./output_writer.c rather than printing inside the reader.

## Usage:

```URL
http://<server>//Users/.../Downloads/file.dcm/read
```

Replace the path with the required path.

Again, this weird syntax is designed for a tile engine that serves from files, otherwise reading the POST-uploaded file's metadata would be preferred.

## Compiling

First see `compile_fcgi_mac.sh`, and adapt it for your needs to compile the local FCGI SDK in the project directory:

```sh
export LDFLAGS="-L/opt/homebrew/opt/jpeg/lib -L/opt/homebrew/opt/libtiff/lib -L/opt/homebrew/opt/openslide/lib/ -L/opt/homebrew/opt/openjpeg/lib"
export CPPFLAGS="-I/opt/homebrew/opt/jpeg/include -I/opt/homebrew/opt/libtiff/include -I/opt/homebrew/opt/openslide/include/openslide/"
cd fcgi
./autogen.sh
./configure
export DESTDIR="$(pwd)/.."
make
make install
cd ..
```

Compile the project:

```sh
clang -DSERVER output_writer.c reader.c content_parser.c server.c usr/local/lib/libfcgi.a -o server.fcgi
clang output_writer.c reader.c content_parser.c local.c -o local.out
```

Then restart nginx:

```sh
brew services stop nginx
cgi-fcgi -connect "127.0.0.1:3000" server.fcgi
echo "events{} http { server {listen 9999; location / { fastcgi_pass 127.0.0.1:3000; include fastcgi_params; } } }" > /opt/homebrew/etc/nginx/nginx.conf
brew services run nginx
```

This is for connecting to it through a browser.

It can also be run as a CLI tool: `./out /path/to/dicom-file.dcm`.
