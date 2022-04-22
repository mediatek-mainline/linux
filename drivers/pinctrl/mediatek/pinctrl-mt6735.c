// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2022 Yassine Oudjana <y.oudjana@protonmail.com>
 */

#include <linux/platform_device.h>
#include <linux/regmap.h>

#include "pinctrl-mtk-mt6735.h"
#include "pinctrl-paris.h"

/* Common registers */
#define GPIO_DIR	0x000
#define GPIO_DOUT	0x100
#define GPIO_DIN	0x200
#define GPIO_MODE1	0x300
#define GPIO_MODE2	0x310
#define GPIO_MODE3	0x320
#define GPIO_MODE4	0x330
#define GPIO_MODE5	0x340
#define GPIO_MODE6	0x350
#define GPIO_MODE7	0x360
#define GPIO_MODE8	0x370
#define GPIO_MODE9	0x380
#define GPIO_MODE10	0x390
#define GPIO_MODE11	0x3a0
#define GPIO_MODE12	0x3b0
#define GPIO_MODE13	0x3c0
#define GPIO_MODE14	0x3d0
#define GPIO_MODE15	0x3e0
#define GPIO_MODE16	0x3f0
#define GPIO_MODE17	0x400
#define GPIO_MODE18	0x410
#define GPIO_MODE19	0x420
#define GPIO_MODE20	0x430
#define GPIO_MODE21	0x440

/* Pin group registers */
#define GPIO_IES	0x000
#define GPIO_SMT	0x010
#define GPIO_TDSEL0	0x020
#define GPIO_TDSEL1	0x024
#define GPIO_RDSEL0	0x028
#define GPIO_RDSEL1	0x02c
#define GPIO_PULLEN0	0x030
#define GPIO_PULLEN1	0x040
#define GPIO_PULLSEL0	0x050
#define GPIO_PULLSEL1	0x060
#define GPIO_DRV0	0x070
#define GPIO_DRV1	0x074
#define GPIO_PUPD0	0x080
#define GPIO_PUPD1	0x090

#define PIN_FIELD_BASE(_s_pin, _e_pin, _i_base, _s_addr, _x_addrs, _s_bit, _x_bits)	\
	PIN_FIELD_CALC(_s_pin, _e_pin, _i_base, _s_addr, _x_addrs, _s_bit,	\
		       _x_bits, 32, 0)

#define PINS_FIELD_BASE(_s_pin, _e_pin, _i_base, _s_addr, _x_addrs, _s_bit, _x_bits)	\
	PIN_FIELD_CALC(_s_pin, _e_pin, _i_base, _s_addr, _x_addrs, _s_bit,	\
		      _x_bits, 32, 1)

static const struct mtk_pin_field_calc mt6735_pin_dir_range[] = {
	PIN_FIELD(0, 203, GPIO_DIR, 0x10, 0, 1)
};

static const struct mtk_pin_field_calc mt6735_pin_di_range[] = {
	PIN_FIELD(0, 203, GPIO_DIN, 0x10, 0, 1)
};

static const struct mtk_pin_field_calc mt6735_pin_do_range[] = {
	PIN_FIELD(0, 203, GPIO_DOUT, 0x10, 0, 1)
};

static const struct mtk_pin_field_calc mt6735_pin_mode_range[] = {
	PIN_FIELD(0, 4, GPIO_MODE1, 0x10, 0, 3),
	PIN_FIELD(5, 9, GPIO_MODE1, 0x10, 16, 3),
	PIN_FIELD(10, 14, GPIO_MODE2, 0x10, 0, 3),
	PIN_FIELD(15, 19, GPIO_MODE2, 0x10, 16, 3),
	PIN_FIELD(20, 24, GPIO_MODE3, 0x10, 0, 3),
	PIN_FIELD(25, 29, GPIO_MODE3, 0x10, 16, 3),
	PIN_FIELD(30, 34, GPIO_MODE4, 0x10, 0, 3),
	PIN_FIELD(35, 39, GPIO_MODE4, 0x10, 16, 3),
	PIN_FIELD(40, 44, GPIO_MODE5, 0x10, 0, 3),
	PIN_FIELD(45, 49, GPIO_MODE5, 0x10, 16, 3),
	PIN_FIELD(50, 54, GPIO_MODE6, 0x10, 0, 3),
	PIN_FIELD(55, 59, GPIO_MODE6, 0x10, 16, 3),
	PIN_FIELD(60, 64, GPIO_MODE7, 0x10, 0, 3),
	PIN_FIELD(65, 69, GPIO_MODE7, 0x10, 16, 3),
	PIN_FIELD(70, 74, GPIO_MODE8, 0x10, 0, 3),
	PIN_FIELD(75, 79, GPIO_MODE8, 0x10, 16, 3),
	PIN_FIELD(80, 84, GPIO_MODE9, 0x10, 0, 3),
	PIN_FIELD(85, 89, GPIO_MODE9, 0x10, 16, 3),
	PIN_FIELD(90, 94, GPIO_MODE10, 0x10, 0, 3),
	PIN_FIELD(95, 99, GPIO_MODE10, 0x10, 16, 3),
	PIN_FIELD(100, 104, GPIO_MODE11, 0x10, 0, 3),
	PIN_FIELD(105, 109, GPIO_MODE11, 0x10, 16, 3),
	PIN_FIELD(110, 114, GPIO_MODE12, 0x10, 0, 3),
	PIN_FIELD(115, 119, GPIO_MODE12, 0x10, 16, 3),
	PIN_FIELD(120, 124, GPIO_MODE13, 0x10, 0, 3),
	PIN_FIELD(125, 129, GPIO_MODE13, 0x10, 16, 3),
	PIN_FIELD(130, 134, GPIO_MODE14, 0x10, 0, 3),
	PIN_FIELD(135, 139, GPIO_MODE14, 0x10, 16, 3),
	PIN_FIELD(140, 144, GPIO_MODE15, 0x10, 0, 3),
	PIN_FIELD(145, 149, GPIO_MODE15, 0x10, 16, 3),
	PIN_FIELD(150, 154, GPIO_MODE16, 0x10, 0, 3),
	PIN_FIELD(155, 159, GPIO_MODE16, 0x10, 16, 3),
	PIN_FIELD(160, 164, GPIO_MODE17, 0x10, 0, 3),
	PIN_FIELD(165, 169, GPIO_MODE17, 0x10, 16, 3),
	PIN_FIELD(170, 174, GPIO_MODE18, 0x10, 0, 3),
	PIN_FIELD(175, 179, GPIO_MODE18, 0x10, 16, 3),
	PIN_FIELD(180, 184, GPIO_MODE19, 0x10, 0, 3),
	PIN_FIELD(185, 189, GPIO_MODE19, 0x10, 16, 3),
	PIN_FIELD(190, 194, GPIO_MODE20, 0x10, 0, 3),
	PIN_FIELD(195, 199, GPIO_MODE20, 0x10, 16, 3),
	PIN_FIELD(200, 203, GPIO_MODE21, 0x10, 0, 3),
};

