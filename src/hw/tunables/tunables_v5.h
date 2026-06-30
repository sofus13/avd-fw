// SPDX-License-Identifier: MIT

#ifndef __TUNABLES_V5__H
#define __TUNABLES_V5__H

#if AVD_TIER == 1
#include "tunables_v5t1.h"
#elif AVD_TIER == 2
#include "tunables_v5t2.h"
#else
#include "tunables_v5t0.h"
#endif

#endif /* __TUNABLES_V3__H */
