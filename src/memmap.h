#ifndef __MEMMAP_H__
#define __MEMMAP_H__

#include "util.h"
#include "hw/reg/reg.h"

#define CM3_SRAM_BASE	0x10000000
#define AVD_CMD_BUF	REG(CM3_SRAM_BASE + 0x100)

#define AVD_CMD_BUF	REG(CM3_SRAM_BASE + 0x100)
#define AVD_CMP_BUF	REG(CM3_SRAM_BASE + 0x1000)
#define AVD_LOG_BUF	REG(CM3_SRAM_BASE + 0x4000)

#endif /* __MEMMAP_H__ */
