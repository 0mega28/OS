#include "../drivers/ports.h"

void main()
{
    /* *** Old Code just for reference ***
    // Create a pointer to a char, and point it to the first text cell of
    // video memory (i.e. the top-left of the screen )
    char* video_memory = (char*) 0xb8000;
    // At the address pointed to by video_memory, store the character ’X ’
    // ( i.e. display ’X’ in the top-left of the screen ).
    *video_memory = 'X';
    */

    /* Screen cursor position: ask VGA control register (0x3d4) for bytes
     * 14 = high byte of cursor and 15 = low byte of cursor. */

    port_byte_out(0x3d4, 14); /* Requesting byte 14: high byte of cursor position */
    /* Data is returned in VGA data register (0x3d5) */
    int position = port_byte_in(0x3d5);
    position <<= 8; /* High byte */

    port_byte_out(0x3d4, 15); /* requesting low byte */
    position += port_byte_in(0x3d5);

    /* VGA 'cells' consist of the character and its control data
     * e.g. 'white on black background', 'red text on white bg', etc */
    int offset_from_vga = position * 2;

    /* Let's write on the current cursor position, we already know how
     * to do that */
    char *vga = (char *)0xb8000;
    vga[offset_from_vga] = 'X';
    vga[offset_from_vga + 1] = 0x0f; /* White text on black background */
}