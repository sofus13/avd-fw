#ifndef __REG_V3_H__
#define __REG_V3_H__

#include "reg_common.h"

#undef SP
#undef VP_OFFSET
#undef IRQ_SUBMIT
#undef DECODE_CTRL_BASE

#define SP			0x10010000
#define VP_OFFSET		0x124
#define IRQ_SUBMIT		9

#define DECODE_CTRL_BASE	REG(0x40100000)

#endif /* __REG_V3_H__ */
