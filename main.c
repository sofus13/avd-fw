typedef volatile unsigned int u32;

#define BIT(nr) (1 << (nr))
#define m(addr) ((u32 *)(addr))

#if VERSION == 2
#define SP 0x1000c000
#else
#define SP 0x10010000
#endif

#define NVIC_ISER m(0xe000e100)

#define MBOX_BASE 0x50010000

/* thanks to https://github.com/ArcaneNibble/m1n1.git */
#define MBOX_IRQ_ENABLE_0 m(MBOX_BASE + 0x10)

#define MBOX_IRQ_CLEAR_0 m(MBOX_BASE + 0x2c)

#define MBOX0_STATUS m(MBOX_BASE + 0x50)
#define MBOX0_SUBMIT m(MBOX_BASE + 0x54)
#define MBOX0_RETRIEVE m(MBOX_BASE + 0x58)

#define MBOX1_STATUS m(MBOX_BASE + 0x5c)
#define MBOX1_SUBMIT m(MBOX_BASE + 0x60)
#define MBOX1_RETRIEVE m(MBOX_BASE + 0x64)

/* used to signal boot */
#define MBOX_FLAG_0 m(MBOX_BASE + 0x90)

#define MBOX0_EMPTY BIT(0)
#define MBOX0_NOT_EMPTY BIT(1)

#define MBOX1_EMPTY BIT(2)
#define MBOX1_NOT_EMPTY BIT(3)

#define DECODE_CTRL_BASE 0x40100000

#if VERSION == 2
/* everything is packed into one register */
#define DECODE_STATUS(n) m(DECODE_CTRL_BASE + 0x60)
#else
#define DECODE_STATUS(n) m(DECODE_CTRL_BASE + 0x124 + (n * 4))
#endif

#define DECODE_STATUS_UNK BIT(0)
#define DECODE_STATUS_ERROR BIT(1)
#define DECODE_STATUS_DONE BIT(2)
#define DECODE_STATUS_FULL BIT(3)

void handler(void)
{
  u32 ipsr;
  __asm volatile("mrs %0, ipsr" : "=r"(ipsr));
  u32 irq_num = ipsr & 0x1ff;

  *MBOX1_SUBMIT = 0x10000 | (irq_num < 16 ? 1000 + irq_num : irq_num - 16);

  while (1)
    __asm volatile("wfi");
}

void nmi(void) __attribute__((weak, alias("handler")));
void hardfault(void) __attribute__((weak, alias("handler")));
void memmanage(void) __attribute__((weak, alias("handler")));
void busfault(void) __attribute__((weak, alias("handler")));
void usagefault(void) __attribute__((weak, alias("handler")));
void svcall(void) __attribute__((weak, alias("handler")));
void pendsv(void) __attribute__((weak, alias("handler")));
void systick(void) __attribute__((weak, alias("handler")));

#define IRQ(n) \
	void irq##n(void) __attribute__((weak, alias("handler")));
	IRQ(0) IRQ(1) IRQ(2)

	IRQ(3) IRQ(4) IRQ(5) IRQ(6) IRQ(7) IRQ(8) IRQ(9) IRQ(10)
	IRQ(11) IRQ(12) IRQ(13) IRQ(14) IRQ(15) IRQ(16) IRQ(17)

	/* IRQ(18) IRQ(19) IRQ(20) */ IRQ(21) IRQ(22)
	/* IRQ(23) IRQ(24) IRQ(25) */ IRQ(26) IRQ(27)
	/* IRQ(28) IRQ(29) IRQ(30) */ IRQ(31) IRQ(32)
	/* IRQ(33) IRQ(34) IRQ(35) */ IRQ(36) IRQ(37)
	/* IRQ(38) */ IRQ(39) /* IRQ(40) */ IRQ(41)

	IRQ(42) IRQ(43) IRQ(44) IRQ(45) IRQ(46) IRQ(47)
	IRQ(48) IRQ(49) IRQ(50) IRQ(51) IRQ(52) IRQ(53) IRQ(54)
	IRQ(55) IRQ(56) IRQ(57) IRQ(58) IRQ(59) IRQ(60) IRQ(61)

	/* IRQ( 62) */ IRQ(63) /* IRQ( 64) */ IRQ(65)

	IRQ(66) IRQ(67) IRQ(68) IRQ(69) IRQ(70) IRQ(71)
	IRQ(72) IRQ(73) IRQ(74) IRQ(75) IRQ(76) IRQ(77)

	/* IRQ( 78) IRQ( 79) IRQ( 80) */ IRQ(81) IRQ(82)
	/* IRQ( 83) IRQ( 84) IRQ( 85) */ IRQ(86) IRQ(87)
	/* IRQ( 88) IRQ( 89) IRQ( 90) */ IRQ(91) IRQ(92)
	/* IRQ( 93) IRQ( 94) IRQ( 95) */ IRQ(96) IRQ(97)
	/* IRQ( 98) IRQ( 99) IRQ(100) */ IRQ(101) IRQ(102)
	/* IRQ(103) IRQ(104) IRQ(105) */ IRQ(106) IRQ(107)
	/* IRQ(108) IRQ(109) IRQ(110) */ IRQ(111) IRQ(112)
	/* IRQ(113) IRQ(114) IRQ(115) */ IRQ(116) IRQ(117)
	/* IRQ(118) IRQ(119) IRQ(120) */ IRQ(121) IRQ(122) IRQ(123)
