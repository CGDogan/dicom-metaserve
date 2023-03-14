// https://dicom.nema.org/dicom/2013/output/chtml/part05/sect_6.2.html
// https://pydicom.github.io/pydicom/stable/guides/element_value_types.html
enum {
    VR_AE = 0x4145,
    VR_AS = 0x4153,
    VR_AT = 0x4154,
    VR_CS = 0x4353,
    VR_DA = 0x4441,
    VR_DS = 0x4453,
    VR_DT = 0x4454,
    VR_FL = 0x464C,
    VR_FD = 0x4644,
    VR_IS = 0x4953,
    VR_LO = 0x4C4F,
    VR_LT = 0x4C54,
    VR_OB = 0x4F42,
    VR_OD = 0x4F44,
    VR_OF = 0x4F46,
    VR_OL = 0x4F4C,
    VR_OV = 0x4F56,
    VR_OW = 0x4F57,
    VR_PN = 0x504E,
    VR_SH = 0x5348,
    VR_SL = 0x534C,
    VR_SQ = 0x5351,
    VR_SS = 0x5353,
    VR_ST = 0x5354,
    VR_SV = 0x5356,
    VR_TM = 0x544D,
    VR_UC = 0x5543,
    VR_UI = 0x5549,
    VR_UL = 0x554C,
    VR_UN = 0x554E,
    VR_UR = 0x5552,
    VR_US = 0x5553,
    VR_UT = 0x5554,
    VR_UV = 0x5556,
};

int reserved_padding_explicit_vr(int vr);

char *content_other(char *c, int len);
char *content_uint(char *c, int len);
char *content_string(char *c, int len);
char *content_debug(char *c, int len);
