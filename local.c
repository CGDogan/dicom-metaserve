#include "reader.h"
#include "output_writer.h"
#include <stdlib.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s path/to/filename.dcm", argv[0]);
        return 1;
    }
    FILE * fd = fopen(argv[1], "rb");
    if (!fd) {
        printf("Could not open %s", argv[1]);
        return 1;
    }
    const char *outjson = reader(fd);

    printf("%s", outjson);
}