static const struct mtk_pin_field_calc mt6735_pin_smt_range[] = {
	PINS_FIELD_BASE(0, 4, 2, GPIO_SMT, 0x10, 10, 1),	/* EINT0~4 */
	PINS_FIELD_BASE(5, 8, 3, GPIO_SMT, 0x10, 3, 1),		/* EINT5~8 */
	PINS_FIELD_BASE(9, 12, 3, GPIO_SMT, 0x10, 6, 1),	/* EINT9~12 */
	PINS_FIELD_BASE(13, 18, 1, GPIO_SMT, 0x10, 0, 1),	/* WB_CTRL */
	PINS_FIELD_BASE(19, 21, 1, GPIO_SMT, 0x10, 1, 1),	/* ANTSEL */
	PINS_FIELD_BASE(42, 44, 2, GPIO_SMT, 0x10, 0, 1),	/* CMPDAT */
	PINS_FIELD_BASE(45, 46, 2, GPIO_SMT, 0x10, 1, 1),	/* CMMCLK */
	PINS_FIELD_BASE(47, 48, 2, GPIO_SMT, 0x10, 2, 1),	/* I2C0 */
	PINS_FIELD_BASE(49, 50, 2, GPIO_SMT, 0x10, 3, 1),	/* I2C1 */
	PINS_FIELD_BASE(51, 52, 2, GPIO_SMT, 0x10, 4, 1),	/* I2C2 */
	PINS_FIELD_BASE(53, 54, 2, GPIO_SMT, 0x10, 5, 1),	/* I2C3 */
	PIN_FIELD_BASE(55, 55, 2, GPIO_SMT, 0x10, 6, 1),	/* SRCLKENAI */
	PIN_FIELD_BASE(56, 56, 2, GPIO_SMT, 0x10, 7, 1),	/* SRCLKENA1 */
	PINS_FIELD_BASE(57, 60, 2, GPIO_SMT, 0x10, 8, 1),	/* UART2/3 */
	PINS_FIELD_BASE(61, 64, 2, GPIO_SMT, 0x10, 9, 1),	/* PCM */
	PINS_FIELD_BASE(65, 68, 3, GPIO_SMT, 0x10, 0, 1),	/* SPI */
	PIN_FIELD_BASE(69, 69, 3, GPIO_SMT, 0x10, 1, 1),	/* DISP_PWM */
	PINS_FIELD_BASE(70, 73, 3, GPIO_SMT, 0x10, 2, 1),	/* JTAG */
	PINS_FIELD_BASE(74, 77, 3, GPIO_SMT, 0x10, 4, 1),	/* UART0/1 */
	PINS_FIELD_BASE(78, 80, 3, GPIO_SMT, 0x10, 5, 1),	/* I2S */
	PINS_FIELD_BASE(81, 86, 3, GPIO_SMT, 0x10, 7, 1),	/* KEYPAD */
	PINS_FIELD_BASE(87, 103, 3, GPIO_SMT, 0x10, 8, 1),	/* BPI5~20, C2K_TKBPI */
	PINS_FIELD_BASE(104, 114, 3, GPIO_SMT, 0x10, 9, 1),	/* RFIC0/1 */
	PINS_FIELD_BASE(118, 136, 4, GPIO_SMT, 0x10, 0, 1),	/* LTE_TXBPI, BPI0~4, BPI21~27, PAVM */
	PIN_FIELD_BASE(137, 137, 4, GPIO_SMT, 0x10, 1, 1),	/* RTC32K */
	PINS_FIELD_BASE(138, 142, 4, GPIO_SMT, 0x10, 2, 1),	/* PWRAP */
	PINS_FIELD_BASE(143, 145, 4, GPIO_SMT, 0x10, 3, 1),	/* AUD */
	PINS_FIELD_BASE(146, 147, 4, GPIO_SMT, 0x10, 4, 1),	/* LCM_RST, DSI_TE */
	PIN_FIELD_BASE(148, 148, 4, GPIO_SMT, 0x10, 5, 1),	/* SRCLKENA0 */
	PIN_FIELD_BASE(149, 149, 4, GPIO_SMT, 0x10, 1, 1),	/* WATCHDOG */
	PINS_FIELD_BASE(160, 162, 5, GPIO_SMT, 0x10, 0, 1),	/* SIM2 */
	PINS_FIELD_BASE(163, 165, 4, GPIO_SMT, 0x10, 1, 1),	/* SIM1 */
	PIN_FIELD_BASE(166, 166, 5, GPIO_SMT, 0x10, 2, 1),	/* MSDC1_CMD */
	PIN_FIELD_BASE(167, 167, 5, GPIO_SMT, 0x10, 3, 1),	/* MSDC1_CLK */
	PINS_FIELD_BASE(168, 171, 5, GPIO_SMT, 0x10, 4, 1),	/* MSDC1_DAT */
	PIN_FIELD_BASE(172, 172, 6, GPIO_SMT, 0x10, 0, 1),	/* MSDC0_CMD */
	PIN_FIELD_BASE(173, 173, 6, GPIO_SMT, 0x10, 1, 1),	/* MSDC0_DSL */
	PIN_FIELD_BASE(174, 174, 6, GPIO_SMT, 0x10, 2, 1),	/* MSDC0_CLK */
	PINS_FIELD_BASE(175, 182, 6, GPIO_SMT, 0x10, 3, 1),	/* MSDC0_DAT */
	PIN_FIELD_BASE(183, 183, 6, GPIO_SMT, 0x10, 4, 1),	/* MSDC0_RSTB */
	PINS_FIELD_BASE(184, 185, 6, GPIO_SMT, 0x10, 5, 1),	/* FM */
	PINS_FIELD_BASE(186, 189, 6, GPIO_SMT, 0x10, 6, 1),	/* WB SPI */
	PIN_FIELD_BASE(198, 198, 1, GPIO_SMT, 0x10, 2, 1),	/* MSDC2_CMD */
	PIN_FIELD_BASE(199, 199, 1, GPIO_SMT, 0x10, 3, 1),	/* MSDC2_CLK */
	PINS_FIELD_BASE(200, 203, 1, GPIO_SMT, 0x10, 4, 1)	/* MSDC2_DAT */
};

