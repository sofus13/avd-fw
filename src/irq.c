#include "irq.h"

void irq_handler(void)
{
	u32 ipsr;
	u32 submit;
	u32 irq_num;

	__asm volatile("mrs %0, ipsr" : "=r"(ipsr));
	irq_num = ipsr & 0x1ff;

	submit = 0x10000 | (irq_num < 16 ? 1000 + irq_num : irq_num - 16);

	reg_write(CM3_MBOX1_TX, submit);

	while (1)
	__asm volatile("wfi");
}

void irq_nmi(void)
{
	irq_handler();
}

void irq_hardfault(void)
{
	irq_handler();
}

void irq_memmanage(void)
{
	irq_handler();
}

void irq_busfault(void)
{
	irq_handler();
}

void irq_usagefault(void)
{
	irq_handler();
}

void irq_svcall(void)
{
	irq_handler();
}

void irq_pendsv(void)
{
	irq_handler();
}

void irq_systick(void)
{
	irq_handler();
}

void irq_clear(u32 n, u32 status)
{
	volatile u32 *reg = DECODE_STATUS(n);
#if AVD_VER == 2
	/* packed into one register each status is shifted 5 up */
	reg_write(reg, status << (n * 5));
	while (reg_read(reg) & (status << (n * 5)));
#else
	reg_write(reg, status);
	while (reg_read(reg) & status);
#endif
}

static void vpdone(u32 n)
{
	irq_clear(n, DECODE_STATUS_DONE);
	reg_write(CM3_MBOX1_TX, 0x100 | n);
}

static void err(u32 n)
{
	irq_clear(n, DECODE_STATUS_ERR);
	reg_write(CM3_MBOX1_TX, n);
}

static void ppdone(u32 n)
{
	irq_clear(n, DECODE_STATUS_DONE);
	reg_write(CM3_MBOX1_TX, 0x1000);;
}

/* version 2? */
void irq38(void)
{
	irq_clear(IRQ_SUBMIT, DECODE_STATUS_UNK);
}

void irq40(void)
{
	ppdone(IRQ_SUBMIT);
}

/* version 3+ */
void irq62(void)
{
	irq_clear(IRQ_SUBMIT, DECODE_STATUS_UNK);
}

void irq64(void)
{
	ppdone(IRQ_SUBMIT);
}

irq_unk(18, 0)
irq_err(19, 0)
irq_vdone(20, 0)

irq_unk(23, 0)
irq_err(24, 0)
irq_vdone(25, 0)

irq_unk(28, 0)
irq_err(29, 0)
irq_vdone(30, 0)

irq_unk(33, 0)
irq_err(34, 0)
irq_vdone(35, 0)

irq_unk(78, 0)
irq_err(79, 0)
irq_vdone(80, 0)

irq_unk(83, 1)
irq_err(84, 1)
irq_vdone(85, 1)

irq_unk(88, 2)
irq_err(89, 2)
irq_vdone(90, 2)

irq_unk(93, 3)
irq_err(94, 3)
irq_vdone(95, 3)

irq_unk(98, 4)
irq_err(99, 4)
irq_vdone(100, 4)

irq_unk(103, 5)
irq_err(104, 5)
irq_vdone(105, 5)

irq_unk(108, 6)
irq_err(109, 6)
irq_vdone(110, 6)

irq_unk(113, 7)
irq_err(114, 7)
irq_vdone(115, 7)

irq_unk(118, 8)
irq_err(119, 8)
irq_vdone(120, 8)

irq_unk(123, 9)
irq_err(124, 9)
irq_vdone(125, 9)

irq_unk(128, 10)
irq_err(129, 10)
irq_vdone(130, 10)

irq_unk(133, 11)
irq_err(134, 11)
irq_vdone(135, 11)

IRQ(0) IRQ(1) IRQ(2) IRQ(3)

IRQ(4) IRQ(5) IRQ(6) IRQ(7)
IRQ(8) IRQ(9) IRQ(10) IRQ(11)
IRQ(12) IRQ(13) IRQ(14) IRQ(15)
IRQ(16) IRQ(17)

IRQ(21) IRQ(22) IRQ(26) IRQ(27)
IRQ(31) IRQ(32) IRQ(36) IRQ(37)
IRQ(39) IRQ(41)

IRQ(42) IRQ(43) IRQ(44) IRQ(45)
IRQ(46) IRQ(47) IRQ(48) IRQ(49)
IRQ(50) IRQ(51) IRQ(52) IRQ(53)
IRQ(54) IRQ(55) IRQ(56) IRQ(57)
IRQ(58) IRQ(59) IRQ(60) IRQ(61)

IRQ(63) IRQ(65)

IRQ(66) IRQ(67) IRQ(68) IRQ(69)
IRQ(70) IRQ(71) IRQ(72) IRQ(73)
IRQ(74) IRQ(75) IRQ(76) IRQ(77)

IRQ(81) IRQ(82) IRQ(86) IRQ(87)
IRQ(91) IRQ(92) IRQ(96) IRQ(97)
IRQ(101) IRQ(102) IRQ(106) IRQ(107)
IRQ(111) IRQ(112) IRQ(116) IRQ(117)
IRQ(121) IRQ(122)

IRQ(126) IRQ(127) IRQ(131) IRQ(132)
IRQ(136) IRQ(137) IRQ(138) IRQ(139)
IRQ(140)
