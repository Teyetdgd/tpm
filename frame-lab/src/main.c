#include <lumia.h>

#define BG       0x00F3F6FCu
#define INK      0x0019233Bu
#define MUTED    0x00657289u
#define ACCENT   0x002F6FEDu
#define CYAN     0x0014B8A6u
#define ORANGE   0x00F59E0Bu
#define CARD     0x00FFFFFFu

static int running = 1;
static int width = 560;
static int height = 350;
static unsigned int frame_no;

static void unsigned_text(unsigned int value, char* out) {
    char reverse[11];
    int count = 0;
    if (!value) reverse[count++] = '0';
    while (value && count < 10) {
        reverse[count++] = (char)('0' + value % 10u);
        value /= 10u;
    }
    for (int i = 0; i < count; i++) out[i] = reverse[count - i - 1];
    out[count] = 0;
}

static void draw_card(int win, int x, int y, int w, int h,
                      const char* title, unsigned int color, int value) {
    sys_gui_rect(win, x, y, w, h, CARD);
    sys_gui_rect(win, x, y, 5, h, color);
    sys_gui_text(win, x + 18, y + 14, title, INK);
    sys_gui_rect(win, x + 18, y + 48, w - 36, 8, 0x00DDE4F0u);
    sys_gui_rect(win, x + 18, y + 48, (w - 36) * value / 100, 8, color);
}

static void draw_frame(int win) {
    char number[12];
    int client_h = height - 23;
    if (client_h < 220) client_h = 220;

    if (lumia_gui_begin_frame(win) != 0) return;

    sys_gui_rect(win, 0, 0, width, client_h, BG);
    sys_gui_rect(win, 0, 0, width, 58, ACCENT);
    sys_gui_text(win, 22, 15, "LumiaSDK Frame Lab", 0x00FFFFFFu);
    sys_gui_text(win, 22, 34, "Atomic canvas publication", 0x00DCE8FFu);

    unsigned_text(frame_no, number);
    sys_gui_text(win, width - 104, 22, "Frame", 0x00DCE8FFu);
    sys_gui_text(win, width - 48, 22, number, 0x00FFFFFFu);

    int card_w = (width - 44) / 2;
    int pulse = (int)(frame_no % 101u);
    draw_card(win, 14, 76, card_w, 82, "Staging canvas", CYAN, pulse);
    draw_card(win, 30 + card_w, 76, card_w, 82,
              "Published frame", ORANGE, 100 - pulse);

    int track_y = 184;
    int track_w = width - 56;
    int dot = track_w > 24 ? (int)(frame_no * 5u % (unsigned int)(track_w - 24)) : 0;
    sys_gui_text(win, 22, track_y - 20, "Motion remains one complete frame", INK);
    sys_gui_rect(win, 22, track_y, track_w, 26, 0x00DDE4F0u);
    sys_gui_rect(win, 22 + dot, track_y, 24, 26, ACCENT);

    sys_gui_text(win, 22, client_h - 45,
                 "Move, resize, cover and uncover this window.", MUTED);
    sys_gui_text(win, 22, client_h - 25,
                 "Close normally when manual inspection is complete.", MUTED);

    lumia_gui_end_frame(win);
    frame_no++;
}

int lumia_main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    if (!(lumia_sdk_capabilities() & LUMIA_CAP_FRAME_TRANSACTIONS)) {
        lumia_serial_log("[FRAME-LAB] Frame transactions are unavailable\n");
        return 1;
    }

    int win = sys_gui_window("LumiaSDK Frame Lab", width, height);
    if (win < 0) return 2;
    sys_gui_get_window_size(win, &width, &height);

    while (running) {
        LumiaEvent event;
        int result = lumia_wait_event(win, &event, 40);
        if (result == 1) {
            if (event.type == LUMIA_EVENT_WINDOW_CLOSE || event.type == LUMIA_EVENT_QUIT)
                running = 0;
            else if (event.type == LUMIA_EVENT_WINDOW_RESIZE ||
                     event.type == LUMIA_EVENT_WINDOW_MAXIMIZE ||
                     event.type == LUMIA_EVENT_WINDOW_RESTORE)
                sys_gui_get_window_size(win, &width, &height);
        }
        if (running) draw_frame(win);
    }

    sys_gui_close_window(win);
    return 0;
}

