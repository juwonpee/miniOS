#include "types.h"

#define VGA_BUFFER 0xC03FF000

typedef enum {
	VGA_COLOUR_BLACK = 0,
	VGA_COLOUR_BLUE = 1,
	VGA_COLOUR_GREEN = 2,
	VGA_COLOUR_CYAN = 3,
	VGA_COLOUR_RED = 4,
	VGA_COLOUR_MAGENTA = 5,
	VGA_COLOUR_BROWN = 6,
	VGA_COLOUR_LIGHT_GREY = 7,
	VGA_COLOUR_DARK_GREY = 8,
	VGA_COLOUR_LIGHT_BLUE = 9,
	VGA_COLOUR_LIGHT_GREEN = 10,
	VGA_COLOUR_LIGHT_CYAN = 11,
	VGA_COLOUR_LIGHT_RED = 12,
	VGA_COLOUR_LIGHT_MAGENTA = 13,
	VGA_COLOUR_LIGHT_BROWN = 14,
	VGA_COLOUR_WHITE = 15,
} vga_colour;


char* terminalBuffer; 			
vga_colour vga_text_colour;
uint8_t vga_text_width;
uint8_t vga_text_height;
uint8_t vga_text_cursor_horizontal;
uint8_t vga_text_cursor_vertical;

void vga_screen_clear() {
    for (int x = 0; x < vga_text_width; x++) {
        for (int y = 0; y < vga_text_height; y++) {
            vga_text_set_char(x, y, VGA_COLOUR_WHITE);
        }
    }
}

vga_colour vga_text_get_colour() {
    return vga_text_colour;
}

void vga_text_set_colour(vga_colour colour) {
    for (int x = 0; x < vga_text_width; x++) {
        for (int y = 0; y < vga_text_height; y++) {
            vga_text_set_char_colour(x, y, colour);
        }
    }
}

char vga_text_get_char(uint8_t x, uint8_t y) {
    return (char) *(terminalBuffer + 2 * (x + y * vga_text_width));
}
void vga_text_set_char(uint8_t x, uint8_t y, char string) {
    terminalBuffer[2 * (x + y * vga_text_width) + 1] = string;
}

void vga_text_set_char_colour(uint8_t x, uint8_t y, vga_colour colour) {
    terminalBuffer[2 * (x + y * vga_text_width)] = colour;
}

void vga_text_init(vga_colour colour) {
    // TODO: set graphics mode to text
    terminalBuffer = (void*) VGA_BUFFER;
    vga_text_colour = colour;
    vga_text_width = 80;
    vga_text_height = 25;
    vga_text_cursor_horizontal = 0;
    vga_text_cursor_vertical = 0;

    vga_screen_clear();
}

void vga_text_print(char* string) {
    uint32_t length = 0;
    while(1) {
        if ('\0' == string[length]) {
            break;
        }
        vga_text_print_char(string[length]);
        length++;
    }
}

void vga_text_println(char* string) {
    uint32_t length = 0;
    while(1) {
        if ('\0' == string[length]) {
            length--;
            break;
        }
        vga_text_print_char(string[length]);
        length++;
    }
    vga_text_returncarrige();
    vga_text_newline();
}

void vga_text_print_char(char string) {
    if (string == '\0') {
        return;
    }
    else if (string == '\r') {
        vga_text_returncarrige();
    }
    else if (string == '\n') {
        vga_text_newline();
    }
    else {
        if (vga_text_cursor_horizontal == vga_text_width - 1) {
            vga_text_returncarrige();
            vga_text_newline();
        }
        terminalBuffer[2 * (vga_text_cursor_vertical * vga_text_width + vga_text_cursor_horizontal)] = string;
        vga_text_cursor_horizontal++;
    }
}

void vga_text_returncarrige() {
    vga_text_cursor_horizontal = 0;
}

void vga_text_newline() {
    if (vga_text_cursor_vertical == vga_text_height - 1) {

        for (int x = 0; x < vga_text_width-1; x++) {
            for (int y = 0; y < vga_text_height-1; y++) {
                vga_text_set_char(x, y, vga_text_get_char(x, y+1));
            }
        }
        for (int x = 0; x < vga_text_width-1; x++) {
            vga_text_set_char(x, vga_text_cursor_vertical, ' ');
        }
    }
    else {
        vga_text_cursor_vertical++;
    }
}

