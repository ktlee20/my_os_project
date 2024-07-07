#include "page.h"

/* Initialise page table entry for IA-32e mode kernel */
void
kinitialise_page_tables(void)
{
	pml4_t *pml4_entry;
	pdp_t *pdp_entry;
	pde_t *pd_entry;
	kdword mapping_address;
	int i;

	/* Create pml4 table */
	pml4_entry = (pml4_t *)0x100000;
	kset_page_entry_data(&(pml4_entry[0]), 0x00, 0x101000, PG_FLAGS_DEFAULT, 0);
	for (i = 1 ; i < PG_MAXENTRYCNT ; i++) {
		kset_page_entry_data(&(pml4_entry[i]), 0, 0, 0, 0);
	}

	pdp_entry = (pdp_t *)0x101000;
	for (i = 0 ; i < 64 ; i++) {
		kset_page_entry_data(&(pdp_entry[i]), 0, 0x102000 + (i * PG_TABLESIZE),
							 PG_FLAGS_DEFAULT, 0);
	}

	for (i = 64 ; i < PG_MAXENTRYCNT ; i++) {
		kset_page_entry_data(&(pdp_entry[i]), 0, 0, 0, 0);
	}

	pd_entry = (pde_t *)0x102000;
	mapping_address = 0;

	for (i = 0 ; i < PG_MAXENTRYCNT * 64; i++) {
		kset_page_entry_data(&(pd_entry[i]), (i * (PG_DEFAULTSIZE >> 20)) >> 12,
							 mapping_address, PG_FLAGS_DEFAULT|PG_FLAGS_PS, 0);
		mapping_address += PG_DEFAULTSIZE;
	}
}