#undef IRQ

void clear(unsigned int n, unsigned int status) {
	volatile u32 *reg = DECODE_STATUS(n);
#if VERSION == 2
	/* packed into one register each status is shifted 5 up */
	*reg = status << (n * 5);
	while (*reg & (status << (n * 5)));
#else
	*reg = status;
	while (*reg & status);
#endif
}


static void vpdone(u32 n)
{
	clear(n, DECODE_STATUS_DONE);
	*MBOX1_SUBMIT = 0x100 | n;
}

static void err(u32 n)
{
	clear(n, DECODE_STATUS_ERROR);
	*MBOX1_SUBMIT = n;
}

static void ppdone(u32 n)
{
	clear(n, DECODE_STATUS_DONE);
	*MBOX1_SUBMIT = 0x1000;
}

#define irq(n) void irq##n(void)

#define unk(n, idx) irq(n) { clear(idx, DECODE_STATUS_UNK); }
#define error(n, idx) irq(n) { err(idx); }
#define vdone(n, idx) irq(n) { vpdone(idx); }

/* version 2? */
void irq38(void) { clear(5, DECODE_STATUS_UNK); }
void irq40(void) { ppdone(5); }

unk(18, 0)
error(19, 0)
vdone(20, 0)

unk(23, 0)
error(24, 0)
vdone(25, 0)

unk(28, 0)
error(29, 0)
vdone(30, 0)

unk(33, 0)
error(34, 0)
vdone(35, 0)

/* version 3 */
void irq62(void) { clear(9, DECODE_STATUS_UNK); }
void irq64(void) { ppdone(9); }

unk(78, 0)
error(79, 0)
vdone(80, 0)

unk(83, 1)
error(84, 1)
vdone(85, 1)

unk(88, 2)
error(89, 2)
vdone(90, 2)

unk(93, 3)
error(94, 3)
vdone(95, 3)

unk(98, 4)
error(99, 4)
vdone(100, 4)

unk(103, 5)
error(104, 5)
vdone(105, 5)

unk(108, 6)
error(109, 6)
vdone(110, 6)

unk(113, 7)
error(114, 7)
vdone(115, 7)

unk(118, 8)
error(119, 8)
vdone(120, 8)

#undef irq
#undef unk
#undef error
#undef vdone

extern void _start(void);

__attribute__((section(".vectors"), used))
static void (*const vector_table[])(void) = {
	(void *)SP,
	_start,
	nmi,
	hardfault,
	memmanage,
	busfault,
	usagefault,
	0,
	0,
	0,
	0,
	svcall,
	0,
	0,
	pendsv,
	systick,

	irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9, irq10, irq11,
	irq12, irq13, irq14, irq15, irq16, irq17, irq18, irq19, irq20, irq21,
	irq22, irq23, irq24, irq25, irq26, irq27, irq28, irq29, irq30, irq31,
	irq32, irq33, irq34, irq35, irq36, irq37, irq38, irq39, irq40, irq41,
	irq42, irq43, irq44, irq45, irq46, irq47, irq48, irq49, irq50, irq51,
	irq52, irq53, irq54, irq55, irq56, irq57, irq58, irq59, irq60, irq61,
	irq62, irq63,
#if VERSION > 2
	irq64, irq65, irq66, irq67, irq68, irq69, irq70, irq71, irq72, irq73,
	irq74, irq75, irq76, irq77, irq78, irq79, irq80, irq81, irq82, irq83,
	irq84, irq85, irq86, irq87, irq88, irq89, irq90, irq91, irq92, irq93,
	irq94, irq95, irq96, irq97, irq98, irq99, irq100, irq101, irq102, irq103,
	irq104, irq105, irq106, irq107, irq108, irq109, irq110, irq111, irq112,
	irq113, irq114, irq115, irq116, irq117, irq118, irq119, irq120, irq121,
	irq122, irq123,
#endif
};

void _start(void)
{
	for (int i = 0; i < 7; i++)
		NVIC_ISER[i] = 0xffffffff;

	__asm volatile("cpsie i");

	*MBOX_FLAG_0 = 1; /* signal boot */

	while (1)
		__asm volatile("wfi");
}
