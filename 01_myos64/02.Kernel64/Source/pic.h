#ifndef __PIC_H__
#define __PIC_H__

#include "types64.h"

/* Macroes */
/* Define I/O ports */
#define PIC_MASTER_PORT1	0x20
#define PIC_MASTER_PORT2	0x21
#define PIC_SLAVE_PORT1		0xA0
#define PIC_SLAVE_PORT2		0xA1

/* Start location of IDT table, 0x20 */
#define PIC_IRQ_START_VECTOR	0x20

void kinitialise_pic(void);
void kmask_pic_interrupt(kword irq_bitmask);
void ksend_eoi_to_pic(int irq_number);

#endif /* __PIC_H__ */
