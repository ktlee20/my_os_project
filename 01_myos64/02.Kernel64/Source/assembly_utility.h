#ifndef __ASSEMBLY_UTILITY_H__
#define __ASSEMBLY_UTILITY_H__

#include "types64.h"

kbyte kInPortByte(kword port);
void kOutPortByte(kword port, kbyte data);
void kLoadGDTR(kqword gdtr_address);
void kLoadTR(kword tss_segment_offset);
void kLoadIDTR(kqword idtr_address);


#endif /* __ASSEMBLY_UTILITY_H__ */
