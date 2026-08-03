#include "state.h"
#include "../platform/storage.h"

void state_init(NexusContext* ctx) {
    ctx->win_id = -1;
    ctx->timer_id = -1;
    ctx->is_dark_mode = 1; // default
    ctx->event_ticks = 0;
    ctx->is_running = 1;
    
    // Load persisted theme
    int saved_theme = storage_load_theme_preference();
    if (saved_theme >= 0) {
        ctx->is_dark_mode = saved_theme;
    }
}

void state_toggle_theme(NexusContext* ctx) {
    ctx->is_dark_mode = !ctx->is_dark_mode;
    storage_save_theme_preference(ctx->is_dark_mode);
}

void state_increment_ticks(NexusContext* ctx) {
    ctx->event_ticks++;
}

void state_shutdown(NexusContext* ctx) {
    ctx->is_running = 0;
}
