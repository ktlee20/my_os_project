#include "interrupt_handler.h"
#include "pic.h"
#include "utility.h"

void
kcommon_exception_handler(int ivec_number, kqword error_code)
{
	char buffer[3] = {0, };

	buffer[0] = '0' + ivec_number / 10;
	buffer[1] = '0' + ivec_number % 10;

	k64print_string(0, 0, "=============================");
	k64print_string(0, 1, "       Exception Occur~      ");
	k64print_string(0, 2, "         Vector:             ");
	k64print_string(17, 2, buffer); 
	k64print_string(0, 3, "=============================");

	while (1);
}

void
kcommon_interrupt_handler(int ivec_number)
{
	char buffer[] = "[INT:  , ]";
	static int common_interrupt_count = 0;

	/* Set vector number to buffer */
	buffer[5] = '0' + ivec_number / 10;
	buffer[6] = '0' + ivec_number % 10;
	/* Set event counts */
	buffer[8] = '0' + common_interrupt_count;
	common_interrupt_count = (common_interrupt_count + 1) % 10;
	k64print_string(70, 0, buffer);

	ksend_eoi_to_pic(ivec_number - PIC_IRQ_START_VECTOR);
}

void
kkeyboard_handler(int ivec_number)
{
	char buffer[] = "[INT:  , ]";
	static int keyboard_interrupt_count = 0;

	buffer[5] = '0' + ivec_number / 10;
	buffer[5] = '0' + ivec_number % 10;

	buffer[8] = '0' + keyboard_interrupt_count;
	keyboard_interrupt_count = (keyboard_interrupt_count + 1) % 10;
	k64print_string(0, 0, buffer);

	ksend_eoi_to_pic(ivec_number - PIC_IRQ_START_VECTOR);
}
