#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include "types64.h"

// Macroes
/**********************
********* GDT *********
**********************/

/* Macroes for memory types */
#define GDT_TYPE_CODE	0x0A
#define GDT_TYPE_DATA	0x02
#define GDT_TYPE_TSS	0x09
/* Macroes for DPL */
#define GDT_FLAGS_LOWER_DPL0	0x00
#define GDT_FLAGS_LOWER_DPL1	0x20
#define GDT_FLAGS_LOWER_DPL2	0x40
#define GDT_FLAGS_LOWER_DPL3	0x60
/* Macroes for other GDT bits */
#define GDT_FLAGS_LOWER_S	0x10
#define GDT_FLAGS_LOWER_P	0x80
#define GDT_FLAGS_UPPER_L	0x20
#define GDT_FLAGS_UPPER_DB	0x40
#define GDT_FLAGS_UPPER_G	0x80

/* Macro combinations */
/* Lower flags */
#define GDT_FLAGS_LOWER_KERNELCODE (GDT_TYPE_CODE | GDT_FLAGS_LOWER_S |	\
									GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P)
#define GDT_FLAGS_LOWER_KERNELDATA (GDT_TYPE_DATA | GDT_FLAGS_LOWER_S | \
									GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P)
#define GDT_FLAGS_LOWER_TSS (GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P)
#define GDT_FLAGS_LOWER_USERCODE (GDT_TYPE_CODE | GDT_FLAGS_LOWER_S | \
								  GDT_FLAGS_LOWER_DPL3 | GDT_FLAGS_LOWER_P)
#define GDT_FLAGS_LOWER_USERDATA (GDT_TYPE_DATA | GDT_FLAGS_LOWER_S | \
								  GDT_FLAGS_LOWER_DPL3 | GDT_FLAGS_LOWER_P)
/* Upper flags */
#define GDT_FLAGS_UPPER_CODE (GDT_FLAGS_UPPER_G | GDT_FLAGS_UPPER_L)
#define GDT_FLAGS_UPPER_DATA (GDT_FLAGS_UPPER_G | GDT_FLAGS_UPPER_L)
#define GDT_FLAGS_UPPER_TSS (GDT_FLAGS_UPPER_G)

/* Segment descriptor offsets */
#define GDT_KERNEL_CODE_SEGMENT 0x08
#define GDT_KERNEL_DATA_SEGMENT 0x10
#define GDT_TSS_SEGMENT 0x18

/* Other Macroes for GDT */
#define GDTR_START_ADDRESS	0x142000
#define GDT_MAX_8BYTES_ENTRY_COUNT 3
#define GDT_MAX_16BYTES_ENTRY_COUNT 1
#define GDT_TABLE_SIZE ((sizeof(gdt_8bytes_entry) * \
		GDT_MAX_8BYTES_ENTRY_COUNT) + (sizeof(gdt_16bytes_entry) * \
		GDT_MAX_16BYTES_ENTRY_COUNT))
#define TSS_SEGMENT_SIZE (sizeof(tss_segment))


/**********************
********* IDT *********
**********************/

/* Macroes for each field */
#define IDT_TYPE_INTERRUPT	0x0E
#define IDT_TYPE_TRAP		0x0F
#define IDT_FLAGS_DPL0		0x00
#define IDT_FLAGS_DPL1		0x20
#define IDT_FLAGS_DPL2		0x40
#define IDT_FLAGS_DPL3		0x60
#define IDT_FLAGS_P			0x80
#define IDT_FLAGS_IST0		0
#define IDT_FLAGS_IST1		1

/* Macroes for using */
#define IDT_FLAGS_KERNEL	(IDT_FLAGS_DPL0 | IDT_FLAGS_P)
#define IDT_FLAGS_USER		(IDT_FLAGS_DPL3 | IDT_FLAGS_P)

/* Other maroes for IDT */
/* The number of IDT entries */
#define IDT_MAX_ENTRY_COUNT		100
/* IDT start address, located behind tss segments */
#define IDTR_START_ADDRESS	(GDTR_START_ADDRESS + sizeof(gdtr) + \
		GDT_TABLE_SIZE + TSS_SEGMENT_SIZE)
/* The start address of IDT table */
#define IDT_START_ADDRESS (IDTR_START_ADDRESS + sizeof(idtr))
/* The size of IDT table */
#define IDT_TABLE_SIZE (IDT_MAX_ENTRY_COUNT + sizeof(idt_entry))
/* The start address of IST */
#define IST_START_ADDRESS 0x700000
/* The size of IST */
#define IST_SIZE 0x100000

/* C structures */
/* Align with 1 byte */
#pragma pack(push, 1)
typedef struct kernel_descriptor_register {
	kword limit;
	kqword base_address;
	/* For 16-byte alignment */
	kword pading;
	kdword pading2;
} gdtr, idtr;

/* 8-byte gdt entry structure */
typedef struct kernel_gdt_8bytes_entry_structure {
	kword lower_limit;
	kword lower_base_address;
	kbyte upper_base_address1;
	/* 4 bits type, 1 bit S, 2 bits DPL, 1 bit P */
	kbyte type_and_lower_flags;
	/* 4 bits segment limit, 1 bit AVL, L, D/B, G */
	kbyte upper_limit_and_upper_flags;
	kbyte upper_base_address2;
} gdt_8bytes_entry;

/* 16-byte gdt entry structure */
typedef struct kernel_gdt_16bytes_entry_structure {
	kword lower_limit;
	kword lower_base_address;
	kbyte middle_base_address1;
	/* 4 bits type, 1 bit 0, 2 bits DPL, 1 bit P */
	kbyte type_and_lower_flags;
	/* 4 bits segment limit, 1 bit AVL, 0, 0, G */
	kbyte upper_limit_and_upper_flags;
	kbyte middle_base_address2;
	kdword upper_base_address;
	kdword reserved;
} gdt_16bytes_entry;

typedef struct kernel_tss_data_structure {
	kdword reserved1;
	kqword rsp[3];
	kqword reserved2;
	kqword ist[7];
	kqword reserved3;
	kword reserved4;
	kword io_map_base_address;
} tss_segment;

typedef struct kernel_idt_entry_structure {
	kword lower_base_address;
	kword segment_selector;
	/* 3bits ist, 5bits 0 */
	kbyte  ist;
	/* 4bits type, 1bit 0, 2bit DPL, 1bit P */
	kbyte type_and_flags;
	kword middle_base_address;
	kdword upper_base_address;
	kdword reserved;
} idt_entry;

#pragma pack (pop)

/* Functions */
void kinitialise_gdt_and_tss(void);
void kinitialise_idt_tables(void);

#endif
