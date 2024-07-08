#include "types.h"
#include "page.h"
#include "ModeSwitch.h"

void kprint_string(int x, int y, const char *pc_string);
kbool kinitialise_kernel_code_area(void);
kbool kcheck_init_memory_space(void);
void kcopy_kernel64_image_to_2mb(void);

// main function
void
kernel_main( void )
{
	kdword i;
	kdword eax, ebx, ecx, edx;
	char vendor_name[13] = {0, };

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

	/* Initialise page tables */
	kprint_string(0, 7, "IA-32e Page Tables Initialise.....[    ]");
	kinitialise_page_tables();
	kprint_string(35, 7, "PASS");

	/* Read vendor information  */
	kReadCPUID(0x00, &eax, &ebx, &ecx, &edx);
	*(kdword *)vendor_name = ebx;
	*((kdword *)vendor_name + 1) = edx;
	*((kdword *)vendor_name + 2) = ecx;
	kprint_string(0, 8, "Processor Vendor String.....[            ]");
	kprint_string(29, 8, vendor_name);

	/* Check if 64-bit is supported */
	kReadCPUID(0x80000001, &eax, &ebx, &ecx, &edx);
	kprint_string(0, 9, "64bit Mode Support Check.....[    ]");
	if (edx & (1 << 29)) {
		kprint_string(30, 9, "PASS");
	} else {
		kprint_string(30, 9, "FAIL");
		kprint_string(0, 10, "This processor does not support 64bit mode");
		while (1);
	}

	kprint_string(0, 10, "Copy IA-32e Kernel to 2M Address.....[    ]");	
	kcopy_kernel64_image_to_2mb();
	kprint_string(38, 10, "PASS");

	kprint_string(0, 11, "Switch To IA-32e Mode.....[    ]");
	kSwitchAndExecute64bitKernel();

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

void
kcopy_kernel64_image_to_2mb(void)
{
	kword kernel32_sector_count, total_kernel_sector_count;
	kdword *src_addr, *dst_addr;
	int i;

	total_kernel_sector_count = *((kword *) 0x7c05);
	kernel32_sector_count = *((kword *) 0x7C07);

	src_addr = (kdword *) (0x10000 + (kernel32_sector_count * 512));
	dst_addr = (kdword *) 0x200000;

	for (i = 0 ;
		 i < 512 * (total_kernel_sector_count - kernel32_sector_count) / 4 ;
		 i++) {
		*dst_addr = *src_addr;
		dst_addr++;
		src_addr++;
	}
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
