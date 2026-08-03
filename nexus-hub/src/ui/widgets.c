#include "widgets.h"
#include "theme.h"
#include "../core/state.h"
#include "../platform/notifications.h"
#include "../modules/network_diag.h"
#include "../modules/audio_player.h"
#include <lumia.h>

static void draw_btn(int win_id, int x, int y, int w, int h, const char* label, NexusTheme t) {
    sys_gui_rect(win_id, x, y, w, h, t.button_bg);
    sys_gui_text(win_id, x + 20, y + 15, label, t.fg);
}

void widgets_draw_sidebar(NexusContext* ctx) {
    NexusTheme t = theme_get_current(ctx->is_dark_mode);
    draw_btn(ctx->win_id, 10, 80, 180, 40, "Toggle Theme", t);
    draw_btn(ctx->win_id, 10, 130, 180, 40, "Network Check", t);
    draw_btn(ctx->win_id, 10, 180, 180, 40, "Play Sound", t);
    draw_btn(ctx->win_id, 10, 230, 180, 40, "Trigger Notify", t);
}

void widgets_handle_click(NexusContext* ctx, int x, int y) {
    if (x >= 10 && x <= 190) {
        if (y >= 80 && y <= 120) {
            state_toggle_theme(ctx);
        }
        else if (y >= 130 && y <= 170) {
            network_diag_run();
            notify_show("Network", "Socket connection tested.");
        }
        else if (y >= 180 && y <= 220) {
            audio_player_play_beep();
            notify_show("Audio", "Sound triggered via SDK.");
        }
        else if (y >= 230 && y <= 270) {
            notify_show("Nexus Hub", "LumiaOS Notification API works!");
        }
    }
}
