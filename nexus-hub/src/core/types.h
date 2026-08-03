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
} NexusContext;

#endif
