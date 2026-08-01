#include "lumia.h"

#define DATASET_BYTES (2 * 1024 * 1024)
static const unsigned char diagnostics_dataset[DATASET_BYTES] = {0x4c, 0x55, 0x4d, 0x49, 0x41};

static int win = -1, cpu_bar = -1, memory_bar = -1;
static int volume_slider = -1, interval_slider = -1;
static int save_button = -1, copy_button = -1, notify_button = -1;
static int timer_id = -1, activity = 24, pulse, selected_panel;
static char status_text[64] = "Configuration loaded";

static void text_copy(char* out, const char* in, int cap) {
    int i = 0; while (in[i] && i + 1 < cap) { out[i] = in[i]; i++; } out[i] = 0;
}

static void append_uint(char* out, unsigned value) {
    char reverse[12]; int count = 0, used = strlen(out);
    if (!value) reverse[count++] = '0';
    while (value && count < 11) { reverse[count++] = (char)('0' + value % 10); value /= 10; }
    while (count) out[used++] = reverse[--count];
    out[used] = 0;
}

static unsigned dataset_signature(void) {
    unsigned hash = 2166136261u;
    for (unsigned i = 0; i < DATASET_BYTES; i += 4096)
        hash = (hash ^ diagnostics_dataset[i]) * 16777619u;
    return hash;
}

static void draw_sidebar(void) {
    static const char* labels[] = {"Overview", "Activity", "Storage", "SDK Events"};
    sys_gui_rect(win, 0, 0, 176, 430, 0x00101824);
    sys_gui_text(win, 20, 22, "LUMIA", 0x0068B5FF);
    sys_gui_text(win, 76, 22, "CONTROL", 0x00FFFFFF);
    for (int i = 0; i < 4; i++) {
        int y = 76 + i * 48;
        if (i == selected_panel) sys_gui_rect(win, 10, y - 10, 156, 34, 0x00213A55);
        sys_gui_text(win, 26, y, labels[i], i == selected_panel ? 0x0068B5FF : 0x0094A3B8);
    }
    sys_gui_text(win, 20, 382, "Ring 3 / SDK 1.0", 0x0064748B);
}

static void draw_chart(void) {
    sys_gui_rect(win, 204, 202, 462, 116, 0x00182230);
    sys_gui_text(win, 222, 218, "ACTIVITY HISTORY", 0x0094A3B8);
    for (int i = 0; i < 20; i++) {
        int sample = (diagnostics_dataset[(i * 104729 + pulse) % DATASET_BYTES] + i * 17 + pulse) % 58;
        sys_gui_rect(win, 224 + i * 21, 294 - sample, 12, sample, 0x003B82F6);
    }
}

static void draw_overview(void) {
    char uptime[48] = "Session uptime: "; append_uint(uptime, clock_monotonic_ms() / 1000);
    sys_gui_text(win, 204, 24, "Operations Overview", 0x00FFFFFF);
    sys_gui_text(win, 204, 48, uptime, 0x0094A3B8);
    sys_gui_rect(win, 204, 78, 220, 104, 0x00182230);
    sys_gui_rect(win, 446, 78, 220, 104, 0x00182230);
    sys_gui_text(win, 222, 94, "SERVICE LOAD", 0x0094A3B8);
    sys_gui_text(win, 464, 94, "MEMORY PRESSURE", 0x0094A3B8);
    sys_gui_draw_progress(cpu_bar, 222, 130, 184, 18, activity);
    sys_gui_draw_progress(memory_bar, 464, 130, 184, 18, (activity * 3 + 19) % 100);
    draw_chart();
    sys_gui_text(win, 204, 342, "Output volume", 0x0094A3B8);
    sys_gui_draw_slider(volume_slider, 324, 334, 150, 24);
    sys_gui_text(win, 500, 342, "Refresh", 0x0094A3B8);
    sys_gui_draw_slider(interval_slider, 566, 334, 100, 24);
}

static void draw_footer(void) {
    sys_gui_rect(win, 176, 378, 544, 52, 0x00131C29);
    if (sys_gui_draw_button(save_button, 194, 388, 116, 30)) {
        char config[48] = "volume="; append_uint(config, sys_gui_get_slider(volume_slider));
        fs_write_file("control-center.cfg", config, strlen(config));
        text_copy(status_text, "Saved to VFS", sizeof(status_text));
    }
    if (sys_gui_draw_button(copy_button, 322, 388, 132, 30)) {
        clipboard_set_text("Lumia Control Center diagnostics are healthy");
        text_copy(status_text, "Report copied", sizeof(status_text));
    }
    if (sys_gui_draw_button(notify_button, 466, 388, 116, 30)) {
        notification_show("Control Center", "All monitored SDK services are operational.");
        text_copy(status_text, "Notification sent", sizeof(status_text));
    }
    sys_gui_text(win, 594, 398, status_text, 0x007DD3FC);
}

static void tick(void* unused) {
    (void)unused; LumiaEvent event;
    while (lumia_poll_event(win, &event) > 0) {
        if (event.type == LUMIA_EVENT_KEY_CHAR) {
            char c = event.data.key.character;
            if (c >= '1' && c <= '4') selected_panel = c - '1';
            text_copy(status_text, "Key event received", sizeof(status_text));
        } else if (event.type == LUMIA_EVENT_WINDOW_RESIZE)
            text_copy(status_text, "Window resized", sizeof(status_text));
        else if (event.type == LUMIA_EVENT_MOUSE_DOWN)
            text_copy(status_text, "Pointer event", sizeof(status_text));
    }
    if (timer_poll(timer_id) > 0) {
        pulse++; activity = (activity + 7 + (dataset_signature() & 3)) % 101;
    }
    sys_gui_rect(win, 0, 0, 720, 430, 0x000B111B);
    draw_sidebar(); draw_overview(); draw_footer();
}

int lumia_main(int argc, char** argv) {
    (void)argc; (void)argv;
    win = sys_gui_window("Lumia Control Center", 720, 430);
    if (win < 0) return 1;
    cpu_bar = sys_gui_create_progress(win, 0);
    memory_bar = sys_gui_create_progress(win, 0);
    volume_slider = sys_gui_create_slider(win, 0, 10, 7);
    interval_slider = sys_gui_create_slider(win, 1, 10, 4);
    save_button = sys_gui_create_button(win, "Save", 1);
    copy_button = sys_gui_create_button(win, "Copy report", 0);
    notify_button = sys_gui_create_button(win, "Notify", 0);
    timer_id = timer_create(350, 1);
    sys_gui_set_tick(win, (void*)tick);
    sys_stay_resident();
    return 0;
}
