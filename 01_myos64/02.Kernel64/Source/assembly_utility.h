#ifndef __ASSEMBLY_UTILITY_H__
#define __ASSEMBLY_UTILITY_H__

#include "types64.h"

kbyte kInPortByte(kword port);
kbyte kOutPortByte(kword port, kbyte data);

#endif /* __ASSEMBLY_UTILITY_H__ */
