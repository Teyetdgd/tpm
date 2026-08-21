/*
 * LumiaOS - Notification Center Ring 3 Application
 * 100% Transparent Live Aero Glass Blurred Panel with Darkish Buttons
 * Copyright (c) 2026 Switly. All rights reserved.
 */

#include "lumia.h"

static LumiaNotification notif_items[16];
static int notif_count = 0;
static int unread_count = 0;
static int win_id = -1;

static void refresh_notifications(void) {
    int available = notification_history_count();
    if (available < 0) available = 0;
    if (available > 16) available = 16;
    notif_count = available;
    unread_count = 0;
    for (int i = 0; i < notif_count; i++) {
        if (notification_history_get(i, &notif_items[i]) != 0)
            notif_items[i].active = 0;
        if (notif_items[i].active && !notif_items[i].read) unread_count++;
    }
}

static void draw_notification_center(void) {
    int pw = 320;
    int ph = 600;
    int actual_w = pw;
    sys_gui_get_window_size(win_id, &actual_w, &ph);
    
    // 100% TRANSPARENT BACKGROUND CLEAR - No panel dark rect!
    sys_gui_rect(win_id, 0, 0, pw, ph, 0x00000000);
    
    // Left edge glass border line (#FFFFFF with 40% alpha)
    sys_gui_rect(win_id, 0, 0, 2, ph, 0x60FFFFFF);
    
    // Header Title (Pure Bright Cyan #89B4FA)
    sys_gui_text(win_id, 16, 16, "BILDIRIM MERKEZI", 0xFF89B4FA);
    
    if (unread_count > 0) {
        char unbuf[16];
        unbuf[0] = '['; unbuf[1] = '0' + (unread_count > 9 ? 9 : unread_count);
        unbuf[2] = ' '; unbuf[3] = 'Y'; unbuf[4] = 'e'; unbuf[5] = 'n'; unbuf[6] = 'i'; unbuf[7] = ']'; unbuf[8] = 0;
        sys_gui_text(win_id, 160, 16, unbuf, 0xFFA6E3A1);
    }
    
    // Clear All Button - Darkish Black Background (#181825 with 80% alpha) + Border
    sys_gui_rect(win_id, pw - 84, 12, 68, 24, 0xCC181825);
    sys_gui_rect(win_id, pw - 84, 12, 68, 1, 0x60FFFFFF);
    sys_gui_rect(win_id, pw - 84, 35, 68, 1, 0x30FFFFFF);
    sys_gui_text(win_id, pw - 76, 18, "Temizle", 0xFFCDD6F4);
    
    // Header Divider line
    sys_gui_rect(win_id, 12, 44, pw - 24, 1, 0x40FFFFFF);
    
    // Notification Cards List
    int start_y = 52;
    int card_h = 64;
    int active_items = 0;
    
    for (int i = 0; i < notif_count && i < 6; i++) {
        LumiaNotification* item = &notif_items[i];
        if (!item->active) continue;
        active_items++;
        
        int cy = start_y + ((active_items - 1) * (card_h + 8));
        if (cy + card_h > ph - 80) break;
        
        int cx = 12;
        int cw = pw - 24;
        
        // Darkish Black Translucent Card Background (#1E1E2E with alpha)
        uint32_t bg_col = item->read ? 0xB0141622 : 0xD01E1E2E;
        sys_gui_rect(win_id, cx, cy, cw, card_h, bg_col);
        
        // Card Subtle Glass Borders
        sys_gui_rect(win_id, cx, cy, cw, 1, 0x40FFFFFF);
        sys_gui_rect(win_id, cx, cy + card_h - 1, cw, 1, 0x20FFFFFF);
        sys_gui_rect(win_id, cx + cw - 1, cy, 1, card_h, 0x20FFFFFF);
        
        // Accent Bar
        uint32_t accent = 0xFF89B4FA; // Cyan info
        if (item->type == 1) accent = 0xFFA6E3A1; // Emerald success
        else if (item->type == 2) accent = 0xFFF9E2AF; // Amber warning
        else if (item->type == 3) accent = 0xFFCBA6F7; // Lavender media
        sys_gui_rect(win_id, cx, cy, 4, card_h, accent);
        
        // App Name
        sys_gui_text(win_id, cx + 12, cy + 8, item->app_name, 0xFF89B4FA);
        
        // Unread Dot (#F38BA8)
        if (!item->read) {
            sys_gui_rect(win_id, cx + cw - 28, cy + 10, 6, 6, 0xFFF38BA8);
        }
        
        // Dismiss X button
        sys_gui_text(win_id, cx + cw - 14, cy + 6, "x", 0xFFCDD6F4);
        
        // Title (#FFFFFF)
        sys_gui_text(win_id, cx + 12, cy + 24, item->title, 0xFFFFFFFF);
        
        // Body (#CDD6F4)
        char bbuf[32];
        int bi = 0;
        while (item->body[bi] && bi < 26) { bbuf[bi] = item->body[bi]; bi++; }
        if (item->body[bi]) { bbuf[bi++] = '.'; bbuf[bi++] = '.'; }
        bbuf[bi] = 0;
        sys_gui_text(win_id, cx + 12, cy + 42, bbuf, 0xFFCDD6F4);
    }
    
    if (active_items == 0) {
        sys_gui_text(win_id, 70, ph / 2 - 20, "Henuz bildirim yok", 0xFFCDD6F4);
    }
    
    // Quick Action Footer Divider
    int qy = ph - 70;
    sys_gui_rect(win_id, 12, qy, pw - 24, 1, 0x40FFFFFF);
    qy += 10;
    
    int qw = 64, qh = 44;
    // Darkish Black Quick Action Buttons (#181825 with 85% opacity + subtle borders)
    uint32_t btn_bg = 0xD0181825;
    uint32_t btn_border = 0x50FFFFFF;
    
    // Quick Button 1: Tema
    sys_gui_rect(win_id, 14, qy, qw, qh, btn_bg);
    sys_gui_rect(win_id, 14, qy, qw, 1, btn_border);
    sys_gui_text(win_id, 26, qy + 16, "Tema", 0xFF89B4FA);
    
    // Quick Button 2: Ag
    sys_gui_rect(win_id, 88, qy, qw, qh, btn_bg);
    sys_gui_rect(win_id, 88, qy, qw, 1, btn_border);
    sys_gui_text(win_id, 108, qy + 16, "Ag", 0xFFA6E3A1);
    
    // Quick Button 3: Ses
    sys_gui_rect(win_id, 162, qy, qw, qh, btn_bg);
    sys_gui_rect(win_id, 162, qy, qw, 1, btn_border);
    sys_gui_text(win_id, 180, qy + 16, "Ses", 0xFFF9E2AF);
    
    // Quick Button 4: Test
    sys_gui_rect(win_id, 236, qy, qw, qh, btn_bg);
    sys_gui_rect(win_id, 236, qy, qw, 1, btn_border);
    sys_gui_text(win_id, 248, qy + 16, "+Test", 0xFFCBA6F7);
}

