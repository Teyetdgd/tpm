#include "storage.h"
#include <lumia.h>

int storage_load_theme_preference(void) {
    char conf_read[2];
    if (config_read_file("nexus-hub", "theme.cfg", conf_read, 2) > 0) {
        return (conf_read[0] == '1') ? 1 : 0;
    }
    return -1;
}

void storage_save_theme_preference(int is_dark) {
    char conf_data[2] = { is_dark ? '1' : '0', 0 };
    config_write_file("nexus-hub", "theme.cfg", conf_data, 2);
}
