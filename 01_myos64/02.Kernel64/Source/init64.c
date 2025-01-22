#include "types64.h"
#include "keyboard.h"

void k64print_string(int x, int y, const char *pc_string);

void
kernel64_main(void)
{
	char ascii[2] = {0, };
	kbyte flags, scancode;
	int i = 0;

	k64print_string(27, 11, "PASS");
	k64print_string(0, 12, "IA-32e C Language Kernel Start.....[PASS]");
	k64print_string(0, 13, "Keyboard Activate..................[    ]");

	if (kactivate_keyboard() == TRUE) {
		k64print_string(35, 13, "PASS");
		kchange_keyboard_LED(FALSE, FALSE, FALSE);
	} else {
		k64print_string(35, 13, "FAIL");
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
			}
		}
	}
}

// Print string
void
k64print_string(int x, int y, const char *pc_string)
{
	kvideo_buf_character *pst_screen = (kvideo_buf_character *) 0xB8000;
	int i;

	pst_screen += (y * 80) + x;
	for (i = 0 ; pc_string[i] != 0 ; i++)
	{
		pst_screen[i].character = pc_string[ i ];
	}
}