static const struct mtk_pin_field_calc mt6735_pin_ies_range[] = {
	PINS_FIELD_BASE(0, 4, 2, GPIO_IES, 0x10, 10, 1),	/* EINT0~4 */
	PINS_FIELD_BASE(5, 8, 3, GPIO_IES, 0x10, 3, 1),		/* EINT5~8 */
	PINS_FIELD_BASE(9, 12, 3, GPIO_IES, 0x10, 6, 1),	/* EINT9~12 */
	PINS_FIELD_BASE(13, 18, 1, GPIO_IES, 0x10, 0, 1),	/* WB_CTRL */
	PINS_FIELD_BASE(19, 21, 1, GPIO_IES, 0x10, 1, 1),	/* ANTSEL */
	PINS_FIELD_BASE(42, 44, 2, GPIO_IES, 0x10, 0, 1),	/* CMPDAT */
	PINS_FIELD_BASE(45, 46, 2, GPIO_IES, 0x10, 1, 1),	/* CMMCLK */
	PINS_FIELD_BASE(47, 48, 2, GPIO_IES, 0x10, 2, 1),	/* I2C0 */
	PINS_FIELD_BASE(49, 50, 2, GPIO_IES, 0x10, 3, 1),	/* I2C1 */
	PINS_FIELD_BASE(51, 52, 2, GPIO_IES, 0x10, 4, 1),	/* I2C2 */
	PINS_FIELD_BASE(53, 54, 2, GPIO_IES, 0x10, 5, 1),	/* I2C3 */
	PIN_FIELD_BASE(55, 55, 2, GPIO_IES, 0x10, 6, 1),	/* SRCLKENAI */
	PIN_FIELD_BASE(56, 56, 2, GPIO_IES, 0x10, 7, 1),	/* SRCLKENA1 */
	PINS_FIELD_BASE(57, 60, 2, GPIO_IES, 0x10, 8, 1),	/* UART2/3 */
	PINS_FIELD_BASE(61, 64, 2, GPIO_IES, 0x10, 9, 1),	/* PCM */
	PINS_FIELD_BASE(65, 68, 3, GPIO_IES, 0x10, 0, 1),	/* SPI */
	PIN_FIELD_BASE(69, 69, 3, GPIO_IES, 0x10, 1, 1),	/* DISP_PWM */
	PINS_FIELD_BASE(70, 73, 3, GPIO_IES, 0x10, 2, 1),	/* JTAG */
	PINS_FIELD_BASE(74, 77, 3, GPIO_IES, 0x10, 4, 1),	/* UART0/1 */
	PINS_FIELD_BASE(78, 80, 3, GPIO_IES, 0x10, 5, 1),	/* I2S */
	PINS_FIELD_BASE(81, 86, 3, GPIO_IES, 0x10, 7, 1),	/* KEYPAD */
	PINS_FIELD_BASE(87, 103, 3, GPIO_IES, 0x10, 8, 1),	/* BPI5~20, C2K_TKBPI */
	PINS_FIELD_BASE(104, 114, 3, GPIO_IES, 0x10, 9, 1),	/* RFIC0/1 */
	PINS_FIELD_BASE(118, 136, 4, GPIO_IES, 0x10, 0, 1),	/* LTE_TXBPI, BPI0~4, BPI21~27, PAVM */
	PIN_FIELD_BASE(137, 137, 4, GPIO_IES, 0x10, 1, 1),	/* RTC32K */
	PINS_FIELD_BASE(138, 142, 4, GPIO_IES, 0x10, 2, 1),	/* PWRAP */
	PINS_FIELD_BASE(143, 145, 4, GPIO_IES, 0x10, 3, 1),	/* AUD */
	PINS_FIELD_BASE(146, 147, 4, GPIO_IES, 0x10, 4, 1),	/* LCM_RST, DSI_TE */
	PIN_FIELD_BASE(148, 148, 4, GPIO_IES, 0x10, 5, 1),	/* SRCLKENA0 */
	PIN_FIELD_BASE(149, 149, 4, GPIO_IES, 0x10, 1, 1),	/* WATCHDOG */
	PINS_FIELD_BASE(160, 162, 5, GPIO_IES, 0x10, 0, 1),	/* SIM2 */
	PINS_FIELD_BASE(163, 165, 5, GPIO_IES, 0x10, 1, 1),	/* SIM1 */
	PIN_FIELD_BASE(166, 166, 5, GPIO_IES, 0x10, 2, 1),	/* MSDC1_CMD */
	PIN_FIELD_BASE(167, 167, 5, GPIO_IES, 0x10, 3, 1),	/* MSDC1_CLK */
	PINS_FIELD_BASE(168, 171, 5, GPIO_IES, 0x10, 4, 1),	/* MSDC1_DAT */
	PIN_FIELD_BASE(172, 172, 6, GPIO_IES, 0x10, 0, 1),	/* MSDC0_CMD */
	PIN_FIELD_BASE(173, 173, 6, GPIO_IES, 0x10, 1, 1),	/* MSDC0_DSL */
	PIN_FIELD_BASE(174, 174, 6, GPIO_IES, 0x10, 2, 1),	/* MSDC0_CLK */
	PINS_FIELD_BASE(175, 182, 6, GPIO_IES, 0x10, 3, 1),	/* MSDC0_DAT */
	PIN_FIELD_BASE(183, 183, 6, GPIO_IES, 0x10, 4, 1),	/* MSDC0_RSTB */
	PINS_FIELD_BASE(184, 185, 6, GPIO_IES, 0x10, 5, 1),	/* FM */
	PINS_FIELD_BASE(186, 189, 6, GPIO_IES, 0x10, 6, 1),	/* WB SPI */
	PIN_FIELD_BASE(198, 198, 1, GPIO_IES, 0x10, 2, 1),	/* MSDC2_CMD */
	PIN_FIELD_BASE(199, 199, 1, GPIO_IES, 0x10, 3, 1),	/* MSDC2_CLK */
	PINS_FIELD_BASE(200, 203, 1, GPIO_IES, 0x10, 4, 1)	/* MSDC2_DAT */
};

