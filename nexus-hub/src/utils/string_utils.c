#include "string_utils.h"

void string_itoa(int n, char* buf) {
    int i = 0, j = 0;
    char tmp[16];
    if (n == 0) tmp[i++] = '0';
    while (n > 0) { tmp[i++] = (n % 10) + '0'; n /= 10; }
    while (i > 0) { buf[j++] = tmp[--i]; }
    buf[j] = 0;
}

void string_concat(const char* s1, const char* s2, char* out) {
    int i = 0;
    while (s1[i]) { out[i] = s1[i]; i++; }
    int j = 0;
    while (s2[j]) { out[i++] = s2[j++]; }
    out[i] = 0;
}
