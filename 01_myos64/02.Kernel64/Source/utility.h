#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "types64.h"

/* functions */
void kmemset(void *dest, kbyte data, int size);
int kmemcpy(void *dest, const void *src, int size);
int kmemcmp(const void *dest, const void *src, int size);
void k64print_string(int x, int y, const char *pc_string);

#endif /* __UTILITY_H__ */
