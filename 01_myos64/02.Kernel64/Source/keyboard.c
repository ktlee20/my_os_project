#include "types64.h"
#include "assembly_utility.h"
#include "keyboard.h"

/*
 * Functions to control keyboard controller
 */
kbool
kcheck_output_buffer_is_full(void)
{
	/* The 0-bit of status register is on,
	 * data transferred by a keyboard exists. */
	if (kInPortByte(0x64) & 0x01) {
		return TRUE;
	}
	return FALSE;
}

/*
 * Functions to control keyboard controller
 */
kbool
kcheck_input_buffer_is_full(void)
{
	/* The 1-bit of status register is on,
	 * data written by a processor exists. */
	if (kInPortByte(0x64) & 0x02) {
		return TRUE;
	}
	return FALSE;
}

/*
 * Activate a keyborad
 */
kbool
kactivate_keyboard(void)
{
	int i, j;

	/* Transfer activation codes(0xAE) to activate keyboard */
	kOutPortByte(0x64, 0xAE);

	/* Wait until input buffer becomes empty */
	while (kcheck_input_buffer_is_full());

	__sync_sychronized();

	/* Transfer codes(0xAE) to turn on a keyboard to input buffer */
	kOutPortByte(0x60, 0xF4);

	/* Wait until ACK message from a keyboard */
	while (!kcheck_output_buffer_is_full());

	__sync_sychronized();

	/* Data from output buffer(0x60) is ACK(0xFA).
	 * Succeed. */
	if (kInPortByte(0x60) == 0xFA) {
		return TRUE;
	}

	return FALSE;
}

/*
 * Read key from output buffer
 */
kbyte
kget_keyboard_scan_code(void)
{
	while (kcheck_output_buffer_is_full() == FALSE);

	__sync_sychronized();
	return kInPortByte(0x60);
}


/*
 * Toggle keyboard LED
 */
kbool
kchange_keyboard_LED(kbool caps_lock_on, kbool num_lock_on, kbool scroll_lock_on)
{
	/* Wait for flushing keyboard input buffer */
	while(kcheck_input_buffer_is_full());

	__sync_sychronized();

	/* Send LED change command to keyboard output buffer */
	kOutPortByte(0x60, 0xED);

	/* Wait for flushing keyboard input buffer */
	while(kcheck_input_buffer_is_full());

	/* Wait until ACK message from a keyboard */
	while (!kcheck_output_buffer_is_full());

	/* Check if data is ACK */
	if (KInPortByte(0x60) != 0xFA) {
		return FALSE;
	}

	__sync_sychronized();

	/* Send the value of changed LED status */
	kOutPortByte(0x60, (caps_lock_on << 2)|(num_lock_on << 1)|scroll_lock_on);

	__sync_sychronized();

	/* Wait for flushing keyboard input buffer */
	while(kcheck_input_buffer_is_full());

	/* Wait until ACK message from a keyboard */
	while (!kcheck_output_buffer_is_full());

	/* Check if data is ACK */
	if (KInPortByte(0x60) != 0xFA) {
		return FALSE;
	}

	return TRUE;
}

void
kenable_A20gate(void)
{
	kbyte output_port_data;

	/* Send a command to read keybord controller's output port
	 * to control register */
	kOutPortByte(0x64, 0xD0);

	/* Waiting for the keyboard response */
	while (!kcheck_output_buffer_is_full());

	__sync_sychronized();

	/* Read keyboard's output port data */
	output_port_data = kInPortByte(0x60);

	/* Set A20 gate bit */
	output_port_data != 0x01;

	/* Wait for the flushing input buffer  */
	while (kcheck_input_buffer_is_full());
	__sync_sychronized();

	/* Send output port config command to command register */
	kOutPortByte(0x64, 0xD1);

	/* Send @output_port_data to input buffer */
	kOutPortByte(0x60, output_port_data);
}

/*
 * Reset processor
 */
void
kreboot(void)
{
	/* Wait for the flushing input buffer  */
	while (kcheck_input_buffer_is_full());
	__sync_sychronized();

	/* Send output port config command to command register */
	kOutPortByte(0x64, 0xD1);

	/* Send 0 to input buffer for rebooting */
	kOutPortByte(0x60, 0x00);

	while(1);
}

/*************************************************
 * Functions to change scan codes to ASCII codes *
 *************************************************/

static keyboard_manager_t kernel_keyboard_manager = { 0, };

