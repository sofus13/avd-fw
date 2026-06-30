// SPDX-License-Identifier: MIT

#ifndef __TUNABLES_COMMON_H__
#define __TUNABLES_COMMON_H__

#include "../../util.h"

struct tunable {
	u32 off;
	u32 mask;
	u32 val;
};

#endif /* __TUNABLES_COMMON_H__ */
