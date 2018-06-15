#define blockSize (uint8_t)0x40

uint16_t blockAddrArray[] = {0x8080, 0x40, 0x80C0, 0x40, 0x8000, 0x40, 0x8040, 0x40};

unsigned char blockDataArray[] = {\
0x80, 0x83, 0x00, 0xAE, 0x03, 0xFF, 0x94, 0x90, \
0xCE, 0x80, 0x80, 0xAE, 0x80, 0x82, 0xF6, 0x27, \
0x25, 0xA5, 0x60, 0x27, 0x17, 0xBF, 0x00, 0xEE, \
0x03, 0xBF, 0x03, 0xBE, 0x00, 0xEE, 0x01, 0x90, \
0xF6, 0xF7, 0x5C, 0x90, 0x5C, 0x90, 0xB3, 0x03, \
0x26, 0xF5, 0xBE, 0x00, 0x90, 0x93, 0x90, 0xEE, \
0x03, 0x1C, 0x00, 0x05, 0x20, 0xD8, 0xAE, 0x00, \
0x00, 0x20, 0x02, 0xF7, 0x5C, 0xA3, 0x00, 0x06, \
0x26, 0xF9, 0xAE, 0x01, 0x00, 0x20, 0x02, 0xF7, \
0x5C, 0xA3, 0x01, 0x00, 0x26, 0xF9, 0xCD, 0x80, \
0xD3, 0x20, 0xFE, 0x72, 0x5F, 0x50, 0xC7, 0x72, \
0x5F, 0x50, 0xCA, 0x20, 0xFE, 0x80, 0x80, 0x80, \
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, \
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, \
0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
0x82, 0x00, 0x80, 0x83, 0x82, 0x00, 0x80, 0xDE, \
0x82, 0x00, 0x80, 0xDF, 0x82, 0x00, 0x80, 0xE0, \
0x82, 0x00, 0x80, 0xE1, 0x82, 0x00, 0x80, 0xE2, \
0x82, 0x00, 0x80, 0xE3, 0x82, 0x00, 0x80, 0xE4, \
0x82, 0x00, 0x80, 0xE5, 0x82, 0x00, 0x80, 0xE6, \
0x82, 0x00, 0x80, 0xDD, 0x82, 0x00, 0x80, 0xDD, \
0x82, 0x00, 0x80, 0xE7, 0x82, 0x00, 0x80, 0xE8, \
0x82, 0x00, 0x80, 0xE9, 0x82, 0x00, 0x80, 0xEA, \
0x82, 0x00, 0x80, 0xEB, 0x82, 0x00, 0x80, 0xDD, \
0x82, 0x00, 0x80, 0xDD, 0x82, 0x00, 0x80, 0xEC, \
0x82, 0x00, 0x80, 0xED, 0x82, 0x00, 0x80, 0xEE, \
0x82, 0x00, 0x80, 0xDD, 0x82, 0x00, 0x80, 0xDD, \
0x82, 0x00, 0x80, 0xEF, 0x82, 0x00, 0x80, 0xF0, \
0x82, 0x00, 0x80, 0xF1, 0x82, 0x00, 0x80, 0xDD, \
0x82, 0x00, 0x80, 0xDD, 0x82, 0x00, 0x80, 0xDD, \
0x82, 0x00, 0x80, 0xDD, 0x82, 0x00, 0x80, 0xDD};