static const struct mtk_pin_field_calc mt6735_pin_tdsel_range[] = {
	PINS_FIELD_BASE(0, 4, 2, GPIO_TDSEL1, 0x04, 8, 4),	/* EINT0~4 */
	PINS_FIELD_BASE(5, 8, 3, GPIO_TDSEL0, 0x04, 12, 4),	/* EINT5~8 */
	PINS_FIELD_BASE(9, 12, 3, GPIO_TDSEL0, 0x04, 24, 4),	/* EINT9~12 */
	PINS_FIELD_BASE(13, 18, 1, GPIO_TDSEL0, 0x04, 0, 4),	/* WB_CTRL */
	PINS_FIELD_BASE(19, 21, 1, GPIO_TDSEL0, 0x04, 4, 4),	/* ANTSEL */
	PINS_FIELD_BASE(42, 44, 2, GPIO_TDSEL0, 0x04, 0, 4),	/* CMPDAT */
	PINS_FIELD_BASE(45, 46, 2, GPIO_TDSEL0, 0x04, 4, 4),	/* CMMCLK */
	PINS_FIELD_BASE(47, 48, 2, GPIO_TDSEL0, 0x04, 8, 4),	/* I2C0 */
	PINS_FIELD_BASE(49, 50, 2, GPIO_TDSEL0, 0x04, 12, 4),	/* I2C1 */
	PINS_FIELD_BASE(51, 52, 2, GPIO_TDSEL0, 0x04, 16, 4),	/* I2C2 */
	PINS_FIELD_BASE(53, 54, 2, GPIO_TDSEL0, 0x04, 20, 4),	/* I2C3 */
	PIN_FIELD_BASE(55, 55, 2, GPIO_TDSEL0, 0x04, 24, 4),	/* SRCLKENAI */
	PIN_FIELD_BASE(56, 56, 2, GPIO_TDSEL0, 0x04, 28, 4),	/* SRCLKENA1 */
	PINS_FIELD_BASE(57, 60, 2, GPIO_TDSEL1, 0x04, 0, 4),	/* UART2/3 */
	PINS_FIELD_BASE(61, 64, 2, GPIO_TDSEL1, 0x04, 4, 4),	/* PCM */
	PINS_FIELD_BASE(65, 68, 3, GPIO_TDSEL0, 0x04, 0, 4),	/* SPI */
	PIN_FIELD_BASE(69, 69, 3, GPIO_TDSEL0, 0x04, 4, 4),	/* DISP_PWM */
	PINS_FIELD_BASE(70, 73, 3, GPIO_TDSEL0, 0x04, 8, 4),	/* JTAG */
	PINS_FIELD_BASE(74, 77, 3, GPIO_TDSEL0, 0x04, 16, 4),	/* UART0/1 */
	PINS_FIELD_BASE(78, 80, 3, GPIO_TDSEL0, 0x04, 20, 4),	/* I2S */
	PINS_FIELD_BASE(81, 86, 3, GPIO_TDSEL0, 0x04, 28, 4),	/* KEYPAD */
	PINS_FIELD_BASE(87, 103, 3, GPIO_TDSEL1, 0x04, 0, 4),	/* BPI5~20, C2K_TKBPI */
	PINS_FIELD_BASE(104, 114, 3, GPIO_TDSEL1, 0x04, 4, 4),	/* RFIC0/1 */
	PINS_FIELD_BASE(118, 136, 4, GPIO_TDSEL0, 0x04, 0, 4),	/* LTE_TXBPI, BPI0~4, BPI21~27, PAVM */
	PIN_FIELD_BASE(137, 137, 4, GPIO_TDSEL0, 0x04, 4, 4),	/* RTC32K */
	PINS_FIELD_BASE(138, 142, 4, GPIO_TDSEL0, 0x04, 8, 4),	/* PWRAP */
	PINS_FIELD_BASE(143, 145, 4, GPIO_TDSEL0, 0x04, 12, 4),	/* AUD */
	PINS_FIELD_BASE(146, 147, 4, GPIO_TDSEL0, 0x04, 16, 4),	/* LCM_RST, DSI_TE */
	PIN_FIELD_BASE(148, 148, 4, GPIO_TDSEL0, 0x04, 20, 4),	/* SRCLKENA0 */
	PIN_FIELD_BASE(149, 149, 4, GPIO_TDSEL0, 0x04, 4, 4),	/* WATCHDOG */
	PINS_FIELD_BASE(160, 162, 5, GPIO_TDSEL0, 0x04, 0, 4),	/* SIM2 */
	PINS_FIELD_BASE(163, 165, 5, GPIO_TDSEL0, 0x04, 4, 4),	/* SIM1 */
	PIN_FIELD_BASE(166, 166, 5, GPIO_TDSEL0, 0x04, 8, 4),	/* MSDC1_CMD */
	PIN_FIELD_BASE(167, 167, 5, GPIO_TDSEL0, 0x04, 12, 4),	/* MSDC1_CLK */
	PINS_FIELD_BASE(168, 171, 5, GPIO_TDSEL0, 0x04, 16, 4),	/* MSDC1_DAT */
	PIN_FIELD_BASE(172, 172, 6, GPIO_TDSEL0, 0x04, 0, 4),	/* MSDC0_CMD */
	PIN_FIELD_BASE(173, 173, 6, GPIO_TDSEL0, 0x04, 4, 4),	/* MSDC0_DSL */
	PIN_FIELD_BASE(174, 174, 6, GPIO_TDSEL0, 0x04, 8, 4),	/* MSDC0_CLK */
	PINS_FIELD_BASE(175, 182, 6, GPIO_TDSEL0, 0x04, 12, 4),	/* MSDC0_DAT */
	PIN_FIELD_BASE(183, 183, 6, GPIO_TDSEL0, 0x04, 16, 4),	/* MSDC0_RSTB */
	PINS_FIELD_BASE(184, 185, 6, GPIO_TDSEL0, 0x04, 20, 4),	/* FM */
	PINS_FIELD_BASE(186, 189, 6, GPIO_TDSEL0, 0x04, 24, 4),	/* WB SPI */
	PIN_FIELD_BASE(198, 198, 1, GPIO_TDSEL0, 0x04, 8, 4),	/* MSDC2_CMD */
	PIN_FIELD_BASE(199, 199, 1, GPIO_TDSEL0, 0x04, 12, 4),	/* MSDC2_CLK */
	PINS_FIELD_BASE(200, 203, 1, GPIO_TDSEL0, 0x04, 16, 4)	/* MSDC2_DAT */
};

