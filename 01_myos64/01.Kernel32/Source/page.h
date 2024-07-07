#ifndef __PAGE_H__
#define __PAGE_H__

#include "types.h"

/* Macro */
#define PG_FLAGS_P			0x00000001
#define PG_FLAGS_RW			0x00000002
#define PG_FLAGS_US			0x00000004
#define PG_FLAGS_PWT		0x00000008
#define PG_FLAGS_PCD		0x00000010
#define PG_FLAGS_A			0x00000020
#define PG_FLAGS_D			0x00000040
#define PG_FLAGS_PS			0x00000080
#define PG_FLAGS_G			0x00000100
#define PG_FLAGS_PAT		0x00001000
#define PG_FLAGS_EXB		0x80000000
#define PG_FLAGS_DEFAULT	(PG_FLAGS_P|PG_FLAGS_RW)
#define PG_TABLESIZE		0x1000
#define PG_MAXENTRYCNT		512
#define PG_DEFAULTSIZE		0x200000

#pragma pack(push, 1)
typedef struct kpage_table_entry {
	kdword attribute_and_lower_base_address;
	kdword upper_base_address_and_exb;
} pml4_t, pdp_t, pde_t, pte_t;
#pragma pack(pop)

void kinitialise_page_tables(void);
#define kset_page_entry_data(pst_entry, upper_base_address,				\
						  lower_base_address, lower_flags, upper_flags)	\
{																		\
	((pst_entry)->attribute_and_lower_base_address) =					\
			(lower_base_address | lower_flags);							\
	((pst_entry)->upper_base_address_and_exb) =							\
			((upper_base_address & 0xFF) | upper_flags);				\
}
#endif /* __PAGE_H__ */
