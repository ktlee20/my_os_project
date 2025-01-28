#ifndef __INTERRUPT_HANDLER_H__
#define __INTERRUPT_HANDLER_H__

#include "types64.h"
/* Functions */
void kcommon_exception_handler(int ivec_number, kqword error_code);
void kcommon_interrupt_handler(int ivec_number);
void kkeyboard_handler(int ivec_number);

#endif /* __INTERRUPT_HANDLER_H__ */