static const struct mtk_pin_field_calc mt6735_pin_rdsel_range[] = {
	PINS_FIELD_BASE(0, 4, 2, GPIO_RDSEL1, 0x04, 8, 4),	/* EINT0~4 */
	PINS_FIELD_BASE(5, 8, 3, GPIO_RDSEL0, 0x04, 12, 4),	/* EINT5~8 */
	PINS_FIELD_BASE(9, 12, 3, GPIO_RDSEL0, 0x04, 24, 4),	/* EINT9~12 */
	PINS_FIELD_BASE(13, 18, 1, GPIO_RDSEL0, 0x04, 0, 4),	/* WB_CTRL */
	PINS_FIELD_BASE(19, 21, 1, GPIO_RDSEL0, 0x04, 4, 4),	/* ANTSEL */
	PINS_FIELD_BASE(42, 44, 2, GPIO_RDSEL0, 0x04, 0, 4),	/* CMPDAT */
	PINS_FIELD_BASE(45, 46, 2, GPIO_RDSEL0, 0x04, 4, 4),	/* CMMCLK */
	PINS_FIELD_BASE(47, 48, 2, GPIO_RDSEL0, 0x04, 8, 4),	/* I2C0 */
	PINS_FIELD_BASE(49, 50, 2, GPIO_RDSEL0, 0x04, 12, 4),	/* I2C1 */
	PINS_FIELD_BASE(51, 52, 2, GPIO_RDSEL0, 0x04, 16, 4),	/* I2C2 */
	PINS_FIELD_BASE(53, 54, 2, GPIO_RDSEL0, 0x04, 20, 4),	/* I2C3 */
	PIN_FIELD_BASE(55, 55, 2, GPIO_RDSEL0, 0x04, 24, 4),	/* SRCLKENAI */
	PIN_FIELD_BASE(56, 56, 2, GPIO_RDSEL0, 0x04, 28, 4),	/* SRCLKENA1 */
	PINS_FIELD_BASE(57, 60, 2, GPIO_RDSEL1, 0x04, 0, 4),	/* UART2/3 */
	PINS_FIELD_BASE(61, 64, 2, GPIO_RDSEL1, 0x04, 4, 4),	/* PCM */
	PINS_FIELD_BASE(65, 68, 3, GPIO_RDSEL0, 0x04, 0, 4),	/* SPI */
	PIN_FIELD_BASE(69, 69, 3, GPIO_RDSEL0, 0x04, 4, 4),	/* DISP_PWM */
	PINS_FIELD_BASE(70, 73, 3, GPIO_RDSEL0, 0x04, 8, 4),	/* JTAG */
	PINS_FIELD_BASE(74, 77, 3, GPIO_RDSEL0, 0x04, 16, 4),	/* UART0/1 */
	PINS_FIELD_BASE(78, 80, 3, GPIO_RDSEL0, 0x04, 20, 4),	/* I2S */
	PINS_FIELD_BASE(81, 86, 3, GPIO_RDSEL0, 0x04, 28, 4),	/* KEYPAD */
	PINS_FIELD_BASE(87, 103, 3, GPIO_RDSEL1, 0x04, 0, 4),	/* BPI5~20, C2K_TKBPI */
	PINS_FIELD_BASE(104, 114, 3, GPIO_RDSEL1, 0x04, 4, 4),	/* RFIC0/1 */
	PINS_FIELD_BASE(118, 136, 4, GPIO_RDSEL0, 0x04, 0, 4),	/* LTE_TXBPI, BPI0~4, BPI21~27, PAVM */
	PIN_FIELD_BASE(137, 137, 4, GPIO_RDSEL0, 0x04, 4, 4),	/* RTC32K */
	PINS_FIELD_BASE(138, 142, 4, GPIO_RDSEL0, 0x04, 8, 4),	/* PWRAP */
	PINS_FIELD_BASE(143, 145, 4, GPIO_RDSEL0, 0x04, 12, 4),	/* AUD */
	PINS_FIELD_BASE(146, 147, 4, GPIO_RDSEL0, 0x04, 16, 4),	/* LCM_RST, DSI_TE */
	PIN_FIELD_BASE(148, 148, 4, GPIO_RDSEL0, 0x04, 20, 4),	/* SRCLKENA0 */
	PIN_FIELD_BASE(149, 149, 4, GPIO_RDSEL0, 0x04, 4, 4),	/* WATCHDOG */
	PINS_FIELD_BASE(160, 162, 5, GPIO_RDSEL0, 0x04, 0, 4),	/* SIM2 */
	PINS_FIELD_BASE(163, 165, 5, GPIO_RDSEL0, 0x04, 4, 4),	/* SIM1 */
	PIN_FIELD_BASE(166, 166, 5, GPIO_RDSEL0, 0x04, 8, 4),	/* MSDC1_CMD */
	PIN_FIELD_BASE(167, 167, 5, GPIO_RDSEL0, 0x04, 12, 4),	/* MSDC1_CLK */
	PINS_FIELD_BASE(168, 171, 5, GPIO_RDSEL0, 0x04, 16, 4),	/* MSDC1_DAT */
	PIN_FIELD_BASE(172, 172, 6, GPIO_RDSEL0, 0x04, 0, 4),	/* MSDC0_CMD */
	PIN_FIELD_BASE(173, 173, 6, GPIO_RDSEL0, 0x04, 4, 4),	/* MSDC0_DSL */
	PIN_FIELD_BASE(174, 174, 6, GPIO_RDSEL0, 0x04, 8, 4),	/* MSDC0_CLK */
	PINS_FIELD_BASE(175, 182, 6, GPIO_RDSEL0, 0x04, 12, 4),	/* MSDC0_DAT */
	PIN_FIELD_BASE(183, 183, 6, GPIO_RDSEL0, 0x04, 16, 4),	/* MSDC0_RSTB */
	PINS_FIELD_BASE(184, 185, 6, GPIO_RDSEL0, 0x04, 20, 4),	/* FM */
	PINS_FIELD_BASE(186, 189, 6, GPIO_RDSEL0, 0x04, 24, 4),	/* WB SPI */
	PIN_FIELD_BASE(198, 198, 1, GPIO_RDSEL0, 0x04, 8, 4),	/* MSDC2_CMD */
	PIN_FIELD_BASE(199, 199, 1, GPIO_RDSEL0, 0x04, 12, 4),	/* MSDC2_CLK */
	PINS_FIELD_BASE(200, 203, 1, GPIO_RDSEL0, 0x04, 16, 4)	/* MSDC2_DAT */
};

