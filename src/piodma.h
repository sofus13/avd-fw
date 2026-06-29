#ifndef __PIODMA_H__
#define __PIODMA_H__

#include "util.h"

#define PIODMA_COMMAND(size) (BIT(0) | BIT(4) | ((size) << 6))

int piodma_transfer(unsigned int iova_lo, unsigned int iova_hi,
		unsigned int size);

#endif /* __PIODMA_H__ */
