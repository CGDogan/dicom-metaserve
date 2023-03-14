#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "output_writer.h"

#ifdef SERVER
#include "usr/local/include/fcgi_stdio.h"
#endif

struct output_context_s {
    char * output;
    size_t continue_writing_from;
    size_t allocated;
};

output_context * output_writer_init(void) {
    output_context * oc = (output_context *)malloc(sizeof(output_context));
    oc->allocated = 1000;
    oc->continue_writing_from = 0;
    oc->output = (char *)malloc(oc->allocated * sizeof(char));
    if (!oc->output)
        oc = 0;
    return oc;
}
int output_writer_write(output_context * oc, char *str) {
    unsigned int str_len = strlen(str);
    while ((oc->allocated - oc->continue_writing_from) < str_len) {
        oc->allocated *= 2;
    }
    oc->output = realloc(oc->output, oc->allocated);
    if (!oc->output) {
        return -1;
    }
    strcpy(&oc->output[oc->continue_writing_from], str);
    oc->continue_writing_from += str_len;
    return str_len;
}
char * output_writer_finalize(output_context * oc) {
    char * out = oc->output;
    free(oc);
    return out;
}
