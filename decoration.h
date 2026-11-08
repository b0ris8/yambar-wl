#pragma once

#include <pixman.h>

/* yambar-wl: pointer position over the bar, in buffer pixels; -1 = not over the bar.
 * Set by the bar on pointer motion/leave, read by decorations that react to hover. */
extern int deco_hover_x;
extern int deco_hover_y;

struct deco {
    void *private;
    void (*expose)(const struct deco *deco, pixman_image_t *pix, int x, int y, int width, int height);
    void (*destroy)(struct deco *deco);
};

#define DECORATION_COMMON_ATTRS                                                                                        \
    {                                                                                                                  \
        NULL, false, NULL                                                                                              \
    }
