// SPDX-License-Identifier: MIT

#include "util.h"

u32 reg_read(u32 *reg)
{
	return *reg;
}

void reg_write(u32 *reg, u32 val)
{
	*reg = val;
}
