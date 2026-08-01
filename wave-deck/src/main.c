#include "lumia.h"

static int win_id, play_button, pause_button, stop_button, volume_slider;
static const char* audio_path = "test.wav";
static const char* status = "Ready";

static void tick(void* context) {
    (void)context;
    sys_gui_rect(win_id, 0, 0, 500, 240, 0x00101620);
    sys_gui_text(win_id, 18, 16, "Wave Deck", 0x007FDBFF);
    sys_gui_text(win_id, 18, 44, audio_path, 0x00C6D4E1);
    sys_gui_text(win_id, 18, 70, status, 0x00FFD166);

    int seek = sys_gui_draw_seekbar(win_id, 18, 102, 464, 18);
    if (seek >= 0) audio_seek_permille(seek);
    sys_gui_text(win_id, 18, 132, "Volume", 0x00C6D4E1);
    sys_gui_draw_slider(volume_slider, 84, 126, 200, 26);
    audio_set_volume(sys_gui_get_slider(volume_slider));

    if (sys_gui_draw_button(play_button, 18, 174, 130, 38)) {
        int state = audio_status();
        if (state & 2) { audio_resume(); status = "Playing"; }
        else { status = audio_play(audio_path) == 0 ? "Playing" : "Could not open WAV"; }
    }
    if (sys_gui_draw_button(pause_button, 166, 174, 130, 38)) {
        audio_pause(); status = "Paused";
    }
    if (sys_gui_draw_button(stop_button, 314, 174, 168, 38)) {
        audio_stop(); status = "Stopped";
    }
}

int lumia_main(int argc, char** argv) {
    if (argc > 1) audio_path = argv[1];
    win_id = sys_gui_window("Wave Deck", 500, 240);
    if (win_id < 0) return 1;
    play_button = sys_gui_create_button(win_id, "Play / Resume", 1);
    pause_button = sys_gui_create_button(win_id, "Pause", 0);
    stop_button = sys_gui_create_button(win_id, "Stop", 0);
    volume_slider = sys_gui_create_slider(win_id, 0, 10, audio_get_volume());
    sys_gui_set_tick(win_id, (void*)tick);
    sys_stay_resident();
    return 0;
}
