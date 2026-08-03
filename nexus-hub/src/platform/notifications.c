#include "notifications.h"
#include <lumia.h>

void notify_show(const char* title, const char* message) {
    notification_show(title, message);
}
