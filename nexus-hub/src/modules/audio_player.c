#include "audio_player.h"
#include <lumia.h>

void audio_player_play_beep(void) {
    audio_play("/system/media/alarm.wav");
}
