// SPDX-License-Identifier: MIT

#include "avd.h"

extern void _start(void);

__attribute__((section(".vectors"), used))
static void (*const vector_table[])(void) = {
	(void *)SP,
	_start,
	irq_nmi,
	irq_hardfault,
	irq_memmanage,
	irq_busfault,
	irq_usagefault,
	0,
	0,
	0,
	0,
	irq_svcall,
	0,
	0,
	irq_pendsv,
	irq_systick,

	irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9, irq10, irq11,
	irq12, irq13, irq14, irq15, irq16, irq17, irq18, irq19, irq20, irq21,
	irq22, irq23, irq24, irq25, irq26, irq27, irq28, irq29, irq30, irq31,
	irq32, irq33, irq34, irq35, irq36, irq37, irq38, irq39, irq40, irq41,
	irq42, irq43, irq44, irq45, irq46, irq47, irq48, irq49, irq50, irq51,
	irq52, irq53, irq54, irq55, irq56, irq57, irq58, irq59, irq60, irq61,
	irq62, irq63, irq64, irq65, irq66, irq67, irq68, irq69, irq70, irq71,
	irq72, irq73, irq74, irq75, irq76, irq77, irq78, irq79, irq80, irq81,
	irq82, irq83, irq84, irq85, irq86, irq87, irq88, irq89, irq90, irq91,
	irq92, irq93, irq94, irq95, irq96, irq97, irq98, irq99, irq100, irq101,
	irq102, irq103, irq104, irq105, irq106, irq107, irq108, irq109, irq110,
	irq111, irq112, irq113, irq114, irq115, irq116, irq117, irq118, irq119,
	irq120, irq121, irq122, irq123, irq124, irq125, irq126, irq127, irq128,
	irq129, irq130, irq131, irq132, irq133, irq134, irq135, irq136, irq137,
	irq138, irq139, irq140
};

void apply_tunables()
{
	int n_tunables, i = 0;

	n_tunables = sizeof(avd_tunables) / sizeof(struct tunable);

	for (i = 0; i < n_tunables; i++) {
		u32 val, old_val;

		u32 *reg = REG(DECODE_CTRL_BASE + avd_tunables[i].off);

		old_val = reg_read(reg);
		val = old_val & ~avd_tunables[i].mask;
		val |= avd_tunables[i].val;
		/* always write */
		reg_write(reg, val);
	}
}

void _start(void)
{
	apply_tunables();

	for (int i = 0; i < 7; i++)
		CM3_NVIC_ISER[i] = 0xffffffff;

	__asm volatile("cpsie i");

	reg_write(CM3_BOOT, 1);

	while (1)
		__asm volatile("wfi");
}
