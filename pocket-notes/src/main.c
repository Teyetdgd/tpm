#include "lumia.h"

#define CAPACITY 768
static int win_id, save_button, load_button, copy_button, paste_button;
static char note[CAPACITY];
static int length;
static const char* status = "Type a note";

static void load_note(void) {
    int size = fs_read_file("pocket-notes.txt", note, CAPACITY - 1);
    if (size < 0) { note[0] = 0; length = 0; status = "No saved note yet"; return; }
    length = size; note[length] = 0; status = "Loaded pocket-notes.txt";
}

static void tick(void* context) {
    (void)context;
    LumiaEvent event;
    while (lumia_poll_event(win_id, &event) > 0) {
        if (event.type == LUMIA_EVENT_KEY_CHAR) {
            char ch = event.data.key.character;
            if (ch == 8 && length > 0) note[--length] = 0;
            else if (ch >= 32 && ch < 127 && length < CAPACITY - 1) {
                note[length++] = ch; note[length] = 0;
            }
        }
    }

    sys_gui_rect(win_id, 0, 0, 560, 300, 0x00F2F0E9);
    sys_gui_text(win_id, 16, 14, "Pocket Notes", 0x00263D42);
    sys_gui_rect(win_id, 16, 44, 528, 150, 0x00FFFFFF);
    sys_gui_text(win_id, 26, 58, note, 0x001B263B);
    sys_gui_text(win_id, 16, 206, status, 0x005B6B73);

    if (sys_gui_draw_button(save_button, 16, 238, 120, 38)) {
        status = fs_write_file("pocket-notes.txt", note, length) >= 0 ? "Saved pocket-notes.txt" : "Save failed";
    }
    if (sys_gui_draw_button(load_button, 148, 238, 120, 38)) load_note();
    if (sys_gui_draw_button(copy_button, 280, 238, 120, 38)) {
        clipboard_set_text(note); status = "Copied to clipboard";
    }
    if (sys_gui_draw_button(paste_button, 412, 238, 132, 38)) {
        int size = clipboard_get_text(note, CAPACITY);
        if (size >= 0) { length = size; note[length] = 0; status = "Pasted from clipboard"; }
    }
}

int lumia_main(int argc, char** argv) {
    (void)argc; (void)argv;
    win_id = sys_gui_window("Pocket Notes", 560, 300);
    if (win_id < 0) return 1;
    save_button = sys_gui_create_button(win_id, "Save", 1);
    load_button = sys_gui_create_button(win_id, "Reload", 0);
    copy_button = sys_gui_create_button(win_id, "Copy", 0);
    paste_button = sys_gui_create_button(win_id, "Paste", 0);
    load_note();
    sys_gui_set_tick(win_id, (void*)tick);
    sys_stay_resident();
    return 0;
}

