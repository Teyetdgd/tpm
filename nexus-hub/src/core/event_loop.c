#include "event_loop.h"
#include "state.h"
#include "../ui/window.h"
#include "../ui/layout.h"
#include "../ui/widgets.h"
#include "../modules/audio_player.h"
#include <lumia.h>

void event_loop_run(NexusContext* ctx) {
    LumiaEvent ev;
    
    while (ctx->is_running) {
        if (lumia_wait_event(ctx->win_id, &ev, 0xFFFFFFFF) == 1) {
            
            if (ev.type == LUMIA_EVENT_WINDOW_CLOSE) {
                state_shutdown(ctx);
            } 
            else if (ev.type == LUMIA_EVENT_TIMER) {
                state_increment_ticks(ctx);
                window_request_redraw(ctx);
            } 
            else if (ev.type == LUMIA_EVENT_MOUSE_DOWN) {
                int mx = ev.data.mouse.x;
                int my = ev.data.mouse.y;
                widgets_handle_click(ctx, mx, my);
            }
        }
    }
}
