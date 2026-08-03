#include <lumia.h>
#include "core/state.h"
#include "core/event_loop.h"
#include "ui/window.h"
#include "ui/theme.h"

int lumia_main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    NexusContext ctx;
    state_init(&ctx);

    ctx.win_id = window_create();
    if (ctx.win_id < 0) return -1;
    
    // Start an event timer (1000ms, repeat)
    ctx.timer_id = event_timer_create(ctx.win_id, 1000, 1);

    window_request_redraw(&ctx);
    
    // Blocking event loop
    event_loop_run(&ctx);
    
    window_destroy(ctx.win_id);
    return 0;
}
