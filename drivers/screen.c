#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"
#include "../libc/string.h"

/* Declaration of private functions */
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

/* Current color attributes of shell */
char color_attribute = WHITE_ON_BLACK;

/*****************************************************
 *          Public Kernel API functions              *
 *****************************************************/

/*
 * Print a message on the specified location
 * If col, row are negative, we will use the current offset
 */

void kprint_at(char *message, int col, int row)
{
	int offset;
	if (col >= 0 && row >= 0)
		offset = get_offset(col, row);
	else
	{
		offset = get_cursor_offset();
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}

	/* Loop through the message and print it */
	int i = 0;
	while (message[i] != 0)
	{
		offset = print_char(message[i++], col, row, color_attribute);
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
}

void kprint(char *message)
{
	kprint_at(message, -1, -1);
}

void kprint_backspace()
{
	int offset = get_cursor_offset() - 2;
	int row = get_offset_row(offset);
	int col = get_offset_col(offset);
	offset = print_char(' ', col, row, color_attribute);
	set_cursor_offset(offset - 2);
}

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

void fill_screen_with_color(uint8_t color)
{
	int screen_size = MAX_COLS * MAX_ROWS;
	char *screen = (char *)VIDEO_ADDRESS;

	for (int i = 0; i < screen_size; i++)
	{
		screen[2 * i + 1] = color;
	}
}

void fill_row_with_color(uint8_t color, int row)
{
	int screen_size = MAX_COLS;
	char *screen = (char *)VIDEO_ADDRESS;

	for (int i = 0; i < screen_size; i++)
	{
		screen[2 * row * MAX_COLS + 2 * i + 1] = color;
	}
}

void change_text_color(uint8_t color)
{
	int screen_size = MAX_COLS * MAX_ROWS;
	char *screen = (char *)VIDEO_ADDRESS;

	for (int i = 0; i < screen_size; i++)
	{
		screen[2 * i + 1] = color;
	}
}

void kprint_middle_row(char *message, int row)
{
	int col_offset = ((int)MAX_COLS - strlen(message)) / 2;
	kprint_at(message, col_offset, row);
}

/*****************************************************
 *          Private Kernel functions                 * 
 *****************************************************/

/*
 * Innermost print function for our kernel, directly accesses the video memory 
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */

int print_char(char c, int col, int row, char attr)
{
	unsigned char *screen = (unsigned char *)VIDEO_ADDRESS;
	if (!attr)
		attr = WHITE_ON_BLACK;

	/* Wrong coordinates, Print red E at the end*/
	if (col >= MAX_COLS || row >= MAX_ROWS)
	{
		int last_address = 2 * MAX_COLS * MAX_ROWS;
		screen[last_address - 2] = 'E';
		screen[last_address - 1] = RED_ON_WHITE;
		return get_offset(col, row);
	}

	int offset;
	if (col >= 0 && row >= 0)
		offset = get_offset(col, row);
	else /* If offset not defined write at curret cursor position */
		offset = get_cursor_offset();

	/* Print new Line char*/
	if (c == '\n')
	{
		row = get_offset_row(offset);
		offset = get_offset(0, row + 1); /* Setting the cursor to next row */
	}
	else if (c == '\t')
	{
		offset += 10;
	}
	else
	{
		screen[offset] = c;
		screen[offset + 1] = attr;
		offset += 2; /* Setting the cursor for next char */
	}

	/* Check if the offset is over screen size and scroll */
	if (offset >= MAX_ROWS * MAX_COLS * 2)
	{
		for (int i = 1; i < MAX_ROWS; i++)
		{
			memory_copy((char *)(get_offset(0, i) + VIDEO_ADDRESS),
						(char *)(get_offset(0, i - 1) + VIDEO_ADDRESS),
						2 * MAX_COLS);
		}

		/* For Last line set it to blank */
		char *last_line = (char *)get_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS;
		for (int i = 0; i < MAX_COLS * 2; i++)
			last_line[i] = 0;

		offset -= 2 * MAX_COLS;
	}

	set_cursor_offset(offset);
	return offset;
}

int get_cursor_offset()
{
	/* Use the VGA ports to get the current cursor position
     * 1. Ask for high byte of the cursor offset (data 14)
     * 2. Ask for low byte (data 15)
     */

	port_byte_out(REG_SCREEN_CTRL, 14);
	int offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte */
	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA); /* Low byte */
	return offset * 2;
}

void set_cursor_offset(int offset)
{
	/* here we write data */
	offset /= 2;
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen()
{
	int screen_size = MAX_COLS * MAX_ROWS;
	char *screen = (char *)VIDEO_ADDRESS;

	for (int i = 0; i < screen_size; i++)
	{
		screen[i * 2] = ' ';
		screen[i * 2 + 1] = color_attribute;
	}
	set_cursor_offset(get_offset(0, 0));
}

int get_offset(int col, int row)
{
	return 2 * (row * MAX_COLS + col);
}

int get_offset_row(int offset)
{
	return offset / (2 * MAX_COLS);
}

int get_offset_col(int offset)
{
	return (offset - get_offset_row(offset) * 2 * MAX_COLS) / 2;
}