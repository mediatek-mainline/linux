/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __SOC_MEDIATEK_MT6735_MMSYS_H
#define __SOC_MEDIATEK_MT6735_MMSYS_H

#define MT6735_DISP_OVL0_MOUT_EN		0x030
#define MT6735_DISP_OVL0_MOUT_EN_MASK		GENMASK(1, 0)
#define MT6735_DISP_OVL0_MOUT_COLOR		BIT(0)
#define MT6735_DISP_OVL0_MOUT_WDMA		BIT(1)

#define MT6735_DISP_DITHER_MOUT_EN		0x038
#define MT6735_DISP_DITHER_MOUT_EN_MASK		GENMASK(2, 0)
#define MT6735_DISP_DITHER_MOUT_RDMA0		BIT(0)
#define MT6735_DISP_DITHER_MOUT_UFOE		BIT(1)
#define MT6735_DISP_DITHER_MOUT_WDMA		BIT(2)

#define MT6735_DISP_COL0_SEL_IN			0x058
#define MT6735_DISP_COL0_SEL_IN_MASK		GENMASK(0, 0)
#define MT6735_DISP_COL0_IN_RDMA0		0
#define MT6735_DISP_COL0_IN_OVL0		1

#define MT6735_DISP_DSI0_SEL_IN			0x064
#define MT6735_DISP_DSI0_SEL_IN_MASK		GENMASK(1, 0)
#define MT6735_DISP_DSI0_IN_UFOE		0
#define MT6735_DISP_DSI0_IN_RDMA0		1
#define MT6735_DISP_DSI0_IN_RDMA1		2

#define MT6735_DISP_RDMA0_SOUT_SEL_IN		0x06c
#define MT6735_DISP_RDMA0_SOUT_SEL_IN_MASK	GENMASK(1, 0)
#define MT6735_DISP_RDMA0_SOUT_UFOE		0
#define MT6735_DISP_RDMA0_SOUT_COLOR		1
#define MT6735_DISP_RDMA0_SOUT_DSI		2
#define MT6735_DISP_RDMA0_SOUT_DPI		3

#define MT6735_MMSYS_SW0_RST_B			0x140
#define MT6735_MMSYS_LCM_RST_B			0x150

/*
 * Minimal routing table with one route implemented:
 *
 * OVL -> COLOR -> CCORR -> AAL -> GAMMA -> DITHER -> RDMA0 -> DSI
 *
 * MT6735 MMSYS has other routes (namely ones involving MDP blocks and the DPI
 * controller) that are not implemented here.
 */
static const struct mtk_mmsys_routes mt6735_mmsys_routing_table[] = {
	/* OVL -> COLOR */
	{
		.from_comp = DDP_COMPONENT_OVL0,
		.to_comp = DDP_COMPONENT_COLOR0,
		.addr = MT6735_DISP_OVL0_MOUT_EN,
		.mask = MT6735_DISP_OVL0_MOUT_EN_MASK,
		.val = MT6735_DISP_OVL0_MOUT_COLOR,
	}, {
		.from_comp = DDP_COMPONENT_OVL0,
		.to_comp = DDP_COMPONENT_COLOR0,
		.addr = MT6735_DISP_COL0_SEL_IN,
		.mask = MT6735_DISP_COL0_SEL_IN_MASK,
		.val = MT6735_DISP_COL0_IN_OVL0,
	},
	/*
	 * COLOR and DITHER blocks are linked together through a chain of other
	 * blocks with fixed links
	 */
	/* DITHER -> RDMA0 */
	{
		.from_comp = DDP_COMPONENT_DITHER0,
		.to_comp = DDP_COMPONENT_RDMA0,
		.addr = MT6735_DISP_DITHER_MOUT_EN,
		.mask = MT6735_DISP_DITHER_MOUT_EN_MASK,
		.val = MT6735_DISP_DITHER_MOUT_RDMA0
	},
	/* RDMA0 -> DSI */
	{
		.from_comp = DDP_COMPONENT_RDMA0,
		.to_comp = DDP_COMPONENT_DSI0,
		.addr = MT6735_DISP_RDMA0_SOUT_SEL_IN,
		.mask = MT6735_DISP_RDMA0_SOUT_SEL_IN_MASK,
		.val = MT6735_DISP_RDMA0_SOUT_DSI
	}, {
		.from_comp = DDP_COMPONENT_RDMA0,
		.to_comp = DDP_COMPONENT_DSI0,
		.addr = MT6735_DISP_DSI0_SEL_IN,
		.mask = MT6735_DISP_DSI0_SEL_IN_MASK,
		.val = MT6735_DISP_DSI0_IN_RDMA0
	},
};

#endif /* __SOC_MEDIATEK_MT6735_MMSYS_H */
