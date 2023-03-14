#include <stddef.h>

typedef struct output_context_s output_context;

output_context * output_writer_init(void);
int output_writer_write(output_context *oc, char * str);

char * output_writer_finalize(output_context * oc);
