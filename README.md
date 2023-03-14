# DICOM Meta-serve

*The coding challenge for a tile server*

Notes:

- I used C and FastCGI-SDK. The former allows efficient computations and the latter is to match "iipimage". I expect this to be a productive combination for a tiling engine, and I tried to use the same setup here. Hence despite that C with FastCGI for this coding challenge is overkill for only serving metadata, this seems to be a good starting point for a tile engine.
- I used file descriptors here to read a DICOM file. I concluded that for image serving, using mmap will be more efficient, so my plan is to determine where images are located using stream operations first, then using mmap map images without the overhead of syscalls.
- In the final tile server, I'll move "malloc" out of loops and reuse the scratch space rather than to reallocate repeatedly.
- In the final tile server, I'll use the response writer I coded at ./output_writer.c rather than printing inside the reader.

## Usage:

```URL
http://<server>//Users/.../Downloads/file.dcm/read
```

Replace the path with the required path.

Again, this weird syntax is designed for a tile engine that serves from files, otherwise reading the POST-uploaded file's metadata would be preferred.

## Compiling

The metadata reader can be compiler for use with a CLI or as an HTTP server.

The scripts I've been using are adaptible to other systems.

They're fairly short, so please read them and modify them for your systems.

Please see `compile_fcgi_mac.sh` (to create a /usr/loca/... in the project directory), `compile_server.sh` and `restart_server.sh`.

I use `PORT=3500 ./restart_server.sh` for example.