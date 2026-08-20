#include "layout.h"
#include "theme.h"
#include "widgets.h"
#include "../modules/system_monitor.h"
#include "../utils/string_utils.h"
#include <lumia.h>

/* Responsive layout: everything is positioned from the live client size
 * (ctx->win_w/win_h), which layout_update_metrics refreshes from the kernel
 * on every repaint. Nothing below may hardcode 800x600 — that assumption is
 * exactly what used to leave the UI mangled after maximize/resize. */

#define NEXUS_HEADER_H 60
#define NEXUS_SIDEBAR_W 200
#define NEXUS_FOOTER_H 40

void layout_update_metrics(NexusContext* ctx) {
    if (ctx->win_id < 0) return;
    int info[6];
    if (sys_gui_window_info(ctx->win_id, info)) {
        ctx->win_w = info[2];
        ctx->win_h = info[3];
    }
    if (ctx->win_w < 200) ctx->win_w = 200;   // kernel minimum is 150x100;
    if (ctx->win_h < 150) ctx->win_h = 150;   // keep the layout math sane
}

void layout_render_header(NexusContext* ctx) {
    NexusTheme t = theme_get_current(ctx->is_dark_mode);
    int w = ctx->win_w;
    sys_gui_rect(ctx->win_id, 0, 0, w, NEXUS_HEADER_H, t.bg);
    sys_gui_rect(ctx->win_id, 0, NEXUS_HEADER_H - 1, w, 1, t.button_bg); // border

    sys_gui_text(ctx->win_id, 20, 24, "NEXUS HUB", t.accent);
    sys_gui_text(ctx->win_id, 130, 24, "|  Enterprise Diagnostics Interface", t.text_muted);
}

void layout_render_sidebar(NexusContext* ctx) {
    NexusTheme t = theme_get_current(ctx->is_dark_mode);
    int body_h = ctx->win_h - NEXUS_HEADER_H - NEXUS_FOOTER_H;
    if (body_h < 0) body_h = 0;
    sys_gui_rect(ctx->win_id, 0, NEXUS_HEADER_H, NEXUS_SIDEBAR_W, body_h, t.bg);
    sys_gui_rect(ctx->win_id, NEXUS_SIDEBAR_W - 1, NEXUS_HEADER_H, 1, body_h, t.button_bg); // border

    // Draw buttons via widgets
    widgets_draw_sidebar(ctx);
}

void layout_render_content(NexusContext* ctx) {
    NexusTheme t = theme_get_current(ctx->is_dark_mode);
    int body_h = ctx->win_h - NEXUS_HEADER_H - NEXUS_FOOTER_H;
    if (body_h < 0) body_h = 0;
    int cx = NEXUS_SIDEBAR_W;
    int cw = ctx->win_w - NEXUS_SIDEBAR_W;
    if (cw < 0) cw = 0;
    sys_gui_rect(ctx->win_id, cx, NEXUS_HEADER_H, cw, body_h, t.bg);

    int pad = 30;
    int inner_w = cw - pad * 2;
    if (inner_w < 0) inner_w = 0;

    // Title
    sys_gui_text(ctx->win_id, cx + pad, 90, "SYSTEM OVERVIEW", t.fg);

    // Stats Box — stretches with the window
    sys_gui_rect(ctx->win_id, cx + pad, 130, inner_w, 110, t.button_bg);

    char pid_buf[32];
    string_concat("PID: ", system_monitor_get_pid_string(), pid_buf);
    sys_gui_text(ctx->win_id, cx + pad + 20, 150, pid_buf, t.fg);

    char tick_buf[64];
    char tmp[16];
    string_itoa(ctx->event_ticks, tmp);
    string_concat("Event Loop Ticks: ", tmp, tick_buf);
    sys_gui_text(ctx->win_id, cx + pad + 20, 180, tick_buf, t.fg);

    sys_gui_text(ctx->win_id, cx + pad + 20, 210, "Status: RING 3 ACTIVE", t.accent);

    // Action / Diagnostics Feedback Panel
    sys_gui_text(ctx->win_id, cx + pad, 260, "DIAGNOSTICS & ACTIONS", t.fg);
    sys_gui_rect(ctx->win_id, cx + pad, 290, inner_w, 110, t.button_bg);

    if (ctx->last_action[0] != 0) {
        char act_buf[128];
        string_concat("Last Action: ", ctx->last_action, act_buf);
        sys_gui_text(ctx->win_id, cx + pad + 20, 315, act_buf, t.accent);

        char det_buf[128];
        string_concat("Result: ", ctx->action_detail, det_buf);
        sys_gui_text(ctx->win_id, cx + pad + 20, 345, det_buf, t.fg);
    } else {
        sys_gui_text(ctx->win_id, cx + pad + 20, 325, "Click any sidebar button to run diagnostic action.", t.text_muted);
    }
}

void layout_render_footer(NexusContext* ctx) {
    NexusTheme t = theme_get_current(ctx->is_dark_mode);
    int fy = ctx->win_h - NEXUS_FOOTER_H;
    if (fy < NEXUS_HEADER_H) fy = NEXUS_HEADER_H;
    sys_gui_rect(ctx->win_id, 0, fy, ctx->win_w, NEXUS_FOOTER_H, t.button_bg);
    sys_gui_text(ctx->win_id, 20, fy + 12, "LumiaSDK v0.9.72", t.text_muted);
}

void layout_render_all(NexusContext* ctx) {
    layout_update_metrics(ctx);
    NexusTheme t = theme_get_current(ctx->is_dark_mode);
    sys_gui_rect(ctx->win_id, 0, 0, ctx->win_w, ctx->win_h, t.bg);
    layout_render_header(ctx);
    layout_render_sidebar(ctx);
    layout_render_content(ctx);
    layout_render_footer(ctx);
}

