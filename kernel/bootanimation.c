#include "bootanimation.h"
#include "../drivers/screen.h"
#include "../libc/function.h"
#include <stdint.h>

void bootanimation()
{
	clear_screen();

	int start_row = MAX_ROWS / 2 - 2;
	kprint_middle_row("Welcome to OS", start_row++);
	kprint_middle_row("Hello, there", start_row++);

	fill_screen_with_color(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED));
	wait_for_seconds(1);
	fill_screen_with_color(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE));
	wait_for_seconds(1);
	fill_screen_with_color(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_GREEN));
	wait_for_seconds(1);
	fill_screen_with_color(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_CYAN));
	wait_for_seconds(1);
	clear_screen();
}