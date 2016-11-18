#pragma once

#include "dev_text.h"

extern text_display_base_t *console_display;
extern text_display_vtbl_t console_display_vtbl;

int con_exists(void);
void con_clear(void);
int con_cursor_toggle(int show);
void con_putc(int character);
int con_print(char const *s);
int con_draw_xy(int x, int y, char const *s, int attr);
