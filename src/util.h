// SPDX-License-Identifier: MIT

#ifndef __UTIL_H__
#define __UTIL_H__

#define BIT(x)	(1 << x)

typedef volatile unsigned int u32;

u32 reg_read(u32 *reg);
void reg_write(u32 *reg, u32 val);
void reg_clear(u32 *reg);

#endif /* __UTIL_H__ */
