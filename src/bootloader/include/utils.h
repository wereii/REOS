#ifndef _BOOTLOADER_UTILS_H
#define _BOOTLOADER_UTILS_H

const char *_hexbase = "0123456789ABCDEF";

inline void
itohexa(long long int i, char *out){
  short nibbles = sizeof(i)*2;
  for(int x = 0; x < nibbles; x++){
    out[nibbles - 1 - x] = _hexbase[((i>>x*4)&0xf)];
  }
}

#endif //_BOOTLOADER_UTILS_H
