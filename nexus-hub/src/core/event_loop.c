#include "event_loop.h"
#include "state.h"
#include "../ui/window.h"
#include "../ui/layout.h"
#include "../ui/widgets.h"
#include "../modules/audio_player.h"
#include "../utils/string_utils.h"
#include <lumia.h>

void event_loop_run(NexusContext* ctx) {
    LumiaEvent ev;
    
    while (ctx->is_running) {
        if (lumia_wait_event(ctx->win_id, &ev, 33) == 1) {
            if (ev.type == LUMIA_EVENT_WINDOW_CLOSE) {
                state_shutdown(ctx);
            } 
            else if (ev.type == LUMIA_EVENT_TIMER) {
                state_increment_ticks(ctx);
                window_request_redraw(ctx);
            } 
            else if (ev.type == LUMIA_EVENT_MOUSE_DOWN) {
                // mouse.x/y are ABSOLUTE screen coordinates; the sidebar
                // buttons live in window-client space, so hit-testing must
                // use the client-relative local_x/local_y pair instead —
                // otherwise every click misses unless the window sits at the
                // screen origin.
                int mx = ev.data.mouse.local_x;
                int my = ev.data.mouse.local_y;
                widgets_handle_click(ctx, mx, my);
                window_request_redraw(ctx);
            }
            else if (ev.type == LUMIA_EVENT_NOTIFICATION_CLICK) {
                string_copy(ctx->last_action, "Notification Callback");
                string_copy(ctx->action_detail, "Notification Clicked -> Listener Fired!");
                window_request_redraw(ctx);
            }
            else if (ev.type == LUMIA_EVENT_WINDOW_RESIZE ||
                     ev.type == LUMIA_EVENT_WINDOW_MOVE ||
                     ev.type == LUMIA_EVENT_WINDOW_MAXIMIZE ||
                     ev.type == LUMIA_EVENT_WINDOW_RESTORE ||
                     ev.type == LUMIA_EVENT_MOUSE_MOVE ||
                     ev.type == LUMIA_EVENT_MOUSE_UP) {
                window_request_redraw(ctx);
            }
        } else {
            // Timeout pass (33ms - 30 FPS): re-render interface
            window_request_redraw(ctx);
        }
    }
}

