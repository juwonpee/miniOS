#include "types.h"

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


uint32_t* terminalBuffer; 			
vga_colour vga_text_colour;
uint8_t vga_text_width;
uint8_t vga_text_height;
uint8_t vga_text_cursor_horizontal;
uint8_t vga_text_cursor_vertical;



void vga_text_init(vga_colour colour);

void vga_text_print(char* string);

void vga_text_println(char* string);

void vga_text_print_char(char string);

void vga_text_returncarrige();

void vga_text_newline();

void vga_text_clear();

vga_colour vga_text_get_colour();
void vga_text_set_colour(vga_colour colour);

char vga_text_get_char(uint8_t x, uint8_t y);
void vga_text_set_char(uint8_t x, uint8_t y, char string);

vga_colour vga_text_get_char_colour(uint8_t x, uint8_t y);
void vga_text_set_char_colour(uint8_t x, uint8_t y, vga_colour colour);