int lumia_main(int argc, char** argv) {
    (void)argc; (void)argv;
    
    uint32_t flags = LUMIA_WIN_FRAMELESS | LUMIA_WIN_NO_TASKBAR | LUMIA_WIN_DRAWER_RIGHT | LUMIA_WIN_ALWAYS_ON_TOP;
    win_id = sys_gui_window_ex("Bildirim Merkezi", 320, 600, flags);
    if (win_id < 0) return 1;
    
    // Enable real Aero Glass Frosted Blur backdrop on Ring 3 application!
    sys_gui_set_glass(win_id, 1, 210, 0x12131C);
    notification_history_action(LUMIA_NOTIFICATION_MARK_ALL_READ);
    
    LumiaEvent ev;
    int is_running = 1;
    
    while (is_running) {
        if (lumia_wait_event(win_id, &ev, 33) == 1) {
            if (ev.type == LUMIA_EVENT_WINDOW_CLOSE) {
                is_running = 0;
            }
            else if (ev.type == LUMIA_EVENT_NOTIFICATION_CLICK) {
                notification_history_action(LUMIA_NOTIFICATION_MARK_ALL_READ);
            }
            else if (ev.type == LUMIA_EVENT_MOUSE_DOWN) {
                int mx = ev.data.mouse.local_x;
                int my = ev.data.mouse.local_y;
                
                // Clear All Button
                if (mx >= 236 && mx <= 304 && my >= 12 && my <= 36) {
                    notification_history_action(LUMIA_NOTIFICATION_CLEAR_ALL);
                }
                
                // Quick Actions
                int window_w = 320, window_h = 600;
                sys_gui_get_window_size(win_id, &window_w, &window_h);
                int qy = window_h - 60;
                if (my >= qy && my <= qy + 44) {
                    if (mx >= 14 && mx <= 78) {
                        notification_show("Tema Servisi", "Karanlik tema aktif.");
                    }
                    else if (mx >= 88 && mx <= 152) {
                        notification_show("NetInit", "Ag arayuzu kontrol ediliyor.");
                    }
                    else if (mx >= 162 && mx <= 226) {
                        audio_play("notify.wav");
                        notification_show("Audio Alert", "Sesli uyari calindi.");
                    }
                    else if (mx >= 236 && mx <= 300) {
                        notification_show("DemoApp", "Yeni test bildirimi olusturuldu!");
                    }
                }
                
                // Dismiss card X buttons
                int start_y = 52;
                int card_h = 64;
                int visible_row = 0;
                for (int i = 0; i < notif_count; i++) {
                    if (!notif_items[i].active) continue;
                    int cy = start_y + (visible_row * (card_h + 8));
                    if (mx >= 280 && mx <= 304 && my >= cy + 4 && my <= cy + 24) {
                        notification_history_action(i);
                    }
                    visible_row++;
                }
            }
        }
        refresh_notifications();
        draw_notification_center();
    }
    
    sys_gui_close_window(win_id);
    return 0;
}

