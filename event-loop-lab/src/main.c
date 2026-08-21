#include "lumia.h"

#define COL_BG      0x00101924
#define COL_PANEL   0x001B2938
#define COL_ACCENT  0x004CC9F0
#define COL_GREEN   0x0057D68D
#define COL_YELLOW  0x00FFD166
#define COL_TEXT    0x00EDF4FA
#define COL_MUTED   0x009CB0C3
#define COL_RED     0x00FF6B6B

static int win_id;
static int timer_id;
static int timer_enabled = 1;
static unsigned total_events;
static unsigned timer_events;
static unsigned key_events;
static unsigned mouse_events;
static unsigned window_events;
static unsigned timeouts;
static unsigned wake_latency;
static char last_event[40] = "Application started";
static char last_key[2] = {'-', 0};

static void copy_text(char* dst, const char* src, int capacity) {
    int i = 0;
    while (i + 1 < capacity && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

static void uint_text(char* out, const char* label, unsigned value) {
    char reverse[12];
    int count = 0;
    while (*label) *out++ = *label++;
    if (!value) reverse[count++] = '0';
    while (value && count < 11) {
        reverse[count++] = (char)('0' + value % 10);
        value /= 10;
    }
    while (count) *out++ = reverse[--count];
    *out = 0;
}

static void metric(int x, int y, const char* label, unsigned value, uint32_t color) {
    char text[48];
    uint_text(text, label, value);
    sys_gui_text(win_id, x, y, text, color);
}

static void render(void) {
    sys_gui_rect(win_id, 0, 0, 620, 430, COL_BG);
    sys_gui_text(win_id, 22, 18, "LumiaSDK Event Loop Lab", COL_ACCENT);
    sys_gui_text(win_id, 22, 42, "Scheduler-backed blocking wait diagnostics", COL_MUTED);

    sys_gui_rect(win_id, 20, 76, 580, 82, COL_PANEL);
    sys_gui_text(win_id, 36, 90, "WAIT STATE", COL_MUTED);
    sys_gui_text(win_id, 36, 116,
                 timer_enabled ? "BLOCKED - waiting for event/timer" : "BLOCKED - 3 second timeout test",
                 timer_enabled ? COL_GREEN : COL_YELLOW);
    metric(400, 90, "Last wake: ", wake_latency, COL_TEXT);
    sys_gui_text(win_id, 520, 90, " ms", COL_TEXT);
    sys_gui_text(win_id, 36, 138, last_event, COL_TEXT);

    sys_gui_rect(win_id, 20, 174, 280, 154, COL_PANEL);
    sys_gui_text(win_id, 36, 190, "EVENT COUNTERS", COL_ACCENT);
    metric(36, 218, "Total events:  ", total_events, COL_TEXT);
    metric(36, 240, "Timer events:  ", timer_events, COL_GREEN);
    metric(36, 262, "Key events:    ", key_events, COL_YELLOW);
    metric(36, 284, "Mouse events:  ", mouse_events, COL_TEXT);
    metric(36, 306, "Window events: ", window_events, COL_TEXT);

    sys_gui_rect(win_id, 320, 174, 280, 154, COL_PANEL);
    sys_gui_text(win_id, 336, 190, "LIVE DETAILS", COL_ACCENT);
    sys_gui_text(win_id, 336, 218, "Last character:", COL_MUTED);
    sys_gui_text(win_id, 488, 218, last_key, COL_YELLOW);
    metric(336, 246, "Timeouts: ", timeouts, timeouts ? COL_RED : COL_GREEN);
    sys_gui_text(win_id, 336, 276,
                 timer_enabled ? "Timer: ON (1000 ms)" : "Timer: OFF", timer_enabled ? COL_GREEN : COL_RED);
    sys_gui_text(win_id, 336, 302, "CPU should stay idle while waiting", COL_MUTED);

    sys_gui_rect(win_id, 20, 346, 580, 60, COL_PANEL);
    sys_gui_text(win_id, 36, 360, "T: toggle timer / timeout test   R: reset counters", COL_TEXT);
    sys_gui_text(win_id, 36, 384, "Move/click/type/resize window. Q closes the lab.", COL_MUTED);
}

static void reset_counters(void) {
    total_events = timer_events = key_events = mouse_events = window_events = timeouts = 0;
    copy_text(last_event, "Counters reset", sizeof(last_event));
}

static void toggle_timer(void) {
    if (timer_enabled) {
        timer_cancel(timer_id);
        timer_enabled = 0;
        copy_text(last_event, "Timer stopped; waiting for timeout", sizeof(last_event));
    } else {
        timer_id = event_timer_create(win_id, 1000, 1);
        timer_enabled = timer_id > 0;
        copy_text(last_event, timer_enabled ? "Timer restarted" : "Timer restart failed", sizeof(last_event));
    }
}

static int handle_event(const LumiaEvent* event) {
    total_events++;
    if (event->type == LUMIA_EVENT_TIMER) {
        timer_events++;
        copy_text(last_event, "Timer event woke blocked process", sizeof(last_event));
    } else if (event->type == LUMIA_EVENT_KEY_CHAR) {
        char key = event->data.key.character;
        key_events++;
        last_key[0] = key ? key : '?';
        copy_text(last_event, "Keyboard event received", sizeof(last_event));
        if (key == 't' || key == 'T') toggle_timer();
        if (key == 'r' || key == 'R') reset_counters();
        if (key == 'q' || key == 'Q') return 0;
    } else if (event->type >= LUMIA_EVENT_MOUSE_MOVE && event->type <= LUMIA_EVENT_MOUSE_SCROLL) {
        mouse_events++;
        copy_text(last_event, "Mouse event received", sizeof(last_event));
    } else if (event->type >= LUMIA_EVENT_WINDOW_FOCUS && event->type <= LUMIA_EVENT_WINDOW_RESTORE) {
        window_events++;
        copy_text(last_event, "Window lifecycle event received", sizeof(last_event));
        if (event->type == LUMIA_EVENT_WINDOW_CLOSE) return 0;
    } else if (event->type == LUMIA_EVENT_QUIT) {
        return 0;
    }
    return 1;
}

int lumia_main(int argc, char** argv) {
    (void)argc; (void)argv;
    win_id = sys_gui_window("Event Loop Lab", 620, 430);
    if (win_id < 0) return 1;
    timer_id = event_timer_create(win_id, 1000, 1);
    if (timer_id < 0) {
        timer_enabled = 0;
        copy_text(last_event, "ERROR: event timer creation failed", sizeof(last_event));
    }

    render();
    for (;;) {
        LumiaEvent event;
        uint32_t before = clock_monotonic_ms();
        int result = lumia_wait_event(win_id, &event, timer_enabled ? UINT32_MAX : 3000u);
        wake_latency = clock_monotonic_ms() - before;
        if (result == 1) {
            if (!handle_event(&event)) break;
        } else if (result == 0) {
            timeouts++;
            copy_text(last_event, "3 second timeout completed correctly", sizeof(last_event));
        } else {
            copy_text(last_event, "ERROR: lumia_wait_event failed", sizeof(last_event));
            render();
            break;
        }
        render();
    }
    if (timer_enabled) timer_cancel(timer_id);
    sys_gui_close_window(win_id);
    return 0;
}

