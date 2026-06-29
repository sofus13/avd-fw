#include "avd.h"
#include "hw/reg/reg_common.h"
#include "piodma.h"
#include "memmap.h"

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

void irq1(void)
{
	u32 off_lo;
	u32 off_hi;
	u32 size;
	u32 insn_iova_hi;
	u32 insn_iova_lo;
	/* empty the mailbox, BIT(17) = EMPTY
	 * how many messages can the mailbox store? */
	while ((reg_read(CM3_MBOX0_STATUS) & BIT(17)) == 0) {
		if (reg_read(CM3_ERR)) {
			reg_write(CM3_MBOX_IRQEN_0, 0);
			return;
		}

		/*
		 * Driver does something like:
		 *
		 * piodma.cpu[1 + (transfer_size++)] = instruction;
		 *
		 * I thinks its weird we cant control the destination
		 * might be different for other socs
		 * piodma.cpu[0] = ((((transer_size - 2) * 4) << 16) | ((0x20000 | cm3_offset + 20) & 0x3fffc) | BIT(0));
		 * avd_w32(sram, cm3_offset, piodma.addr & 0xffffffff);
		 * avd_w32(sram, cm3_offset + 4, piodma.addr >> 32);
		 * avd_w32(sram, cm3_offset + 8, transer_size * 4);
		 * avd_w32(sram, cm3_offset + 12, inst.addr & 0xffffffff);
		 * avd_w32(sram, cm3_offset + 16, inst.addr >> 32);
		 * avd_w32(mbox, AVD_REG_MBOX0_SUBMIT, cm3_offset);
		 *
		 */

		u32 off = reg_read(CM3_MBOX0_RX);

		off_lo = reg_read(REG(CM3_SRAM_BASE + off));
		off_hi = reg_read(REG(CM3_SRAM_BASE + off + 4));
		size = reg_read(REG(CM3_SRAM_BASE + off + 8));
		insn_iova_lo = reg_read(REG(CM3_SRAM_BASE + off + 12));
		insn_iova_hi = reg_read(REG(CM3_SRAM_BASE + off + 16));

		piodma_transfer(off_lo, off_hi, size);
		/*
		 * configure stream while we wait for piodma to finish
		 * (hardcoded for t6020 and vp 4)
		 */
		reg_write(REG(DECODE_CTRL_BASE + 0x30c), insn_iova_hi);
		reg_write(REG(DECODE_CTRL_BASE + 0x150), insn_iova_lo);
		reg_write(REG(DECODE_CTRL_BASE + 0x18c), 0);
		reg_write(REG(DECODE_CTRL_BASE + 0x1c8), 0);
		reg_write(REG(DECODE_CTRL_BASE + 0x204), 0);
		reg_write(REG(DECODE_CTRL_BASE + 0x0fc + 4 * 4),
				DECODE_STATUS_UNK | DECODE_STATUS_ERR | DECODE_STATUS_DONE);
		reg_write(REG(DECODE_CTRL_BASE + 0x120),
				DECODE_STATUS_UNK | DECODE_STATUS_DONE);

		while (reg_read(CM3_PIODMA_STATUS) & CM3_PIODMA_STATUS_BUSY);

		/*
		 * write all instructions
		 *
		 * this breaks on multi slice
		 * i think apples solution, is to do a piodma transfer for each slice
		 * and hope the vp is done when the piodma is done??
		 * */
		for (int i = 0; i < (size / 4) - 1; i++)
			reg_write(REG(DECODE_CTRL_BASE + 0xc + (4 * 4)),
					reg_read(REG(CM3_SRAM_BASE + off + 20 + (i * 4))));

	}

	reg_write(CM3_MBOX_IRQCLR_0, CM3_MBOX0_NOT_EMPTY);
}

void _start(void)
{
	apply_tunables();

	for (int i = 0; i < 7; i++)
		CM3_NVIC_ISER[i] = 0xffffffff;

	__asm volatile("cpsie i");

	reg_write(CM3_MBOX_IRQEN_0, CM3_MBOX0_NOT_EMPTY);

	reg_write(CM3_BOOT, 1);

	while (1)
		__asm volatile("wfi");
}
