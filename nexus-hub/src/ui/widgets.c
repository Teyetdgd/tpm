#include "widgets.h"
#include "theme.h"
#include "../core/state.h"
#include "../platform/notifications.h"
#include "../modules/network_diag.h"
#include "../modules/audio_player.h"
#include "../utils/string_utils.h"
#include <lumia.h>

static void draw_btn(int win_id, int x, int y, int w, int h, const char* label, NexusTheme t) {
    sys_gui_rect(win_id, x, y, w, h, t.button_bg);
    sys_gui_text(win_id, x + 20, y + 15, label, t.fg);
}

void widgets_draw_sidebar(NexusContext* ctx) {
    NexusTheme t = theme_get_current(ctx->is_dark_mode);
    // Buttons stretch with the sidebar width so a resized window never shows
    // clipped or floating controls. Hit-testing uses the same formula.
    int bw = 180;
    if (ctx->win_w - 20 < bw) bw = ctx->win_w - 20;
    if (bw < 40) bw = 40;
    draw_btn(ctx->win_id, 10, 80, bw, 40, "Toggle Theme", t);
    draw_btn(ctx->win_id, 10, 130, bw, 40, "Network Check", t);
    draw_btn(ctx->win_id, 10, 180, bw, 40, "Play Sound", t);
    draw_btn(ctx->win_id, 10, 230, bw, 40, "Trigger Notify", t);
}

void widgets_handle_click(NexusContext* ctx, int x, int y) {
    int bw = 180;
    if (ctx->win_w - 20 < bw) bw = ctx->win_w - 20;
    if (bw < 40) bw = 40;
    if (x >= 10 && x <= 10 + bw) {
        if (y >= 80 && y <= 120) {
            state_toggle_theme(ctx);
            string_copy(ctx->last_action, "Toggle Theme");
            string_copy(ctx->action_detail, ctx->is_dark_mode ? "Dark Theme Applied" : "Light Theme Applied");
        }
        else if (y >= 130 && y <= 170) {
            int res = network_diag_run();
            notify_show("Network", "Socket connection tested.");
            string_copy(ctx->last_action, "Network Check");
            if (res) {
                string_copy(ctx->action_detail, "Socket (AF_INET/STREAM) OK");
            } else {
                string_copy(ctx->action_detail, "Socket Creation Failed");
            }
        }
        else if (y >= 180 && y <= 220) {
            audio_player_play_beep();
            notify_show("Audio", "Sound triggered via SDK.");
            string_copy(ctx->last_action, "Play Sound");
            string_copy(ctx->action_detail, "PC Speaker Tone (C5 523Hz) Played");
        }
        else if (y >= 230 && y <= 270) {
            notify_show("Nexus Hub", "LumiaOS Notification API works!");
            string_copy(ctx->last_action, "Trigger Notify");
            string_copy(ctx->action_detail, "Desktop Toast Sent");
        }
    }
}

