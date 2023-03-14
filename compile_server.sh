#export CFLAGS="-I./usr/local/include $(CFLAGS) -lfcgi"
#export LDFLAGS="-L./usr/local/lib $(LDLAGS)  -lfcgi"
# -include ./usr/local/include/fcgi_stdio.h
clang -DSERVER output_writer.c reader.c content_parser.c server.c usr/local/lib/libfcgi.a -Wextra -Wall -pedantic -Wno-unused-label -o server.fcgi
clang output_writer.c reader.c content_parser.c local.c -o local.out