static const struct mtk_pin_field_calc mt6735_pin_drv_range[] = {
	PINS_FIELD_BASE(0, 4, 2, GPIO_DRV1, 0x04, 8, 2),	/* EINT0~4 */
	PINS_FIELD_BASE(5, 8, 3, GPIO_DRV0, 0x04, 12, 2),	/* EINT5~8 */
	PINS_FIELD_BASE(9, 12, 3, GPIO_DRV0, 0x04, 24, 2),	/* EINT9~12 */
	PINS_FIELD_BASE(13, 18, 1, GPIO_DRV0, 0x04, 0, 2),	/* WB_CTRL */
	PINS_FIELD_BASE(19, 21, 1, GPIO_DRV0, 0x04, 4, 2),	/* ANTSEL */
	PINS_FIELD_BASE(42, 44, 2, GPIO_DRV0, 0x04, 0, 2),	/* CMPDAT */
	PINS_FIELD_BASE(45, 46, 2, GPIO_DRV0, 0x04, 4, 3),	/* CMMCLK */
	PINS_FIELD_BASE(47, 48, 2, GPIO_DRV0, 0x04, 8, 1),	/* I2C0 */
	PINS_FIELD_BASE(49, 50, 2, GPIO_DRV0, 0x04, 12, 1),	/* I2C1 */
	PINS_FIELD_BASE(51, 52, 2, GPIO_DRV0, 0x04, 16, 1),	/* I2C2 */
	PINS_FIELD_BASE(53, 54, 2, GPIO_DRV0, 0x04, 20, 1),	/* I2C3 */
	PIN_FIELD_BASE(55, 55, 2, GPIO_DRV0, 0x04, 24, 2),	/* SRCLKENAI */
	PIN_FIELD_BASE(56, 56, 2, GPIO_DRV0, 0x04, 28, 2),	/* SRCLKENA1 */
	PINS_FIELD_BASE(57, 60, 2, GPIO_DRV1, 0x04, 0, 2),	/* UART2/3 */
	PINS_FIELD_BASE(61, 64, 2, GPIO_DRV1, 0x04, 4, 2),	/* PCM */
	PINS_FIELD_BASE(65, 68, 3, GPIO_DRV0, 0x04, 0, 2),	/* SPI */
	PIN_FIELD_BASE(69, 69, 3, GPIO_DRV0, 0x04, 4, 2),	/* DISP_PWM */
	PINS_FIELD_BASE(70, 73, 3, GPIO_DRV0, 0x04, 8, 2),	/* JTAG */
	PINS_FIELD_BASE(74, 77, 3, GPIO_DRV0, 0x04, 16, 2),	/* UART0/1 */
	PINS_FIELD_BASE(78, 80, 3, GPIO_DRV0, 0x04, 20, 2),	/* I2S */
	PINS_FIELD_BASE(81, 86, 3, GPIO_DRV0, 0x04, 28, 2),	/* KEYPAD */
	PINS_FIELD_BASE(87, 103, 3, GPIO_DRV1, 0x04, 0, 2),	/* BPI5~20, C2K_TKBPI */
	PINS_FIELD_BASE(104, 114, 3, GPIO_DRV1, 0x04, 4, 2),	/* RFIC0/1 */
	PINS_FIELD_BASE(118, 136, 4, GPIO_DRV0, 0x04, 0, 2),	/* LTE_TXBPI, BPI0~4, BPI21~27, PAVM */
	PIN_FIELD_BASE(137, 137, 4, GPIO_DRV0, 0x04, 4, 2),	/* RTC32K */
	PINS_FIELD_BASE(138, 142, 4, GPIO_DRV0, 0x04, 8, 2),	/* PWRAP */
	PINS_FIELD_BASE(143, 145, 4, GPIO_DRV0, 0x04, 12, 2),	/* AUD */
	PINS_FIELD_BASE(146, 147, 4, GPIO_DRV0, 0x04, 16, 2),	/* LCM_RST, DSI_TE */
	PIN_FIELD_BASE(148, 148, 4, GPIO_DRV0, 0x04, 20, 2),	/* SRCLKENA0 */
	PIN_FIELD_BASE(149, 149, 4, GPIO_DRV0, 0x04, 4, 2),	/* WATCHDOG */
	PINS_FIELD_BASE(160, 162, 5, GPIO_DRV0, 0x04, 0, 2),	/* SIM2 */
	PINS_FIELD_BASE(163, 165, 5, GPIO_DRV0, 0x04, 4, 2),	/* SIM1 */
	PIN_FIELD_BASE(166, 166, 5, GPIO_DRV0, 0x04, 8, 3),	/* MSDC1_CMD */
	PIN_FIELD_BASE(167, 167, 5, GPIO_DRV0, 0x04, 12, 3),	/* MSDC1_CLK */
	PINS_FIELD_BASE(168, 171, 5, GPIO_DRV0, 0x04, 16, 3),	/* MSDC1_DAT */
	PIN_FIELD_BASE(172, 172, 6, GPIO_DRV0, 0x04, 0, 3),	/* MSDC0_CMD */
	PIN_FIELD_BASE(173, 173, 6, GPIO_DRV0, 0x04, 4, 3),	/* MSDC0_DSL */
	PIN_FIELD_BASE(174, 174, 6, GPIO_DRV0, 0x04, 8, 3),	/* MSDC0_CLK */
	PINS_FIELD_BASE(175, 182, 6, GPIO_DRV0, 0x04, 12, 3),	/* MSDC0_DAT */
	PIN_FIELD_BASE(183, 183, 6, GPIO_DRV0, 0x04, 16, 3),	/* MSDC0_RSTB */
	PINS_FIELD_BASE(184, 185, 6, GPIO_DRV0, 0x04, 20, 2),	/* FM */
	PINS_FIELD_BASE(186, 189, 6, GPIO_DRV0, 0x04, 24, 2),	/* WB SPI */
	PIN_FIELD_BASE(198, 198, 1, GPIO_DRV0, 0x04, 8, 3),	/* MSDC2_CMD */
	PIN_FIELD_BASE(199, 199, 1, GPIO_DRV0, 0x04, 12, 3),	/* MSDC2_CLK */
	PINS_FIELD_BASE(200, 203, 1, GPIO_DRV0, 0x04, 16, 3)	/* MSDC2_DAT */
};

static const struct mtk_pin_field_calc mt6735_pin_r0_range[] = {
	PIN_FIELD_BASE(45, 46, 2, GPIO_PUPD0, 0x10, 0, 4),	/* CMMCLK */
	PIN_FIELD_BASE(81, 86, 3, GPIO_PUPD0, 0x10, 0, 4),	/* KEYPAD */
	PIN_FIELD_BASE(160, 162, 5, GPIO_PUPD1, 0x10, 0, 4),	/* SIM2 */
	PIN_FIELD_BASE(163, 165, 5, GPIO_PUPD1, 0x10, 16, 4),	/* SIM1 */
	PIN_FIELD_BASE(166, 171, 5, GPIO_PUPD0, 0x10, 0, 4),	/* MSDC1 */
	PIN_FIELD_BASE(172, 183, 6, GPIO_PUPD0, 0x10, 0, 4),	/* MSDC0 */
	PIN_FIELD_BASE(198, 203, 1, GPIO_PUPD0, 0x10, 0, 4)	/* MSDC2 */
};

static const struct mtk_pin_field_calc mt6735_pin_r1_range[] = {
	PIN_FIELD_BASE(45, 46, 2, GPIO_PUPD0, 0x10, 1, 4),	/* CMMCLK */
	PIN_FIELD_BASE(81, 86, 3,  GPIO_PUPD0, 0x10, 1, 4),	/* KEYPAD */
	PIN_FIELD_BASE(160, 162, 5, GPIO_PUPD1, 0x10, 1, 4),	/* SIM2 */
	PIN_FIELD_BASE(163, 165, 5, GPIO_PUPD1, 0x10, 17, 4),	/* SIM1 */
	PIN_FIELD_BASE(166, 171, 5, GPIO_PUPD0, 0x10, 1, 4),	/* MSDC1 */
	PIN_FIELD_BASE(172, 183, 6, GPIO_PUPD0, 0x10, 1, 4),	/* MSDC0 */
	PIN_FIELD_BASE(198, 203, 1, GPIO_PUPD0, 0x10, 1, 4)	/* MSDC2 */
};

static const struct mtk_pin_field_calc mt6735_pin_pupd_range[] = {
	PIN_FIELD_BASE(45, 46, 2, GPIO_PUPD0, 0x10, 2, 4),	/* CMMCLK */
	PIN_FIELD_BASE(81, 86, 3, GPIO_PUPD0, 0x10, 2, 4),	/* KEYPAD */
	PIN_FIELD_BASE(160, 162, 5, GPIO_PUPD1, 0x10, 2, 4),	/* SIM2 */
	PIN_FIELD_BASE(163, 165, 5, GPIO_PUPD1, 0x10, 18, 4),	/* SIM1 */
	PIN_FIELD_BASE(166, 171, 5, GPIO_PUPD0, 0x10, 2, 4),	/* MSDC1 */
	PIN_FIELD_BASE(172, 183, 6, GPIO_PUPD0, 0x10, 2, 4),	/* MSDC0 */
	PIN_FIELD_BASE(198, 203, 1, GPIO_PUPD0, 0x10, 2, 4)	/* MSDC2 */
};

