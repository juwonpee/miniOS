#include "types.h"
#include "vga_text_print.h"

typedef enum{
    GRAPHICS_VGA_TEXT_80_25,
}graphics_mode;

graphics_mode graphics_mode_text;

graphics_mode get_graphics_mode();
void set_graphics_mode (graphics_mode mode);

void print(char* string);

void println(char* string);