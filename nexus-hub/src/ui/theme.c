#include "theme.h"

NexusTheme theme_get_current(int is_dark) {
    NexusTheme t;
    if (is_dark) {
        t.bg = 0x18181B;         // Zinc 900
        t.fg = 0xF4F4F5;         // Zinc 100
        t.accent = 0x3B82F6;     // Blue 500
        t.button_bg = 0x27272A;  // Zinc 800
        t.button_hover = 0x3F3F46;// Zinc 700
        t.text_muted = 0xA1A1AA; // Zinc 400
    } else {
        t.bg = 0xF8FAFC;         // Slate 50
        t.fg = 0x0F172A;         // Slate 900
        t.accent = 0x2563EB;     // Blue 600
        t.button_bg = 0xE2E8F0;  // Slate 200
        t.button_hover = 0xCBD5E1;// Slate 300
        t.text_muted = 0x64748B; // Slate 500
    }
    return t;
}
