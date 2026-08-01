#include "lumia.h"

static int win_id = -1;
static int notify_button = -1;
static int copy_button = -1;
static int heartbeat_progress = -1;
static int timer_id = -1;
static int heartbeat;
static int key_events;
static char uptime_text[32];
static char key_text[2] = {'-', 0};

static void append_uint(char* out, unsigned value) {
    char reversed[12];
    int count = 0;
    if (!value) reversed[count++] = '0';
    while (value && count < 11) {
        reversed[count++] = (char)('0' + value % 10);
        value /= 10;
    }
    while (count) *out++ = reversed[--count];
    *out = 0;
}

static void tick(void* context) {
    (void)context;
    LumiaEvent event;
    while (lumia_poll_event(win_id, &event) > 0) {
        if (event.type == LUMIA_EVENT_KEY_CHAR) {
            key_text[0] = event.data.key.character;
            key_events++;
        }
    }
    if (timer_poll(timer_id) > 0) heartbeat = (heartbeat + 7) % 101;

    char* digits = uptime_text;
    const char* prefix = "Uptime: ";
    while (*prefix) *digits++ = *prefix++;
    append_uint(digits, clock_monotonic_ms() / 1000);

    sys_gui_rect(win_id, 0, 0, 440, 250, 0x00131A24);
    sys_gui_text(win_id, 20, 18, "LumiaSDK Service Dashboard", 0x0068B5FF);
    sys_gui_text(win_id, 20, 48, uptime_text, 0x00FFFFFF);
    sys_gui_text(win_id, 20, 72, "Last key:", 0x00B9C7D8);
    sys_gui_text(win_id, 112, 72, key_text, 0x00FFD166);
    sys_gui_text(win_id, 150, 72, "Key events:", 0x00B9C7D8);
    sys_gui_text(win_id, 250, 72, "type in this window", 0x007A8DA5);
    sys_gui_text(win_id, 20, 104, "Repeating timer heartbeat", 0x00B9C7D8);
    sys_gui_draw_progress(heartbeat_progress, 20, 126, 400, 18, heartbeat);

    if (sys_gui_draw_button(notify_button, 20, 170, 180, 38))
        notification_show("Lumia Dashboard", "The SDK notification service works.");
    if (sys_gui_draw_button(copy_button, 220, 170, 200, 38)) {
        clipboard_set_text("Copied from Lumia Dashboard");
        notification_show("Clipboard", "Dashboard text copied.");
    }
}

int lumia_main(int argc, char** argv) {
    (void)argc; (void)argv;
    win_id = sys_gui_window("Lumia Dashboard", 440, 250);
    if (win_id < 0) return 1;
    notify_button = sys_gui_create_button(win_id, "Show notification", 1);
    copy_button = sys_gui_create_button(win_id, "Copy SDK message", 0);
    heartbeat_progress = sys_gui_create_progress(win_id, 0);
    timer_id = timer_create(250, 1);
    sys_gui_set_tick(win_id, (void*)tick);
    sys_stay_resident();
    return 0;
}
