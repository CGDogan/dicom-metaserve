/*
 * Modified from sample code as a template
 *
 * Copyright (c) 1996 Open Market, Inc.
 *
 * See the file "LICENSE.TERMS" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */

#include "usr/local/include/fcgi_config.h"
#include <string.h>
#include <stdlib.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

#include "usr/local/include/fcgi_stdio.h"

// cgi-fcgi -connect 127.0.0.1:2017 out.fcgi
// https://blog.petehouston.com/develop-fastcgi-application-with-c/
/*
events{}
http {
server {
            listen       9999;
    location / {
        fastcgi_pass 127.0.0.1:2018;
        include fastcgi_params;
    }
}
}
in /opt/homebrew/etc/nginx/nginx.conf

but first: cgi-fcgi -connect 127.0.0.1:2018 out.fcgi
and update port in nginx conf
then brew services start nginx
*/

#include "reader.h"

int main(void)
{
    while (FCGI_Accept() >= 0) {
        printf("Status: 200 OK\r\nContent-type: text/plain; charset=utf-8\r\n\r\n");
        const char *out = "Error";
        const char *original_url = getenv("REQUEST_URI");
        if (!original_url)
            // Not a real browser visit
            continue;

        // Note: Must run before any goto!
        // Note: Must set to 0 after every free!
        FILE *fd = 0;
        char *url = 0;

        int url_len = strlen(original_url);
        // TODO: Can be optimized by moving out of loop, as well as delaying copying
        url = (char *) malloc((url_len + 1) * sizeof(char));
        strcpy(url, original_url);
    
        char * upper_bound = strstr(url, "/read");
        if (!upper_bound) {
            out = "No file path specified. Expected URL format: \"http://<domain>/<path>/read\" eg http://localhost/a.dcm/read in current dir or http://localhost//Users/.../a.dcm/read";
            goto show;
        }
        *upper_bound = 0;
        printf("Path: %s\n", &url[1]);

        fd = fopen(&url[1], "rb");
        free(url);
        url = 0;
        if (!fd) {
            out = "Could not open";
            goto show;
        }
        // parse and set the output
        out = reader(fd);

        show:
        printf("%s\n",
               out);
        fflush(stdout);
        if (fd) {
            fclose(fd);
            fd = 0;
        }
        if (url) {
            free(url);
            url = 0;
        }
    }
    return 0;
}
