#include <lumia.h>
#include <stdint.h>

#define WINDOW_WIDTH  400
#define WINDOW_HEIGHT 300
#define THEME_DARK    0x1E1E1E
#define THEME_LIGHT   0xF0F0F0
#define COLOR_TEXT_D  0xFFFFFF
#define COLOR_TEXT_L  0x000000
#define COLOR_ACCENT  0x0078D7
#define COLOR_BTN     0x333333

static int dark_mode = 1;
static int win_id = -1;
static int timer_id = -1;
static int ticks = 0;

void draw_ui() {
    uint32_t bg = dark_mode ? THEME_DARK : THEME_LIGHT;
    uint32_t fg = dark_mode ? COLOR_TEXT_D : COLOR_TEXT_L;
    
    // Background
    sys_gui_rect(win_id, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, bg);
    
    // Header
    sys_gui_rect(win_id, 0, 0, WINDOW_WIDTH, 40, COLOR_ACCENT);
    sys_gui_text(win_id, 10, 12, "Nexus Hub (LumiaSDK Demo)", 0xFFFFFF);
    
    // Info texts
    char pid_str[32];
    int pid = process_getpid();
    // primitive itoa
    int n = pid, i = 0, j = 0;
    char tmp[10];
    if (n == 0) tmp[i++] = '0';
    while (n > 0) { tmp[i++] = (n % 10) + '0'; n /= 10; }
    char pid_txt[] = "PID: ";
    while (pid_txt[j]) { pid_str[j] = pid_txt[j]; j++; }
    while (i > 0) { pid_str[j++] = tmp[--i]; }
    pid_str[j] = 0;
    
    sys_gui_text(win_id, 10, 60, pid_str, fg);
    
    // Theme Button
    sys_gui_rect(win_id, 10, 100, 120, 30, COLOR_BTN);
    sys_gui_text(win_id, 25, 107, "Toggle Theme", 0xFFFFFF);
    
    // Notification Button
    sys_gui_rect(win_id, 140, 100, 120, 30, COLOR_BTN);
    sys_gui_text(win_id, 155, 107, "Notify & Sound", 0xFFFFFF);

    // Clipboard Button
    sys_gui_rect(win_id, 270, 100, 120, 30, COLOR_BTN);
    sys_gui_text(win_id, 280, 107, "Copy to Clip", 0xFFFFFF);

    // Timer display
    char tick_str[32];
    j = 0;
    char t_txt[] = "Timer Ticks: ";
    while (t_txt[j]) { tick_str[j] = t_txt[j]; j++; }
    n = ticks; i = 0;
    if (n == 0) tmp[i++] = '0';
    while (n > 0) { tmp[i++] = (n % 10) + '0'; n /= 10; }
    while (i > 0) { tick_str[j++] = tmp[--i]; }
    tick_str[j] = 0;
    
    sys_gui_text(win_id, 10, 150, tick_str, fg);

    // Settings logic (save dark mode)
    char conf_data[2] = { dark_mode ? '1' : '0', 0 };
    config_write_file("nexus-hub", "theme.cfg", conf_data, 2);
}

int lumia_main(int argc, char** argv) {
    // Load config
    (void)argc;
    (void)argv;
    char conf_read[2];
    if (config_read_file("nexus-hub", "theme.cfg", conf_read, 2) > 0) {
        dark_mode = (conf_read[0] == '1');
    }

    win_id = sys_gui_window("Nexus Hub", WINDOW_WIDTH, WINDOW_HEIGHT);
    if (win_id < 0) return -1;
    
    // Start an event timer (1000ms, repeat)
    timer_id = event_timer_create(win_id, 1000, 1);

    draw_ui();
    
    LumiaEvent ev;
    while (1) {
        // Asynchronous Wait
        if (lumia_wait_event(win_id, &ev, 0xFFFFFFFF) == 1) {
            if (ev.type == LUMIA_EVENT_WINDOW_CLOSE) {
                break;
            } else if (ev.type == LUMIA_EVENT_TIMER) {
                ticks++;
                draw_ui();
            } else if (ev.type == LUMIA_EVENT_MOUSE_DOWN) {
                int mx = ev.data.mouse.x;
                int my = ev.data.mouse.y;
                
                // Toggle theme button
                if (mx >= 10 && mx <= 130 && my >= 100 && my <= 130) {
                    dark_mode = !dark_mode;
                    draw_ui();
                }
                
                // Notify & Sound button
                else if (mx >= 140 && mx <= 260 && my >= 100 && my <= 130) {
                    notification_show("Nexus Hub", "Button clicked! Playing sound...");
                    audio_play("/system/media/login.wav");
                }
                
                // Clipboard button
                else if (mx >= 270 && mx <= 390 && my >= 100 && my <= 130) {
                    clipboard_set_text("Nexus Hub SDK Demo Clipboard Text!");
                    notification_show("Nexus Hub", "Text copied to clipboard.");
                }
            }
        }
    }
    
    sys_gui_close_window(win_id);
    return 0;
}