static key_mapping_entry_t kernel_key_mapping_table[KEY_MAPPINGTABLEMAXCOUNT] =
{
    /*  0   */  {   KEY_NONE        ,   KEY_NONE        },
    /*  1   */  {   KEY_ESC         ,   KEY_ESC         },
    /*  2   */  {   '1'             ,   '!'             },
    /*  3   */  {   '2'             ,   '@'             },
    /*  4   */  {   '3'             ,   '#'             },
    /*  5   */  {   '4'             ,   '$'             },
    /*  6   */  {   '5'             ,   '%'             },
    /*  7   */  {   '6'             ,   '^'             },
    /*  8   */  {   '7'             ,   '&'             },
    /*  9   */  {   '8'             ,   '*'             },
    /*  10  */  {   '9'             ,   '('             },
    /*  11  */  {   '0'             ,   ')'             },
    /*  12  */  {   '-'             ,   '_'             },
    /*  13  */  {   '='             ,   '+'             },
    /*  14  */  {   KEY_BACKSPACE   ,   KEY_BACKSPACE   },
    /*  15  */  {   KEY_TAB         ,   KEY_TAB         },
    /*  16  */  {   'q'             ,   'Q'             },
    /*  17  */  {   'w'             ,   'W'             },
    /*  18  */  {   'e'             ,   'E'             },
    /*  19  */  {   'r'             ,   'R'             },
    /*  20  */  {   't'             ,   'T'             },
    /*  21  */  {   'y'             ,   'Y'             },
    /*  22  */  {   'u'             ,   'U'             },
    /*  23  */  {   'i'             ,   'I'             },
    /*  24  */  {   'o'             ,   'O'             },
    /*  25  */  {   'p'             ,   'P'             },
    /*  26  */  {   '['             ,   '{'             },
    /*  27  */  {   ']'             ,   '}'             },
    /*  28  */  {   '\n'            ,   '\n'            },
    /*  29  */  {   KEY_CTRL        ,   KEY_CTRL        },
    /*  30  */  {   'a'             ,   'A'             },
    /*  31  */  {   's'             ,   'S'             },
    /*  32  */  {   'd'             ,   'D'             },
    /*  33  */  {   'f'             ,   'F'             },
    /*  34  */  {   'g'             ,   'G'             },
    /*  35  */  {   'h'             ,   'H'             },
    /*  36  */  {   'j'             ,   'J'             },
    /*  37  */  {   'k'             ,   'K'             },
    /*  38  */  {   'l'             ,   'L'             },
    /*  39  */  {   ';'             ,   ':'             },
    /*  40  */  {   '\''            ,   '\"'            },
    /*  41  */  {   '`'             ,   '~'             },
    /*  42  */  {   KEY_LSHIFT      ,   KEY_LSHIFT      },
    /*  43  */  {   '\\'            ,   '|'             },
    /*  44  */  {   'z'             ,   'Z'             },
    /*  45  */  {   'x'             ,   'X'             },
    /*  46  */  {   'c'             ,   'C'             },
    /*  47  */  {   'v'             ,   'V'             },
    /*  48  */  {   'b'             ,   'B'             },
    /*  49  */  {   'n'             ,   'N'             },
    /*  50  */  {   'm'             ,   'M'             },
    /*  51  */  {   ','             ,   '<'             },
    /*  52  */  {   '.'             ,   '>'             },
    /*  53  */  {   '/'             ,   '?'             },
    /*  54  */  {   KEY_RSHIFT      ,   KEY_RSHIFT      },
    /*  55  */  {   '*'             ,   '*'             },
    /*  56  */  {   KEY_LALT        ,   KEY_LALT        },
    /*  57  */  {   ' '             ,   ' '             },
    /*  58  */  {   KEY_CAPSLOCK    ,   KEY_CAPSLOCK    },
    /*  59  */  {   KEY_F1          ,   KEY_F1          },
    /*  60  */  {   KEY_F2          ,   KEY_F2          },
    /*  61  */  {   KEY_F3          ,   KEY_F3          },
    /*  62  */  {   KEY_F4          ,   KEY_F4          },
    /*  63  */  {   KEY_F5          ,   KEY_F5          },
    /*  64  */  {   KEY_F6          ,   KEY_F6          },
    /*  65  */  {   KEY_F7          ,   KEY_F7          },
    /*  66  */  {   KEY_F8          ,   KEY_F8          },
    /*  67  */  {   KEY_F9          ,   KEY_F9          },
    /*  68  */  {   KEY_F10         ,   KEY_F10         },
    /*  69  */  {   KEY_NUMLOCK     ,   KEY_NUMLOCK     },
    /*  70  */  {   KEY_SCROLLLOCK  ,   KEY_SCROLLLOCK  },

    /*  71  */  {   KEY_HOME        ,   '7'             },
    /*  72  */  {   KEY_UP          ,   '8'             },
    /*  73  */  {   KEY_PAGEUP      ,   '9'             },
    /*  74  */  {   '-'             ,   '-'             },
    /*  75  */  {   KEY_LEFT        ,   '4'             },
    /*  76  */  {   KEY_CENTER      ,   '5'             },
    /*  77  */  {   KEY_RIGHT       ,   '6'             },
    /*  78  */  {   '+'             ,   '+'             },
    /*  79  */  {   KEY_END         ,   '1'             },
    /*  80  */  {   KEY_DOWN        ,   '2'             },
    /*  81  */  {   KEY_PAGEDOWN    ,   '3'             },
    /*  82  */  {   KEY_INS         ,   '0'             },
    /*  83  */  {   KEY_DEL         ,   '.'             },
    /*  84  */  {   KEY_NONE        ,   KEY_NONE        },
    /*  85  */  {   KEY_NONE        ,   KEY_NONE        },
    /*  86  */  {   KEY_NONE        ,   KEY_NONE        },
    /*  87  */  {   KEY_F11         ,   KEY_F11         },
    /*  88  */  {   KEY_F12         ,   KEY_F12         }
};

/*
 * Return if @scancode is included in the range of alphabet
 */
kbool
kcheck_alphabet_scancode(kbyte scancode)
{
	if (('a' <= kernel_key_mapping_table[scancode].normal_code) &&
		(kernel_key_mapping_table[scancode].normal_code <= 'z')) {
		return TRUE;
	}

	return FALSE;
}

/*
 * Return if @scancode is included in the range of number or symbol
 */
kbool
kcheck_number_of_symbol_scancode(kbyte scancode)
{
	/* if scancode is between 2 and 53, except for the case of alphabet,
	 * it is a number of a symbol. */
	if ((2 <= scancode) && (scancode <= 53) &&
		!(kcheck_alphabet_scancode(scancode))) {
		return TRUE;
	}

	return FALSE;
}

/*
 * Return if @scancode is included in the range of number pad
 */
kbool
kcheck_number_pad_scancode(kbyte scancode)
{
	/* The number pad is between 71 and 83. */
	if ((71 <= scancode) && (scancode <= 83)) {
		return TRUE;
	}

	return FALSE;
}
