// SPDX-License-Identifier: MIT

#ifndef __TUNABLES_H__
#define __TUNABLES_H__

#include "tunables_common.h"

#if AVD_VER == 2
#include "tunables_v2.h"
#elif AVD_VER == 3
#include "tunables_v3.h"
#elif AVD_VER == 4
#include "tunables_v4.h"
#elif AVD_VER == 5
#include "tunables_v5.h"
#else
const struct tunable avd_tunables[] = {
    { },
};
#endif

#endif /* __TUNABLES__H__ */
