#include "../include/string.h"

void *memcpy(void *dest, const void *src, uint32_t n)
{
    uint8_t *d = (uint8_t*)dest;
    const uint8_t *s = (const uint8_t*)src;
    
    for (uint32_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

void *memset(void *s, int c, uint32_t n)
{
    uint8_t *p = (uint8_t*)s;
    
    for (uint32_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }
    return s;
}

int memcmp(const void *s1, const void *s2, uint32_t n)
{
    const uint8_t *p1 = (const uint8_t*)s1;
    const uint8_t *p2 = (const uint8_t*)s2;
    
    for (uint32_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}

uint32_t strlen(const char *s)
{
    uint32_t len = 0;
    while (*s++) len++;
    return len;
}

char *strcpy(char *dest, const char *src)
{
    char *d = dest;
    while ((*d++ = *src++));
    return dest;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
