#ifndef _BOOTLOADER_UTILS_H
#define _BOOTLOADER_UTILS_H

const char _hexbase[] = "0123456789ABCDEF";

static void itohexa(long long int i, char *out)
{
    short nibbles = sizeof(i) * 2;
    for (int x = 0; x < nibbles; x++)
    {
        out[nibbles - 1 - x] = _hexbase[((i >> x * 4) & 0xf)];
    }
}

typedef unsigned char byte;

// Never use this again anywhere
void *memset(void *s, int c, __SIZE_TYPE__ sz)
{
    __UINTPTR_TYPE__ *p;
    __UINTPTR_TYPE__ x = c & 0xff;
    byte *pp = (byte *)s;
    byte xx = c & 0xff;
    __SIZE_TYPE__ tail;
    int i;
    int bytes_per_word;

    for (i = 3; (1 << i) < 64; ++i)
        x |= x << (1 << i);
    bytes_per_word = 1 << (i - 3);

    /* Prologue. */
    while (((unsigned int)pp & (bytes_per_word - 1)) && sz--)
        *pp++ = xx;
    tail = sz & (bytes_per_word - 1);
    p = (__UINTPTR_TYPE__ *)pp;

    /* Main loop. */
    sz >>= i - 3;
    while (sz--)
        *p++ = x;

    /* Epilogue. */
    pp = (byte *)p;
    while (tail--)
        *pp++ = xx;

    return s;
}

static void GetUnicodeChar(unsigned int code, char chars[5])
{
    if (code <= 0x7F)
    {
        chars[0] = (code & 0x7F);
        chars[1] = '\0';
    }
    else if (code <= 0x7FF)
    {
        // one continuation byte
        chars[1] = 0x80 | (code & 0x3F);
        code = (code >> 6);
        chars[0] = 0xC0 | (code & 0x1F);
        chars[2] = '\0';
    }
    else if (code <= 0xFFFF)
    {
        // two continuation bytes
        chars[2] = 0x80 | (code & 0x3F);
        code = (code >> 6);
        chars[1] = 0x80 | (code & 0x3F);
        code = (code >> 6);
        chars[0] = 0xE0 | (code & 0xF);
        chars[3] = '\0';
    }
    else if (code <= 0x10FFFF)
    {
        // three continuation bytes
        chars[3] = 0x80 | (code & 0x3F);
        code = (code >> 6);
        chars[2] = 0x80 | (code & 0x3F);
        code = (code >> 6);
        chars[1] = 0x80 | (code & 0x3F);
        code = (code >> 6);
        chars[0] = 0xF0 | (code & 0x7);
        chars[4] = '\0';
    }
    else
    {
        // unicode replacement character
        chars[2] = 0xEF;
        chars[1] = 0xBF;
        chars[0] = 0xBD;
        chars[3] = '\0';
    }
}

static inline unsigned short char2short(const unsigned char high, const unsigned char low)
{
    return ((unsigned short)high << 8) | (unsigned short)low;
}

static void atow(const char in[], const unsigned int size, unsigned short *out)
{
    for (unsigned int i = 0; i < size; i++)
    {
        out[i] = char2short('\0', in[i]);
    }
}

#endif //_BOOTLOADER_UTILS_H