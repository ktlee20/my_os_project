#include "types64.h"
#include "keyboard.h"
#include "descriptor.h"
#include "assembly_utility.h"
#include "utility.h"

void
kernel64_main(void)
{
	char ascii[2] = {0, };
	kbyte flags, scancode;
	int i = 0;

	k64print_string(27, 11, "PASS");
	k64print_string(0, 12, "IA-32e C Language Kernel Start.....[PASS]");
	k64print_string(0, 13, "Initialise GDT.....................[    ]");
	kinitialise_gdt_and_tss();	
	kLoadGDTR(GDTR_START_ADDRESS);
	k64print_string(35, 13, "PASS");

	k64print_string(0, 14, "Load TSS segment...................[    ]");
	kLoadTR(GDT_TSS_SEGMENT);
	k64print_string(35, 14, "PASS");

	k64print_string(0, 15, "Initialise IDT.....................[    ]");
	kinitialise_idt_tables();
	kLoadIDTR(IDTR_START_ADDRESS);
	k64print_string(35, 15, "PASS");

	k64print_string(0, 16, "Keyboard Activate..................[    ]");

	if (kactivate_keyboard() == TRUE) {
		k64print_string(35, 16, "PASS");
		kchange_keyboard_LED(FALSE, FALSE, FALSE);
	} else {
		k64print_string(35, 16, "FAIL");
		while (1);
	}

	while (1) {
		/* if output buffer is full, then we can read scancode */
		if (kcheck_output_buffer_is_full()) {
			/* Get scancode from output buffer */
			scancode = kget_keyboard_scancode();

			/* If chaning ASCII code is required, then call convert
			 * function */
			if (kconvert_scancode_to_ASCII(scancode, ascii, &flags)) {
				if (flags & KEY_FLAGS_DOWN) {
					k64print_string(i++, 14, ascii);
				}

				if (ascii[0] == '0') {
					scancode /= 0;
				}
			}
		}
	}
}
