#include "types.h"

void kprint_string(int x, int y, const char *pc_string);
kbool kinitialise_kernel_code_area(void);
kbool kcheck_init_memory_space(void);

// main function
void
kernel_main( void )
{
	kdword i;

	kprint_string(0, 4, "C Language Kernel Started.....[PASS]");

	kprint_string(0, 5, "Minimum Memory Size Check.....[    ]");

	if (!kcheck_init_memory_space()) {
		kprint_string(31, 5, "FAIL");
		kprint_string(0, 6, "Not enough memory or improper memory settings");
		kprint_string(0, 7, "This OS requires minimum 64MB memory");
		while (1);
	} else {
		kprint_string(31, 5, "PASS");
	}

	/* Initialise 1MB - 6MB memory area to load IA-32e mode kernel */
	kprint_string(0, 6, "IA-32e Kernel Area Initialise.....[    ]");
	if (kinitialise_kernel_code_area() == FALSE) {
		kprint_string(35, 6, "FAIL");
		kprint_string(0, 7, "Kernel Area Initialisation Fail");
		while (1);
	}
	kprint_string(35, 6, "PASS");

	while (1);
}

/*
 * Check if this system has enough memory and proper settings to load OS
 */
kbool
kcheck_init_memory_space(void)
{
	kdword *init_memory_address;
	init_memory_address = (kdword *) 0x100000;

	while ((kdword)init_memory_address < 0x4000000) {
		*init_memory_address = 0xDEADBEEF;

		if (*init_memory_address != 0xDEADBEEF) {
			return FALSE;
		}

		init_memory_address += (0x100000 / 4);
	}
	return TRUE;
}

/*
 * Initialise the specific kenrel area to 0.
 */
kbool
kinitialise_kernel_code_area(void)
{
	kdword *init_memory_address;

	/* Set 1MB location */
	init_memory_address = (kdword *) 0x100000;

	while ((kdword)init_memory_address < 0x600000) {
		*init_memory_address = 0x00;

		/* Double check */
		if (*init_memory_address != 0) {
			return FALSE;
		}

		/* Move to the next location */
		init_memory_address++;
	}
	return TRUE;
}

// Print string
void
kprint_string(int x, int y, const char *pc_string)
{
	kvideo_buf_character *pst_screen = (kvideo_buf_character *) 0xB8000;
	int i;

	pst_screen += (y * 80) + x;
	for (i = 0 ; pc_string[i] != 0 ; i++)
	{
		pst_screen[i].character = pc_string[ i ];
	}
}
