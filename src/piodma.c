#include "piodma.h"
#include "util.h"
#include "hw/reg/reg.h"


int piodma_transfer(unsigned int iova_lo, unsigned int iova_hi,
		unsigned int size)
{
	reg_write(CM3_PIODMA_SRC_ADDR_LO, iova_lo);
	reg_write(CM3_PIODMA_SRC_ADDR_HI, iova_hi);

	/* this should be written by the driver, maybe something like
	 * #define PIODMA_BASE_ADDRESS (sram + xxx) */
	reg_write(REG(CM3_PIODMA_BASE + 0x24), 0x28707000);

	/* TODO */
	reg_write(CM3_PIODMA_IRQ_STATUS, 0xffffffff);

	reg_write(CM3_PIODMA_COMMAND, PIODMA_COMMAND(size));

	return 0;
}
