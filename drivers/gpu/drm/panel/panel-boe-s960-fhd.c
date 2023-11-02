// SPDX-License-Identifier: GPL-2.0+
// Copyright (c) 2023 Maxim Kutnij <gtk3@inbox.ru>

#include <linux/backlight.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/regulator/consumer.h>

#include <video/mipi_display.h>

#include <drm/drm_mipi_dsi.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>

struct boe_s960_fhd {
	struct drm_panel panel;
	struct mipi_dsi_device *dsi;
	struct regulator_bulk_data supplies[2];
	struct gpio_desc *reset_gpio;
	bool prepared;
};

static inline
struct boe_s960_fhd *to_boe_s960_fhd(struct drm_panel *panel)
{
	return container_of(panel, struct boe_s960_fhd, panel);
}

#define dsi_dcs_write_seq(dsi, seq...) do {				\
		static const u8 d[] = { seq };				\
		int ret;						\
		ret = mipi_dsi_dcs_write_buffer(dsi, d, ARRAY_SIZE(d));	\
		if (ret < 0)						\
			return ret;					\
	} while (0)

static void boe_s960_fhd_reset(struct boe_s960_fhd *ctx)
{
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	usleep_range(10000, 11000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	usleep_range(5000, 6000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	msleep(30);
}

static int boe_s960_fhd_on(struct boe_s960_fhd *ctx)
{
	struct mipi_dsi_device *dsi = ctx->dsi;
	struct device *dev = &dsi->dev;
	int ret;

	dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	dsi_dcs_write_seq(dsi, 0xf0, 0x5a, 0x5a);
	dsi_dcs_write_seq(dsi, 0xf1, 0x5a, 0x5a);
	dsi_dcs_write_seq(dsi, 0xfc, 0xa5, 0xa5);

	ret = mipi_dsi_dcs_exit_sleep_mode(dsi);
	if (ret < 0) {
		dev_err(dev, "Failed to exit sleep mode: %d\n", ret);
		return ret;
	}
	msleep(120);

	dsi_dcs_write_seq(dsi, 0x01, 0x04, 0x01, 0x04, 0x01, 0x01, 0x01, 0x00, 0x01, 0x02, 0x01, 0x01, 0x01, 0x03, 0x01, 0x01, 0x01, 0x05, 0x01, 0x01, 0x03, 0x02, 0x01, 0x32, 0x01, 0x04, 0x01, 0x92, 0x01, 0x01, 0x01, 0x00, 0x01, 0x02, 0x01, 0x01, 0x01, 0x03, 0x01, 0x01, 0x01, 0x05, 0x01, 0x03, 0x03, 0x02, 0x01, 0x0a, 0x03, 0x02, 0x01, 0x01, 0x01, 0x04, 0x01, 0x92, 0x01, 0x03, 0x01, 0x00, 0x01, 0x05, 0x01, 0x02, 0x03, 0x02, 0x01, 0x14, 0x01, 0x04, 0x01, 0x92, 0x01, 0x03, 0x01, 0x01, 0x01, 0x05, 0x01, 0x03, 0x03, 0x02, 0x01, 0x0a, 0x04, 0x05, 0x02, 0x11, 0x00, 0x03, 0x02, 0x01, 0x96, 0x04, 0x05, 0x02, 0x29, 0x00, 0x03, 0x02, 0x01, 0x32, 0x03, 0x0c, 0x01, 0x01);


	ret = mipi_dsi_dcs_set_display_on(dsi);
	if (ret < 0) {
		dev_err(dev, "Failed to set display on: %d\n", ret);
		return ret;
	}
	msleep(20);

	//dsi_dcs_write_seq(dsi, 0xf0, 0xa5, 0xa5);
	//dsi_dcs_write_seq(dsi, 0xf1, 0xa5, 0xa5);
	//dsi_dcs_write_seq(dsi, 0xfc, 0x5a, 0x5a);

	dev_info(dev, "display set on\n");

	return 0;
}

static int boe_s960_fhd_off(struct boe_s960_fhd *ctx)
{
	struct mipi_dsi_device *dsi = ctx->dsi;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	dsi_dcs_write_seq(dsi, 0x28, 0x00);
	msleep(40);
	dsi_dcs_write_seq(dsi, 0x10, 0x00);
	msleep(100);

	return 0;
}

static int boe_s960_fhd_prepare(struct drm_panel *panel)
{
	struct boe_s960_fhd *ctx = to_boe_s960_fhd(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	if (ctx->prepared)
		return 0;

	ret = regulator_bulk_enable(ARRAY_SIZE(ctx->supplies), ctx->supplies);
	if (ret < 0) {
		dev_err(dev, "Failed to enable regulators: %d\n", ret);
		return ret;
	}

	boe_s960_fhd_reset(ctx);

	ret = boe_s960_fhd_on(ctx);
	if (ret < 0) {
		dev_err(dev, "Failed to initialize panel: %d\n", ret);
		gpiod_set_value_cansleep(ctx->reset_gpio, 1);
		regulator_bulk_disable(ARRAY_SIZE(ctx->supplies), ctx->supplies);
		return ret;
	}

	ctx->prepared = true;
	return 0;
}

static int boe_s960_fhd_unprepare(struct drm_panel *panel)
{
	struct boe_s960_fhd *ctx = to_boe_s960_fhd(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	if (!ctx->prepared)
		return 0;

	ret = boe_s960_fhd_off(ctx);
	if (ret < 0)
		dev_err(dev, "Failed to un-initialize panel: %d\n", ret);

	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	regulator_bulk_disable(ARRAY_SIZE(ctx->supplies), ctx->supplies);

	ctx->prepared = false;
	return 0;
}

static const struct drm_display_mode boe_s960_fhd_mode = {
	.clock = (1200 + 80 + 4 + 70) * (1920 + 25 + 4 + 35) * 50 / 1000,
	.hdisplay =1200,
	.hsync_start = 1200 + 80,
	.hsync_end = 1200 + 80 + 4,
	.htotal =1200 + 80 + 4 + 70,
	.vdisplay =1920,
	.vsync_start = 1920 + 25,
	.vsync_end = 1920 + 25 + 4,
	.vtotal =1920 + 25 + 4 + 35,
	.width_mm = 140, 
	.height_mm = 210, 
};

static int boe_s960_fhd_get_modes(struct drm_panel *panel,
					struct drm_connector *connector)
{
	struct drm_display_mode *mode;

	mode = drm_mode_duplicate(connector->dev, &boe_s960_fhd_mode);
	if (!mode)
		return -ENOMEM;

	drm_mode_set_name(mode);

	mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
	connector->display_info.width_mm = mode->width_mm;
	connector->display_info.height_mm = mode->height_mm;
	drm_mode_probed_add(connector, mode);

	return 1;
}

static const struct drm_panel_funcs boe_s960_fhd_panel_funcs = {
	.prepare = boe_s960_fhd_prepare,
	.unprepare = boe_s960_fhd_unprepare,
	.get_modes = boe_s960_fhd_get_modes,
};

static int boe_s960_fhd_bl_update_status(struct backlight_device *bl)
{
	struct mipi_dsi_device *dsi = bl_get_data(bl);
	u16 brightness = backlight_get_brightness(bl);
	int ret;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	ret = mipi_dsi_dcs_set_display_brightness(dsi, brightness);
	if (ret < 0)
		return ret;

	dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	return 0;
}

static const struct backlight_ops boe_s960_fhd_bl_ops = {
	.update_status = boe_s960_fhd_bl_update_status,
};

static struct backlight_device *
boe_s960_fhd_create_backlight(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	const struct backlight_properties props = {
		.type = BACKLIGHT_RAW,
		.brightness = 255,
		.max_brightness = 255,
	};

	return devm_backlight_device_register(dev, dev_name(dev), dev, dsi,
					      &boe_s960_fhd_bl_ops, &props);
}

static int boe_s960_fhd_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct boe_s960_fhd *ctx;
	int ret;

	ctx = devm_kzalloc(dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->supplies[0].supply = "vddio";
	ctx->supplies[1].supply = "vdd";
	ret = devm_regulator_bulk_get(dev, ARRAY_SIZE(ctx->supplies),
				      ctx->supplies);
	if (ret < 0)
		return dev_err_probe(dev, ret, "Failed to get regulators\n");

	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->reset_gpio),
				     "Failed to get reset-gpios\n");

	ctx->dsi = dsi;
	mipi_dsi_set_drvdata(dsi, ctx);

	dsi->lanes = 2;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE |
			  MIPI_DSI_MODE_NO_EOT_PACKET |
			  MIPI_DSI_CLOCK_NON_CONTINUOUS;

	drm_panel_init(&ctx->panel, dev, &boe_s960_fhd_panel_funcs,
		       DRM_MODE_CONNECTOR_DSI);

	ret = drm_panel_of_backlight(&ctx->panel);
	if (ret)
		return dev_err_probe(dev, ret, "Failed to get backlight\n");

	/* Fallback to DCS backlight if no backlight is defined in DT */
	if (!ctx->panel.backlight) {
		ctx->panel.backlight = boe_s960_fhd_create_backlight(dsi);
		if (IS_ERR(ctx->panel.backlight))
			return dev_err_probe(dev, PTR_ERR(ctx->panel.backlight),
					     "Failed to create backlight\n");
	}

	drm_panel_add(&ctx->panel);

	ret = mipi_dsi_attach(dsi);
	if (ret < 0) {
		dev_err(dev, "Failed to attach to DSI host: %d\n", ret);
		drm_panel_remove(&ctx->panel);
		return ret;
	}

	return 0;
}

static void boe_s960_fhd_remove(struct mipi_dsi_device *dsi)
{
	struct boe_s960_fhd *ctx = mipi_dsi_get_drvdata(dsi);
	int ret;

	ret = mipi_dsi_detach(dsi);
	if (ret < 0)
		dev_err(&dsi->dev, "Failed to detach from DSI host: %d\n", ret);

	drm_panel_remove(&ctx->panel);
}

static const struct of_device_id boe_s960_fhd_of_match[] = {
	{ .compatible = "s960,wuxga_boe_fhd" }, // FIXME
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, boe_s960_fhd_of_match);

static struct mipi_dsi_driver boe_s960_fhd_driver = {
	.probe = boe_s960_fhd_probe,
	.remove = boe_s960_fhd_remove,
	.driver = {
		.name = "panel-s960_wuxga_boe_fhd",
		.of_match_table = boe_s960_fhd_of_match,
	},
};
module_mipi_dsi_driver(boe_s960_fhd_driver);

MODULE_AUTHOR("Maxim Kutnij <gtk3@inbox.ru>");
MODULE_DESCRIPTION("DRM driver for boe_s960_fhd video mode dsi panel");
MODULE_LICENSE("GPL");
