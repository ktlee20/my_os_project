#include "utility.h"

/*
 * Set memory space(@dest) to @data by @size
 */
void
kmemset(void *dest, kbyte data, int size)
{
	int i;

	for (i = 0 ; i < size ; i++) {
		((char *)dest)[i] = data;
	}
}

/*
 * Copy memory space(@src) to @dest by @size
 */
int
kmemcpy(void *dest, const void *src, int size)
{
	int i;

	for (i = 0 ; i < size ; i++) {
		((char *)dest)[i] = ((char *)src)[i];
	}

	return size;
}

int
kmemcmp(const void *dest, const void *src, int size)
{
	int i;
	char result = 0;

	for (i = 0 ; i < size ; i++) {
		if ((result = ((char *)dest)[i] - ((char *)src)[i])) {
			return result;
		}
	}

	return result;
}

// Print string
void
k64print_string(int x, int y, const char *pc_string)
{
	kvideo_buf_character *pst_screen = (kvideo_buf_character *) 0xB8000;
	int i;

	pst_screen += (y * 80) + x;
	for (i = 0 ; pc_string[i] != 0 ; i++)
	{
		pst_screen[i].character = pc_string[ i ];
	}
}
