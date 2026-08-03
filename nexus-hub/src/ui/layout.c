#include "layout.h"
#include "theme.h"
#include "widgets.h"
#include "../modules/system_monitor.h"
#include "../utils/string_utils.h"
#include <lumia.h>

void layout_render_header(NexusContext* ctx) {
    NexusTheme t = theme_get_current(ctx->is_dark_mode);
    sys_gui_rect(ctx->win_id, 0, 0, 800, 60, t.bg);
    sys_gui_rect(ctx->win_id, 0, 59, 800, 1, t.button_bg); // border
    
    sys_gui_text(ctx->win_id, 20, 24, "NEXUS HUB", t.accent);
    sys_gui_text(ctx->win_id, 130, 24, "|  Enterprise Diagnostics Interface", t.text_muted);
}

void layout_render_sidebar(NexusContext* ctx) {
    NexusTheme t = theme_get_current(ctx->is_dark_mode);
    sys_gui_rect(ctx->win_id, 0, 60, 200, 500, t.bg);
    sys_gui_rect(ctx->win_id, 199, 60, 1, 500, t.button_bg); // border

    // Draw buttons via widgets
    widgets_draw_sidebar(ctx);
}

void layout_render_content(NexusContext* ctx) {
    NexusTheme t = theme_get_current(ctx->is_dark_mode);
    sys_gui_rect(ctx->win_id, 200, 60, 600, 500, t.bg);
    
    // Title
    sys_gui_text(ctx->win_id, 230, 90, "SYSTEM OVERVIEW", t.fg);
    
    // Stats Box
    sys_gui_rect(ctx->win_id, 230, 130, 540, 120, t.button_bg);
    
    char pid_buf[32];
    string_concat("PID: ", system_monitor_get_pid_string(), pid_buf);
    sys_gui_text(ctx->win_id, 250, 150, pid_buf, t.fg);
    
    char tick_buf[64];
    char tmp[16];
    string_itoa(ctx->event_ticks, tmp);
    string_concat("Event Loop Ticks: ", tmp, tick_buf);
    sys_gui_text(ctx->win_id, 250, 180, tick_buf, t.fg);
    
    sys_gui_text(ctx->win_id, 250, 210, "Status: RING 3 ACTIVE", t.accent);
}

void layout_render_footer(NexusContext* ctx) {
    NexusTheme t = theme_get_current(ctx->is_dark_mode);
    sys_gui_rect(ctx->win_id, 0, 560, 800, 40, t.button_bg);
    sys_gui_text(ctx->win_id, 20, 572, "LumiaSDK v0.9.72", t.text_muted);
}

void layout_render_all(NexusContext* ctx) {
    layout_render_header(ctx);
    layout_render_sidebar(ctx);
    layout_render_content(ctx);
    layout_render_footer(ctx);
}
