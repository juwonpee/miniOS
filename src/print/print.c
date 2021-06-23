#include "print.h"
#include "vga_text_print.h"

graphics_mode get_graphics_mode() {
    return graphics_mode_text;
}

void set_graphics_mode(graphics_mode mode) {
    graphics_mode_text = mode;
    if (graphics_mode_text == GRAPHICS_VGA_TEXT_80_25) {
        vga_text_init(VGA_COLOUR_WHITE);
    }
}

void print(char* string) {
    if (graphics_mode_text == GRAPHICS_VGA_TEXT_80_25) {
        vga_text_print(string);
    }
}

void println(char* string) {
    if (graphics_mode_text == GRAPHICS_VGA_TEXT_80_25) {
        vga_text_println(string);
    }
}