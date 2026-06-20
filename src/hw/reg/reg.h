#ifndef __REG_H__
#define __REG_H__

#include "reg_common.h"

#if AVD_VER == 2
#include "reg_v2.h"
#elif AVD_VER == 3
#include "reg_v3.h"
#elif AVD_VER == 5
#include "reg_v5.h"
#endif

#endif /* __REG_H__ */
