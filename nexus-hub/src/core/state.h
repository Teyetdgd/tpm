#ifndef NEXUS_STATE_H
#define NEXUS_STATE_H

#include "types.h"

void state_init(NexusContext* ctx);
void state_toggle_theme(NexusContext* ctx);
void state_increment_ticks(NexusContext* ctx);
void state_shutdown(NexusContext* ctx);

#endif

