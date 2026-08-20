#include "lumia.h"

static int win_id;

static void touch_apis(void) {
    int fd;
    (void)lumia_sdk_version();
    (void)lumia_sdk_capabilities();
    (void)lumia_error_string(LUMIA_EACCES);
    fd = net_socket(LUMIA_AF_INET, LUMIA_SOCK_STREAM, 0);
    if (fd >= 0) fs_close(fd);
}

static void log_line(const char* line) {
    lumia_serial_log(line);
    lumia_serial_log("\n");
}

static void draw_row(int y, const char* label) {
    sys_gui_text(win_id, 24, y, "[ ]", 0xFF1746A2);
    sys_gui_text(win_id, 62, y, label, 0xFF202020);
}

static void draw_interface(void) {
    sys_gui_rect(win_id, 0, 0, 560, 390, 0xFFF4F7FB);
    sys_gui_text(win_id, 24, 20, "LumiaSDK Manual Test Panel", 0xFF1746A2);
    sys_gui_text(win_id, 24, 42, "Use the interface manually; diagnostics are written to COM1.", 0xFF505050);
    draw_row(78,  "Read ABI version and capability API behavior");
    draw_row(106, "Inspect SDK error strings");
    draw_row(134, "Create, move, resize and focus this window");
    draw_row(162, "Use minimize, maximize and close controls");
    draw_row(190, "Check title, border, shadow and control sharpness");
    draw_row(218, "Click and type to inspect event delivery");
    draw_row(246, "Open another app and verify z-order");
    draw_row(274, "Inspect taskbar entry and restore behavior");
    sys_gui_text(win_id, 24, 326, "Close the window when manual inspection is complete.", 0xFF9A6700);
}

int lumia_main(int argc, char** argv) {
    LumiaEvent event;
    int running = 1;
    (void)argc; (void)argv;
    log_line("[SDK-ABI] LumiaSDK serial diagnostic started");
    touch_apis();
    log_line("[SDK-ABI] ABI version and capabilities queried");
    log_line("[SDK-ABI] Error-string lookup queried");
    log_line("[SDK-ABI] Socket descriptor creation/closure queried");
    win_id = sys_gui_window("SDK ABI Test", 560, 390);
    if (win_id < 0) {
        log_line("[SDK-ABI] GUI window creation failed");
        return 1;
    }
    log_line("[SDK-ABI] GUI surface created");
    draw_interface();
    while (running) {
        int got = lumia_wait_event(win_id, &event, 1000);
        if (got != 1) continue;
        if (event.type == LUMIA_EVENT_WINDOW_CLOSE || event.type == LUMIA_EVENT_QUIT) {
            log_line("[SDK-ABI] Close requested");
            running = 0;
        } else if (event.type == LUMIA_EVENT_WINDOW_MOVE) {
            log_line("[SDK-ABI] Window moved");
        } else if (event.type == LUMIA_EVENT_WINDOW_RESIZE) {
            log_line("[SDK-ABI] Window resized");
        } else if (event.type == LUMIA_EVENT_WINDOW_FOCUS) {
            log_line("[SDK-ABI] Window focused");
        } else if (event.type == LUMIA_EVENT_WINDOW_BLUR) {
            log_line("[SDK-ABI] Window blurred");
        }
    }
    sys_gui_close_window(win_id);
    log_line("[SDK-ABI] Diagnostic finished");
    return 0;
}

