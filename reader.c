#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "output_writer.h"
#include "content_parser.h"

#ifdef SERVER
#include "usr/local/include/fcgi_stdio.h"
#endif

char * reader(FILE * f) {
    // Get file size
    fseek(f, 0, SEEK_END);
    size_t size = (long unsigned int) ftell(f);
    fseek(f, 0, SEEK_SET);

    printf("File Size: %zu\n", size);

    char * preamble = malloc(129 * sizeof(char));
    unsigned int s;
    const char * err;
    s = fread(preamble, 1, 128, f);
    if (s < 128) {
        err = "Too short!";
        goto invalid;
    }

    for (int i = 0; i < 127; i++)
        if (preamble[i] < 32 || preamble[i] > 126)
            preamble[i] = ' ';

    preamble[128] = 0;

    printf("Preamble text (if readable): %s\n", preamble);

    s = fread(preamble, 1, 4, f);
    if ((s < 4) || memcmp(preamble, "DICM", 4)) {
        err = "Not DICOM file!";
        goto invalid;
    }

    // https://www.vladsiv.com/dicom-file-format-basics/
    // Start parsing all 0002 tags with Explicit VR Little Endian
    // Get(0002, 0010) - TransferSyntaxUID and use it to parse the Data Set
    // So get 0002 tags, serialize them, parse (0002,0010), serialize

    while (!feof(f)) {
        // https://plastimatch.org/dicom_tutorial.html
        // Optimization: reuse these allocations
        unsigned char * tag = (unsigned char *) malloc(4 * sizeof(char));
        s = fread(tag, 1, 4, f);
        if (s < 4) {
            err = "Not DICOM file!";
            goto invalid;
        }
        int tag_upper = ((unsigned int)tag[1] << 8) | (unsigned int)tag[0];
        int tag_lower = ((unsigned int)tag[3] << 8) | (unsigned int)tag[2];
        //int specific_tag = ((unsigned int)tag_upper << 16) | (unsigned int)tag_lower;
        free(tag);

        //printf("So far: %u %u %u %u\n", tag[0], tag[1], tag[2], tag[3]);
        printf("(%.4x,%.4x): ", tag_upper, tag_lower);

        if (tag_upper != 2) {
            printf("End of metadata!\n");
            break;
        }

        // NOTE: This code will need to be modified for implicit VR
        // https://dicom.nema.org/medical/dicom/current/output/pdf/part05.pdf 7.1.3
        // https://plastimatch.org/dicom_tutorial.html
        // Use header data to decide when to have it explicit
        // And when explicit VR, 2 bytes
        unsigned char *vr_buf = (unsigned char *)malloc(2 * sizeof(char));
        s = fread(vr_buf, 1, 2, f);
        if (s < 2)
        {
            err = "Not DICOM file!";
            goto invalid;
        }
        printf("%s ", vr_buf);

        int vr = (((int)vr_buf[0]) << 8) | vr_buf[1];
        free(vr_buf);

        char * (*parser_serializer)(char* bytes, int length);
        parser_serializer = &content_other;

        if (vr == VR_UL)
            parser_serializer = content_uint;
        else if (
            vr == VR_AE || vr == VR_AS || vr == VR_CS || vr == VR_LO || vr == VR_LT || vr == VR_PN || vr == VR_ST || vr == VR_UR || vr == VR_UI)
            parser_serializer = content_string;
        else if(vr == VR_OB)
            parser_serializer = content_debug;

        unsigned int length_bits = 2;

        if (reserved_padding_explicit_vr(vr)) {
            // Skip 16 bits
            if (fseek(f, 2, SEEK_CUR)) {
                err = "Not DICOM file!";
                goto invalid;
            }
            length_bits = 4;
        }

        unsigned char *len_buf = (unsigned char *)malloc(4 * sizeof(char));
        for (int i = 0; i < 4; i++)
            len_buf[i] = 0;
        s = fread(len_buf, 1, length_bits, f);
        if (s < length_bits) {
            err = "Not DICOM file!";
            goto invalid;
        }
        unsigned int content_len = (((unsigned int)len_buf[3]) << 24) | (((unsigned int)len_buf[2]) << 16) | (((unsigned int)len_buf[1]) << 8) | len_buf[0];
        printf("%u ", content_len);
        free(len_buf);

        unsigned char * content = (unsigned char *) malloc(content_len * sizeof(char));
        s = fread(content, 1, content_len, f);
        if (s < content_len) {
            err = "Not DICOM file!";
            goto invalid;
        }
        
        //output_writer_write(oc, parser_serializer(content, content_len));
        printf("%s\n", parser_serializer((char *) content, content_len));
    }

    if (0) {
    invalid:
    //fprintf(stderr, "%s", err);
        printf("%s", err);
    }
    if (0) {
    memory_allocator_fault:
    //fprintf(stderr, "Out of memory, could not prepare an answer");
        printf("%s", "Out of memory, could not prepare an answer");
    }

    // output_writer_finalize(oc)
    // For now, use printf
    return "Printed";
}
