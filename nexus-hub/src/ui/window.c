#include "window.h"
#include "layout.h"
#include <lumia.h>

#define WIN_W 800
#define WIN_H 600

int window_create(void) {
    return sys_gui_window("Nexus Hub - Ultimate Diagnostics", WIN_W, WIN_H);
}

void window_destroy(int win_id) {
    if (win_id >= 0) {
        sys_gui_close_window(win_id);
    }
}

void window_request_redraw(NexusContext* ctx) {
    if (ctx->win_id >= 0) {
        layout_render_all(ctx);
    }
}
