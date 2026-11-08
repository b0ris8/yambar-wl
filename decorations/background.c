#include <stdint.h>
#include <stdlib.h>

#include "../config-verify.h"
#include "../config.h"
#include "../decoration.h"
#include "../plugin.h"

/*
 * background
 *   color:  RRGGBBAA            (required)
 *   radius: N  px, corner radius (optional, default 0 = square)
 *   margin: N  px, vertical inset from top and bottom of the bar
 *              (optional, default 0 = full bar height)
 */

struct private
{
    pixman_color_t color;
    int radius;
    int margin;
};

static void
destroy(struct deco *deco)
{
    struct private *d = deco->private;
    free(d);
    free(deco);
}

/* Anti-aliased quarter circle into an a8 mask (4x4 supersampling).
 * (cx, cy) = circle centre in mask coordinates. */
static void
corner_mask(pixman_image_t *mask, int r, double cx, double cy)
{
    uint8_t *data = (uint8_t *)pixman_image_get_data(mask);
    const int stride = pixman_image_get_stride(mask);
    const double rr = (double)r * r;

    for (int py = 0; py < r; py++) {
        for (int px = 0; px < r; px++) {
            int inside = 0;
            for (int sy = 0; sy < 4; sy++) {
                for (int sx = 0; sx < 4; sx++) {
                    double dx = px + (sx + 0.5) / 4.0 - cx;
                    double dy = py + (sy + 0.5) / 4.0 - cy;
                    if (dx * dx + dy * dy <= rr)
                        inside++;
                }
            }
            data[py * stride + px] = (uint8_t)(inside * 255 / 16);
        }
    }
}

static void
expose(const struct deco *deco, pixman_image_t *pix, int x, int y, int width, int height)
{
    const struct private *d = deco->private;

    const int top = y + d->margin;
    const int h = height - 2 * d->margin;
    if (h <= 0 || width <= 0)
        return;

    int r = d->radius;
    if (r > h / 2)
        r = h / 2;
    if (r > width / 2)
        r = width / 2;

    if (r <= 0) {
        pixman_image_fill_rectangles(PIXMAN_OP_OVER, pix, &d->color, 1, &(pixman_rectangle16_t){x, top, width, h});
        return;
    }

    /* Straight parts: centre column + left/right columns between the corners */
    pixman_rectangle16_t rects[3];
    int n = 0;
    if (width - 2 * r > 0)
        rects[n++] = (pixman_rectangle16_t){x + r, top, width - 2 * r, h};
    if (h - 2 * r > 0) {
        rects[n++] = (pixman_rectangle16_t){x, top + r, r, h - 2 * r};
        rects[n++] = (pixman_rectangle16_t){x + width - r, top + r, r, h - 2 * r};
    }
    if (n > 0)
        pixman_image_fill_rectangles(PIXMAN_OP_OVER, pix, &d->color, n, rects);

    /* Corners */
    pixman_image_t *src = pixman_image_create_solid_fill(&d->color);
    pixman_image_t *mask = pixman_image_create_bits(PIXMAN_a8, r, r, NULL, 0);
    if (src == NULL || mask == NULL)
        goto out;

    const struct {
        int dst_x, dst_y;
        double cx, cy;
    } corners[4] = {
        {x, top, r, r},                         /* top-left */
        {x + width - r, top, 0, r},             /* top-right */
        {x, top + h - r, r, 0},                 /* bottom-left */
        {x + width - r, top + h - r, 0, 0},     /* bottom-right */
    };

    for (int i = 0; i < 4; i++) {
        corner_mask(mask, r, corners[i].cx, corners[i].cy);
        pixman_image_composite32(PIXMAN_OP_OVER, src, mask, pix, 0, 0, 0, 0, corners[i].dst_x, corners[i].dst_y, r,
                                 r);
    }

out:
    if (mask != NULL)
        pixman_image_unref(mask);
    if (src != NULL)
        pixman_image_unref(src);
}

static struct deco *
background_new(pixman_color_t color, int radius, int margin)
{
    struct private *priv = calloc(1, sizeof(*priv));
    priv->color = color;
    priv->radius = radius;
    priv->margin = margin;

    struct deco *deco = calloc(1, sizeof(*deco));
    deco->private = priv;
    deco->expose = &expose;
    deco->destroy = &destroy;

    return deco;
}

static struct deco *
from_conf(const struct yml_node *node)
{
    const struct yml_node *color = yml_get_value(node, "color");
    const struct yml_node *radius = yml_get_value(node, "radius");
    const struct yml_node *margin = yml_get_value(node, "margin");
    return background_new(conf_to_color(color), radius != NULL ? yml_value_as_int(radius) : 0,
                          margin != NULL ? yml_value_as_int(margin) : 0);
}

static bool
verify_conf(keychain_t *chain, const struct yml_node *node)
{
    static const struct attr_info attrs[] = {
        {"color", true, &conf_verify_color},
        {"radius", false, &conf_verify_unsigned},
        {"margin", false, &conf_verify_unsigned},
        DECORATION_COMMON_ATTRS,
    };

    return conf_verify_dict(chain, node, attrs);
}

const struct deco_iface deco_background_iface = {
    .verify_conf = &verify_conf,
    .from_conf = &from_conf,
};

#if defined(CORE_PLUGINS_AS_SHARED_LIBRARIES)
extern const struct deco_iface iface __attribute__((weak, alias("deco_background_iface")));
#endif
