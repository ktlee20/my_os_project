#include "pic.h"
#include "assembly_utility.h"

/*
 * Initialise PIC
 */

void
kinitialise_pic(void)
{
	/* PIC is controlled by ICW commands(1~4), and these commands
	 * are transferred through I/O port. */
	/* Initialise master PIC controller */
	/* ICW1(port 0x20), IC4 bit = 1 */
	kOutPortByte(PIC_MASTER_PORT1, 0x11);

	/* ICW2(port 0x21), and start interrupt vector */
	kOutPortByte(PIC_MASTER_PORT2, PIC_IRQ_START_VECTOR);

	/* ICW3(port 0x21), slave PIC controller connection location
	 * It is connected to MASTER PIC controller's #2 pin.
	 * Set 0x04 */
	kOutPortByte(PIC_MASTER_PORT2, 0x04);

	/* ICW4(port 0x21), uPM bit = 1 */
	kOutPortByte(PIC_MASTER_PORT2, 0x01);

	/* Initialise slave PIC controller */
	/* ICW1(port 0xA0), IC4 bit = 1 */
	kOutPortByte(PIC_SLAVE_PORT1, 0x11);

	/* ICW2(port 0xA1), interrupt vector(0x20 + 8) */
	kOutPortByte(PIC_SLAVE_PORT2, PIC_IRQ_START_VECTOR + 8);

	/* ICW3(port 0xA1), connected location with master port */
	kOutPortByte(PIC_SLAVE_PORT2, 0x02);

	/* ICW4(port 0xA1), uPM bit = 1 */
	kOutPortByte(PIC_SLAVE_PORT2, 0x01);
}

/*
 * masking # of interrupts to prevent specific interrupts
 */
void
kmask_pic_interrupt(kword irq_bitmask)
{
	/* Set IMR to master PIC controller
	 * OCW1(port 0x21), IRQ 0 - IRQ 7
	 */
	kOutPortByte(PIC_MASTER_PORT2, (kbyte)irq_bitmask);

	/* Set IMR to slave PIC controller
	 * OCW1(port 0xA1), IRQ 8 - IRQ 15
	 */
	kOutPortByte(PIC_MASTER_PORT2, (kbyte)(irq_bitmask >> 8));
}

/*
 * Sending the completion of interrupt processing
 */
void
ksend_eoi_to_pic(int irq_number)
{
	/* Send EOI to PIC controller
	 * OCW2(port 0x20), EOI bit(Bit 5) = 1
	 */
	kOutPortByte(PIC_MASTER_PORT1, 0x20);

	/* Send EOI to slave PIC controller if the processed interrupt was
	 * slave PIC's */
	if (irq_number >= 8) {
		/* OCW2(port 0xA0), EOI bit(bit #, 5) 1 */
		kOutPortByte(PIC_SLAVE_PORT1, 0x20);
	}
}