static const struct mtk_pin_field_calc mt6735_pin_pullen_range[] = {
	PIN_FIELD_BASE(0, 4, 2, GPIO_PULLEN0, 0x10, 23, 1),
	PIN_FIELD_BASE(5, 8, 3, GPIO_PULLEN0, 0x10, 9, 1),
	PIN_FIELD_BASE(9, 12, 3, GPIO_PULLEN0, 0x10, 20, 1),
	PIN_FIELD_BASE(13, 21, 1, GPIO_PULLEN0, 0x10, 0, 1),
	PIN_FIELD_BASE(42, 44, 2, GPIO_PULLEN0, 0x10, 0, 1),
	PIN_FIELD_BASE(47, 64, 2, GPIO_PULLEN0, 0x10, 5, 1),
	PIN_FIELD_BASE(65, 73, 3, GPIO_PULLEN0, 0x10, 0, 1),
	PIN_FIELD_BASE(74, 80, 3, GPIO_PULLEN0, 0x10, 13, 1),
	PIN_FIELD_BASE(87, 114, 3, GPIO_PULLEN0, 0x10, 30, 1),
	PIN_FIELD_BASE(118, 149, 4, GPIO_PULLEN0, 0x10, 0, 1),
	PIN_FIELD_BASE(184, 189, 6, GPIO_PULLEN0, 0x10, 12, 1)
};

static const struct mtk_pin_field_calc mt6735_pin_pullsel_range[] = {
	PIN_FIELD_BASE(0, 4, 2, GPIO_PULLSEL0, 0x10, 23, 1),
	PIN_FIELD_BASE(5, 8, 3, GPIO_PULLSEL0, 0x10, 9, 1),
	PIN_FIELD_BASE(9, 12, 3, GPIO_PULLSEL0, 0x10, 20, 1),
	PIN_FIELD_BASE(13, 21, 1, GPIO_PULLSEL0, 0x10, 0, 1),
	PIN_FIELD_BASE(42, 44, 2, GPIO_PULLSEL0, 0x10, 0, 1),
	PIN_FIELD_BASE(47, 64, 2, GPIO_PULLSEL0, 0x10, 5, 1),
	PIN_FIELD_BASE(65, 73, 3, GPIO_PULLSEL0, 0x10, 0, 1),
	PIN_FIELD_BASE(74, 80, 3, GPIO_PULLSEL0, 0x10, 13, 1),
	PIN_FIELD_BASE(87, 114, 3, GPIO_PULLSEL0, 0x10, 30, 1),
	PIN_FIELD_BASE(118, 149, 4, GPIO_PULLSEL0, 0x10, 0, 1),
	PIN_FIELD_BASE(184, 189, 6, GPIO_PULLSEL0, 0x10, 12, 1)
};

static const struct mtk_pin_reg_calc mt6735_reg_cals[] = {
	[PINCTRL_PIN_REG_DIR] = MTK_RANGE(mt6735_pin_dir_range),
	[PINCTRL_PIN_REG_DI] = MTK_RANGE(mt6735_pin_di_range),
	[PINCTRL_PIN_REG_DO] = MTK_RANGE(mt6735_pin_do_range),
	[PINCTRL_PIN_REG_MODE] = MTK_RANGE(mt6735_pin_mode_range),
	[PINCTRL_PIN_REG_SMT] = MTK_RANGE(mt6735_pin_smt_range),
	[PINCTRL_PIN_REG_IES] = MTK_RANGE(mt6735_pin_ies_range),
	[PINCTRL_PIN_REG_TDSEL] = MTK_RANGE(mt6735_pin_tdsel_range),
	[PINCTRL_PIN_REG_RDSEL] = MTK_RANGE(mt6735_pin_rdsel_range),
	[PINCTRL_PIN_REG_DRV] = MTK_RANGE(mt6735_pin_drv_range),
	[PINCTRL_PIN_REG_R0] = MTK_RANGE(mt6735_pin_r0_range),
	[PINCTRL_PIN_REG_R1] = MTK_RANGE(mt6735_pin_r1_range),
	[PINCTRL_PIN_REG_PUPD] = MTK_RANGE(mt6735_pin_pupd_range),
	[PINCTRL_PIN_REG_PULLEN] = MTK_RANGE(mt6735_pin_pullen_range),
	[PINCTRL_PIN_REG_PULLSEL] = MTK_RANGE(mt6735_pin_pullsel_range),
};

static const struct mtk_pin_field_calc mt6735m_pin_dir_range[] = {
	PIN_FIELD(0, 197, GPIO_DIR, 0x10, 0, 1)
};

static const struct mtk_pin_field_calc mt6735m_pin_di_range[] = {
	PIN_FIELD(0, 197, GPIO_DIN, 0x10, 0, 1)
};

static const struct mtk_pin_field_calc mt6735m_pin_do_range[] = {
	PIN_FIELD(0, 197, GPIO_DOUT, 0x10, 0, 1)
};

static const struct mtk_pin_field_calc mt6735m_pin_mode_range[] = {
	PIN_FIELD(0, 4, GPIO_MODE1, 0x10, 0, 3),
	PIN_FIELD(5, 9, GPIO_MODE1, 0x10, 16, 3),
	PIN_FIELD(10, 14, GPIO_MODE2, 0x10, 0, 3),
	PIN_FIELD(15, 19, GPIO_MODE2, 0x10, 16, 3),
	PIN_FIELD(20, 24, GPIO_MODE3, 0x10, 0, 3),
	PIN_FIELD(25, 29, GPIO_MODE3, 0x10, 16, 3),
	PIN_FIELD(30, 34, GPIO_MODE4, 0x10, 0, 3),
	PIN_FIELD(35, 39, GPIO_MODE4, 0x10, 16, 3),
	PIN_FIELD(40, 44, GPIO_MODE5, 0x10, 0, 3),
	PIN_FIELD(45, 49, GPIO_MODE5, 0x10, 16, 3),
	PIN_FIELD(50, 54, GPIO_MODE6, 0x10, 0, 3),
	PIN_FIELD(55, 59, GPIO_MODE6, 0x10, 16, 3),
	PIN_FIELD(60, 64, GPIO_MODE7, 0x10, 0, 3),
	PIN_FIELD(65, 69, GPIO_MODE7, 0x10, 16, 3),
	PIN_FIELD(70, 74, GPIO_MODE8, 0x10, 0, 3),
	PIN_FIELD(75, 79, GPIO_MODE8, 0x10, 16, 3),
	PIN_FIELD(80, 84, GPIO_MODE9, 0x10, 0, 3),
	PIN_FIELD(85, 89, GPIO_MODE9, 0x10, 16, 3),
	PIN_FIELD(90, 94, GPIO_MODE10, 0x10, 0, 3),
	PIN_FIELD(95, 99, GPIO_MODE10, 0x10, 16, 3),
	PIN_FIELD(100, 104, GPIO_MODE11, 0x10, 0, 3),
	PIN_FIELD(105, 109, GPIO_MODE11, 0x10, 16, 3),
	PIN_FIELD(110, 114, GPIO_MODE12, 0x10, 0, 3),
	PIN_FIELD(115, 119, GPIO_MODE12, 0x10, 16, 3),
	PIN_FIELD(120, 124, GPIO_MODE13, 0x10, 0, 3),
	PIN_FIELD(125, 129, GPIO_MODE13, 0x10, 16, 3),
	PIN_FIELD(130, 134, GPIO_MODE14, 0x10, 0, 3),
	PIN_FIELD(135, 139, GPIO_MODE14, 0x10, 16, 3),
	PIN_FIELD(140, 144, GPIO_MODE15, 0x10, 0, 3),
	PIN_FIELD(145, 149, GPIO_MODE15, 0x10, 16, 3),
	PIN_FIELD(150, 154, GPIO_MODE16, 0x10, 0, 3),
	PIN_FIELD(155, 159, GPIO_MODE16, 0x10, 16, 3),
	PIN_FIELD(160, 164, GPIO_MODE17, 0x10, 0, 3),
	PIN_FIELD(165, 169, GPIO_MODE17, 0x10, 16, 3),
	PIN_FIELD(170, 174, GPIO_MODE18, 0x10, 0, 3),
	PIN_FIELD(175, 179, GPIO_MODE18, 0x10, 16, 3),
	PIN_FIELD(180, 184, GPIO_MODE19, 0x10, 0, 3),
	PIN_FIELD(185, 189, GPIO_MODE19, 0x10, 16, 3),
	PIN_FIELD(190, 194, GPIO_MODE20, 0x10, 0, 3),
	PIN_FIELD(195, 197, GPIO_MODE20, 0x10, 16, 3),
};

