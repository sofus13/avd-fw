#ifndef __REG_COMMON_H__
#define __REG_COMMON_H__

#include "../../util.h"

#define REG(x)			((u32 *)x)

#define SP			0x10012000

#define CM3_NVIC_ISER		REG(0xe000e100)

#define CM3_MBOX_BASE		REG(0x50010000)

/* Registers below found by R: https://github.com/ArcaneNibble/m1n1.git */
#define CM3_MBOX_IRQEN_0	REG(CM3_MBOX_BASE + 0x10)
#define CM3_MBOX_IRQCLR_0	REG(CM3_MBOX_BASE + 0x2c)

#define CM3_MBOX0_STATUS	REG(CM3_MBOX_BASE + 0x50)
#define CM3_MBOX0_TX		REG(CM3_MBOX_BASE + 0x54)
#define CM3_MBOX0_RX		REG(CM3_MBOX_BASE + 0x58)

#define CM3_MBOX1_STATUS	REG(CM3_MBOX_BASE + 0x5c)
#define CM3_MBOX1_TX		REG(CM3_MBOX_BASE + 0x60)
#define CM3_MBOX1_RX		REG(CM3_MBOX_BASE + 0x64)

#define CM3_BOOT		REG(CM3_MBOX_BASE + 0x90)

#define CM3_MBOX0_EMPTY		BIT(0)
#define CM3_MBOX0_NOT_EMPTY	BIT(1)
#define CM3_MBOX1_EMPTY		BIT(2)
#define CM3_MBOX1_NOT_EMPTY	BIT(3)

#define DECODE_STATUS_UNK	BIT(0)
#define DECODE_STATUS_ERR	BIT(1)
#define DECODE_STATUS_DONE	BIT(2)
#define DECODE_STATUS_FULL	BIT(3)

#define VP_OFFSET		0x194
#define IRQ_SUBMIT		13

#define DECODE_CTRL_BASE	REG(0x41100000)
#define DECODE_STATUS(x)	REG(DECODE_CTRL_BASE + VP_OFFSET + (x * 4))

#endif /* __REG_COMMON_H__ */
