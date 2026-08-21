#ifndef NEXUS_WINDOW_H
#define NEXUS_WINDOW_H

#include "../core/types.h"

int window_create(void);
void window_destroy(int win_id);
void window_request_redraw(NexusContext* ctx);

#endif

