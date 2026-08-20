#include "lumia.h"

static int win_id = -1;
static int notify_button = -1;
static int copy_button = -1;
static int menu_button = -1;
static int heartbeat_progress = -1;
static int notifications_checkbox = -1;
static int refresh_dropdown = -1;
static int mode_radio = -1;
static int note_textbox = -1;
static int section_tabs = -1;
static int service_list = -1;
static int log_scroll = -1;
static int actions_menu = -1;
static const char* refresh_options[] = {"250 ms", "500 ms", "1000 ms"};
static const char* mode_options[] = {"Quiet", "Normal", "Verbose"};
static const char* section_options[] = {"Overview", "Services", "Logs"};
static const char* service_options[] = {"Network", "Audio", "Storage", "Packages"};
static const char* log_lines[] = {
    "[OK] Event queue initialized", "[OK] Timer service online",
    "[OK] Clipboard provider ready", "[OK] Network descriptor active",
    "[OK] Audio stream available", "[OK] Package cache mounted",
    "[OK] Window compositor ready", "[OK] Widget handles owned",
    "[OK] Scroll viewport clipped", "[OK] Dashboard checks complete"
};
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
            sys_gui_textbox_handle_key(note_textbox, event.data.key.scancode);
        }
    }
    if (timer_poll(timer_id) > 0) heartbeat = (heartbeat + 7) % 101;
    int menu_action = sys_gui_context_menu_poll(actions_menu);
    if (menu_action == 10) {
        heartbeat = 0;
        notification_show("Dashboard", "Dashboard counters refreshed.");
    } else if (menu_action == 20) {
        clipboard_set_text("Copied from Lumia Dashboard context menu");
        notification_show("Dashboard", "Context action copied text.");
    }

    char* digits = uptime_text;
    const char* prefix = "Uptime: ";
    while (*prefix) *digits++ = *prefix++;
    append_uint(digits, clock_monotonic_ms() / 1000);

    sys_gui_rect(win_id, 0, 0, 520, 390, 0x00131A24);
    sys_gui_text(win_id, 20, 18, "LumiaSDK Service Dashboard", 0x0068B5FF);
    int active_tab = sys_gui_draw_tab_view(section_tabs, 20, 42, 480, 28);
    sys_gui_text(win_id, 20, 84, uptime_text, 0x00FFFFFF);
    sys_gui_text(win_id, 20, 108, "Last key:", 0x00B9C7D8);
    sys_gui_text(win_id, 112, 108, key_text, 0x00FFD166);
    if (active_tab == 0) {
        sys_gui_text(win_id, 20, 140, "Repeating timer heartbeat", 0x00B9C7D8);
        sys_gui_draw_progress(heartbeat_progress, 20, 162, 480, 18, heartbeat);
        sys_gui_draw_checkbox(notifications_checkbox, 20, 194);
        sys_gui_draw_textbox(note_textbox, 20, 226, 210);
        sys_gui_draw_radio_group(mode_radio, 270, 222, 20);
        sys_gui_draw_dropdown(refresh_dropdown, 270, 188, 180);
    } else if (active_tab == 1) {
        sys_gui_text(win_id, 20, 140, "Select a kernel service", 0x00B9C7D8);
        sys_gui_draw_list_view(service_list, 20, 162, 300, 108, 26);
        int selected = sys_gui_get_list_view(service_list);
        if (selected >= 0) sys_gui_text(win_id, 340, 172, service_options[selected], 0x0068B5FF);
    } else {
        sys_gui_text(win_id, 20, 140, "Scrollable event stream", 0x00B9C7D8);
        int scroll_offset = sys_gui_begin_scroll_container(log_scroll, 20, 162, 480, 118);
        for (int i = 0; i < 10; i++)
            sys_gui_text(win_id, 32, 172 + i * 28 - scroll_offset, log_lines[i], i == 9 ? 0x0068B5FF : 0x00B9C7D8);
        sys_gui_end_scroll_container(log_scroll);
    }

    if (sys_gui_draw_button(notify_button, 20, 312, 150, 38) &&
        sys_gui_get_checkbox(notifications_checkbox))
        notification_show("Lumia Dashboard", "The SDK notification service works.");
    if (sys_gui_draw_button(copy_button, 185, 312, 150, 38)) {
        clipboard_set_text("Copied from Lumia Dashboard");
        notification_show("Clipboard", "Dashboard text copied.");
    }
    if (sys_gui_draw_button(menu_button, 350, 312, 150, 38))
        sys_gui_context_menu_show(actions_menu, 350, 286);
}

int lumia_main(int argc, char** argv) {
    (void)argc; (void)argv;
    win_id = sys_gui_window("Lumia Dashboard", 520, 390);
    if (win_id < 0) return 1;
    notify_button = sys_gui_create_button(win_id, "Show notification", 1);
    copy_button = sys_gui_create_button(win_id, "Copy SDK message", 0);
    menu_button = sys_gui_create_button(win_id, "Actions", 0);
    heartbeat_progress = sys_gui_create_progress(win_id, 0);
    notifications_checkbox = sys_gui_create_checkbox(win_id, "Enable notifications", 1);
    refresh_dropdown = sys_gui_create_dropdown(win_id, refresh_options, 3, 0);
    mode_radio = sys_gui_create_radio_group(win_id, mode_options, 3, 1);
    note_textbox = sys_gui_create_textbox(win_id, "Type a dashboard note", 48, 0);
    section_tabs = sys_gui_create_tab_view(win_id, section_options, 3, 0);
    service_list = sys_gui_create_list_view(win_id, service_options, 4, 0);
    log_scroll = sys_gui_create_scroll_container(win_id, 300);
    actions_menu = sys_gui_create_context_menu(win_id, 190);
    sys_gui_context_menu_add_item(actions_menu, "Refresh dashboard", 10, LUMIA_CONTEXT_ITEM_DEFAULT);
    sys_gui_context_menu_add_item(actions_menu, "Copy status", 20, LUMIA_CONTEXT_ITEM_DEFAULT);
    sys_gui_context_menu_add_separator(actions_menu);
    sys_gui_context_menu_add_item(actions_menu, "Administrator tools", 30, LUMIA_CONTEXT_ITEM_DISABLED);
    timer_id = timer_create(250, 1);
    sys_gui_set_tick(win_id, (void*)tick);
    sys_stay_resident();
    return 0;
}

