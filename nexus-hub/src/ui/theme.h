#ifndef NEXUS_THEME_H
#define NEXUS_THEME_H

#include <stdint.h>

typedef struct {
    uint32_t bg;
    uint32_t fg;
    uint32_t accent;
    uint32_t button_bg;
    uint32_t button_hover;
    uint32_t text_muted;
} NexusTheme;

NexusTheme theme_get_current(int is_dark);

#endif
