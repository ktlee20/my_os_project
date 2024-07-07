#ifndef __MODE_SWITCH_H__
#define __MODE_SWITCH_H__

#include "types.h"

void kReadCPUID(kdword cpu_id_value, kdword *val_eax, kdword *val_ebx,
				kdword *val_ecx, kdword *val_edx);
void kSwitchAndExecute64bitKernel(void);

#endif /* __MODE_SWITCH_H__ */
