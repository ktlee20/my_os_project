#include "types64.h"

void k64print_string(int x, int y, const char *pc_string);

void
kernel64_main(void)
{
	k64print_string(27, 11, "PASS");
	k64print_string(0, 12, "IA-32e C Language Kernel Start.....[PASS]");
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
