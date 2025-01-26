#include "descriptor.h"
#include "utility.h"

void kdummy_handler(void);

/*
* This code includes functions handling GDT and IDT
*/

/*
 * Set up gdt 8-byte entry values
 */
void
kset_gdt_8bytes_entry(gdt_8bytes_entry *p_gdt_entry, kdword base_address,
					  kdword limit, kbyte upper_flags, kbyte lower_flags,
					  kbyte type)
{
	p_gdt_entry->lower_limit = limit & 0xFFFF;
	p_gdt_entry->lower_base_address = base_address & 0xFFFF;
	p_gdt_entry->upper_base_address1 = (base_address >> 16) & 0xFF;
	p_gdt_entry->type_and_lower_flags = lower_flags | type;
	p_gdt_entry->upper_limit_and_upper_flags = ((limit >> 16) & 0x0F) |
												upper_flags;
	p_gdt_entry->upper_base_address2 = (base_address >> 24) & 0xFF;
}

/*
 * Set up gdt 16-byte entry values
 */
void
kset_gdt_16bytes_entry(gdt_16bytes_entry *p_gdt_entry, kqword base_address,
					   kdword limit, kbyte upper_flags, kbyte lower_flags,
					   kbyte type)
{
	p_gdt_entry->lower_limit = limit & 0xFFFF;
	p_gdt_entry->lower_base_address = base_address & 0xFFFF;
	p_gdt_entry->middle_base_address1 = (base_address >> 16) & 0xFF;
	p_gdt_entry->type_and_lower_flags = lower_flags | type;
	p_gdt_entry->upper_limit_and_upper_flags = ((limit >> 16) & 0x0F) |
											   upper_flags;
	p_gdt_entry->middle_base_address2 = (base_address >> 24) & 0xFF;
	p_gdt_entry->upper_base_address = base_address >> 32;
	p_gdt_entry->reserved = 0;
}

/*
 * Initialise TSS segment
 */
void
kinitialise_tss_segment(tss_segment *p_tss)
{
	kmemset(p_tss, 0, sizeof(tss_segment));
	p_tss->ist[0] = IST_START_ADDRESS + IST_SIZE;
	/* Set IO map base address upto limit to ban IO map usage */
	p_tss->io_map_base_address = 0xFFFF;
}

/*
 * Initialise GDT table
 */
void
kinitialise_gdt_and_tss(void)
{
	gdtr *p_gdtr;
	gdt_8bytes_entry *p_gdt_entry;
	tss_segment *p_tss;

	/* Set up GDTR */
	p_gdtr = (gdtr *)GDTR_START_ADDRESS;
	p_gdt_entry = (gdt_8bytes_entry *)(GDTR_START_ADDRESS + sizeof(gdtr));
	p_gdtr->limit = GDT_TABLE_SIZE - 1;
	p_gdtr->base_address = (kqword)p_gdt_entry;

	/* Set up TSS segment */
	p_tss = (tss_segment *)((kqword)p_gdt_entry + GDT_TABLE_SIZE);

	/* Set up NULL, 64bit code, data and TSS gate descriptor tables */
	kset_gdt_8bytes_entry(&(p_gdt_entry[0]), 0, 0, 0, 0, 0);
	kset_gdt_8bytes_entry(&(p_gdt_entry[1]), 0, 0xFFFFF,
			GDT_FLAGS_UPPER_CODE, GDT_FLAGS_LOWER_KERNELCODE, GDT_TYPE_CODE);
	kset_gdt_8bytes_entry(&(p_gdt_entry[2]), 0, 0xFFFFF,
			GDT_FLAGS_UPPER_DATA, GDT_FLAGS_LOWER_KERNELDATA, GDT_TYPE_DATA);
	kset_gdt_16bytes_entry((gdt_16bytes_entry *)&(p_gdt_entry[3]),
			(kqword)p_tss, sizeof(tss_segment) - 1, GDT_FLAGS_UPPER_TSS,
			GDT_FLAGS_LOWER_TSS, GDT_TYPE_TSS);

	/* Initialise tss segment */
	kinitialise_tss_segment(p_tss);
}


/*
 * Set up IDT values
 */
void
kset_idt_entry(idt_entry *p_idt_entry, void *interrupt_handler, kword selector,
			   kbyte ist, kbyte flags, kbyte type)
{
	p_idt_entry->lower_base_address = (kqword)interrupt_handler & 0xFFFF;
	p_idt_entry->segment_selector = selector;
	p_idt_entry->ist = ist & 0x3;
	p_idt_entry->type_and_flags = type | flags;
	p_idt_entry->middle_base_address = ((kqword)interrupt_handler>>16) & 0xFFFF;
	p_idt_entry->upper_base_address = (kqword)interrupt_handler >> 32;
	p_idt_entry->reserved = 0;
}

void
kinitialise_idt_tables(void)
{
	idtr *p_idtr;
	idt_entry *p_idt_entry;
	int i;

	p_idtr = (idtr *)IDTR_START_ADDRESS;
	p_idt_entry = (idt_entry *)(IDTR_START_ADDRESS + sizeof(idtr));

	p_idtr->base_address = (kqword)p_idt_entry;
	p_idtr->limit = IDT_TABLE_SIZE - 1;

	for (i = 0 ; i < IDT_MAX_ENTRY_COUNT ; i++) {
		kset_idt_entry(&(p_idt_entry[i]), kdummy_handler, 0x08, IDT_FLAGS_IST1,
					   IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	}
}

/*
 * Dummy interrupt handler
 */
void
kdummy_handler(void)
{
	k64print_string(0, 0, "=============================");
	k64print_string(0, 1, "   Dummy interrupt handler   ");
	k64print_string(0, 2, "Exception or interrupt occurs");
	k64print_string(0, 3, "=============================");

	while (1);
}
