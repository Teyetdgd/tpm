#ifndef NEXUS_LAYOUT_H
#define NEXUS_LAYOUT_H

#include "../core/types.h"

void layout_render_all(NexusContext* ctx);
void layout_render_header(NexusContext* ctx);
void layout_render_sidebar(NexusContext* ctx);
void layout_render_content(NexusContext* ctx);
void layout_render_footer(NexusContext* ctx);

#endif