/*
 * Rather than redefining all tables for MT6735M just to remove the 6 MSDC2
 * pins at the end, reuse the MT6735 tables and exclude the MSDC2 pin fields
 * by subtracting their count from the array size when possible
 */
static const struct mtk_pin_reg_calc mt6735m_reg_cals[] = {
	[PINCTRL_PIN_REG_DIR] = MTK_RANGE(mt6735m_pin_dir_range),
	[PINCTRL_PIN_REG_DI] = MTK_RANGE(mt6735m_pin_di_range),
	[PINCTRL_PIN_REG_DO] = MTK_RANGE(mt6735m_pin_do_range),
	[PINCTRL_PIN_REG_MODE] = MTK_RANGE(mt6735m_pin_mode_range),
	[PINCTRL_PIN_REG_SMT] = { mt6735_pin_smt_range,
		ARRAY_SIZE(mt6735_pin_smt_range) - 3 },
	[PINCTRL_PIN_REG_IES] = { mt6735_pin_ies_range,
		ARRAY_SIZE(mt6735_pin_ies_range) - 3 },
	[PINCTRL_PIN_REG_TDSEL] = { mt6735_pin_tdsel_range,
		ARRAY_SIZE(mt6735_pin_tdsel_range) - 3 },
	[PINCTRL_PIN_REG_RDSEL] = { mt6735_pin_rdsel_range,
		ARRAY_SIZE(mt6735_pin_rdsel_range) - 3 },
	[PINCTRL_PIN_REG_DRV] = { mt6735_pin_drv_range,
		ARRAY_SIZE(mt6735_pin_drv_range) - 3 },
	[PINCTRL_PIN_REG_R0] = { mt6735_pin_r0_range,
		ARRAY_SIZE(mt6735_pin_r0_range) - 1 },
	[PINCTRL_PIN_REG_R1] = { mt6735_pin_r1_range,
		ARRAY_SIZE(mt6735_pin_r1_range) - 1 },
	[PINCTRL_PIN_REG_PUPD] = { mt6735_pin_pupd_range,
		ARRAY_SIZE(mt6735_pin_pupd_range) - 1 },
	[PINCTRL_PIN_REG_PULLEN] = MTK_RANGE(mt6735_pin_pullen_range),
	[PINCTRL_PIN_REG_PULLSEL] = MTK_RANGE(mt6735_pin_pullsel_range),
};

static const char * const mt6735_pinctrl_register_base_names[] = {
	"gpio", "iocfg0", "iocfg1", "iocfg2", "iocfg3", "iocfg4", "iocfg5"
};

static const struct mtk_eint_hw mt6735_eint_hw = {
	.port_mask = 7,
	.ports     = 6,
	.ap_num    = 224,
	.db_cnt    = 16,
};

static const struct mtk_pin_soc mt6735_data = {
	.reg_cal = mt6735_reg_cals,
	.pins = mtk_pins_mt6735,
	.npins = ARRAY_SIZE(mtk_pins_mt6735),
	.ngrps = ARRAY_SIZE(mtk_pins_mt6735),
	.eint_hw = &mt6735_eint_hw,
	.gpio_m = 0,
	.ies_present = true,
	.base_names = mt6735_pinctrl_register_base_names,
	.nbase_names = ARRAY_SIZE(mt6735_pinctrl_register_base_names),
	.bias_set_combo = mtk_pinconf_bias_set_combo,
	.bias_get_combo = mtk_pinconf_bias_get_combo,
	.drive_set = mtk_pinconf_drive_set_raw,
	.drive_get = mtk_pinconf_drive_get_raw,
	.adv_pull_get = mtk_pinconf_adv_pull_get,
	.adv_pull_set = mtk_pinconf_adv_pull_set,
};

static const struct mtk_pin_soc mt6735m_data = {
	.reg_cal = mt6735m_reg_cals,
	.pins = mtk_pins_mt6735m,
	.npins = ARRAY_SIZE(mtk_pins_mt6735m),
	.ngrps = ARRAY_SIZE(mtk_pins_mt6735m),
	.eint_hw = &mt6735_eint_hw,
	.gpio_m = 0,
	.ies_present = true,
	.base_names = mt6735_pinctrl_register_base_names,
	.nbase_names = ARRAY_SIZE(mt6735_pinctrl_register_base_names),
	.bias_set_combo = mtk_pinconf_bias_set_combo,
	.bias_get_combo = mtk_pinconf_bias_get_combo,
	.drive_set = mtk_pinconf_drive_set_raw,
	.drive_get = mtk_pinconf_drive_get_raw,
	.adv_pull_get = mtk_pinconf_adv_pull_get,
	.adv_pull_set = mtk_pinconf_adv_pull_set,
};

static const struct of_device_id mt6735_pinctrl_match[] = {
	{ .compatible = "mediatek,mt6735-pinctrl", .data = &mt6735_data },
	{ .compatible = "mediatek,mt6735m-pinctrl", .data = &mt6735m_data },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, mt6735_pinctrl_match);

static struct platform_driver mt6735_pinctrl_driver = {
	.probe = mtk_paris_pinctrl_probe,
	.driver = {
		.name = "mt6735-pinctrl",
		.of_match_table = mt6735_pinctrl_match,
		.pm = &mtk_paris_pinctrl_pm_ops,
	},
};
module_platform_driver(mt6735_pinctrl_driver);

MODULE_AUTHOR("Yassine Oudjana <y.oudjana@protonmail.com>");
MODULE_DESCRIPTION("MediaTek MT6735 pinctrl driver");
MODULE_LICENSE("GPL");
