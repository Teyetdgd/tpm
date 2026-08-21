#ifndef NEXUS_TYPES_H
#define NEXUS_TYPES_H

#include <stdint.h>

typedef struct {
    int x;
    int y;
    int w;
    int h;
} NexusRect;

typedef struct {
    int r;
    int g;
    int b;
} NexusColorRGB;

typedef struct {
    int win_id;
    int timer_id;
    int is_dark_mode;
    int event_ticks;
    int is_running;
    int win_w;          // live client size from sys_gui_window_info, kept
    int win_h;          // current by layout_update_metrics every repaint so
                        // maximize/resize never leaves a stale 800x600 layout
    char last_action[64];
    char action_detail[64];
} NexusContext;

#endif

