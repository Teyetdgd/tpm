#include "system_monitor.h"
#include "../utils/string_utils.h"
#include <lumia.h>

static char pid_str[32] = {0};

const char* system_monitor_get_pid_string(void) {
    if (pid_str[0] == 0) {
        int pid = process_getpid();
        string_itoa(pid, pid_str);
    }
    return pid_str;
}
