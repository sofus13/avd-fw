#ifndef __REG_V2_H__
#define __REG_V2_H__

#include "reg_common.h"

#undef SP
#undef VP_OFFSET
#undef IRQ_SUBMIT
#undef DECODE_CTRL_BASE
#undef DECODE_STATUS

#define SP			0x1000c000
#define VP_OFFSET		0x60
#define IRQ_SUBMIT		4

#define DECODE_CTRL_BASE	0x40100000
#define DECODE_STATUS(x)	REG(DECODE_CTRL_BASE + VP_OFFSET)

#endif /* __REG_V2_H__ */
