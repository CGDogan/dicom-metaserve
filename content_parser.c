#include "stdio.h"
#include "stdlib.h"
#include "content_parser.h"

#ifdef SERVER
#include "usr/local/include/fcgi_stdio.h"
#endif

// http://oftankonyv.reak.bme.hu/tiki-index.php?page=Value+representation+in+DICOM
// https://dicom.nema.org/medical/dicom/current/output/pdf/part05.pdf 7.1.2
int reserved_padding_explicit_vr(int vr)
{
    switch (vr)
    {
    case VR_AE:
    case VR_AS:
    case VR_AT:
    case VR_CS:
    case VR_DA:
    case VR_DS:
    case VR_DT:
    case VR_FL:
    case VR_FD:
    case VR_IS:
    case VR_LO:
    case VR_LT:
    case VR_PN:
    case VR_SH:
    case VR_SL:
    case VR_SS:
    case VR_ST:
    case VR_TM:
    case VR_UI:
    case VR_UL:
    case VR_US:
        // 32 bit length, 16 bit reserved bytes
        return 0;
    default:
        // TODO: for OB, OD, OF, OL, OV, OW, SQ and UN, implement undefined length with delimitation (0xFFFFFFFF)
        // SV, UC, UR, UV and UT may not
        return 1;
    }
}

char * content_other(char * c, int len) {
    (void) c;
    (void) len;
    return "(Contents Skipped)";
}

char * content_uint(char * c, int len)
{
    unsigned char * limit = (unsigned char *) &c[len - 1];
    unsigned long int s = 0;
    while (limit >= (unsigned char *) c) {
        s <<= 8;
        s |= *limit;
        limit--;
    }
    char * num = malloc(21 * sizeof(char));
    sprintf(num, "%lu", s);
    return num;
}

char * content_string(char *c, int len) {
    (void) len;
    return c;
}

char * content_debug(char *c, int len) {
    unsigned char * uc = (unsigned char *)c;
    // TODO: find a solution to memory leak. Perhaps pass the output char* from parent.
    char * out = (char *) malloc((len * 5 + 1) * sizeof(char));
    char * idx = out;
    for (unsigned char *x = uc; x < (uc + len); x++)
        idx += sprintf(idx, "0x%u ", (unsigned int)*x);
    return out;
}
