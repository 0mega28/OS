#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/mem.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../kernel/kshell.h"
#include <stdbool.h>

#define BACKSPACE 0X0E
#define ENTER 0X1C

bool is_busy = false;

static char key_buffer[256];

#define SC_MAX 57

const char *sc_name[] = {"ERROR", "Esc", "1", "2", "3", "4", "5", "6",
                         "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
                         "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
                         "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
                         "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
                         "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                         'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
                         'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
                         'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

static void keyboard_callback(registers_t *regs)
{
    /* The PIC leaves us the scancode in port 0x60 */
    uint8_t scancode = port_byte_in(0x60);

    if (!is_busy)
    {
        if (scancode > SC_MAX)
            return;
        else if (scancode == BACKSPACE)
        {
            if (strlen(key_buffer) > 0)
            {
                backspace(key_buffer);
                kprint_backspace();
            }
        }
        else if (scancode == ENTER)
        {
            kprint("\n");
            user_input(key_buffer); /* kernel-controlled function */
            key_buffer[0] = '\0';
        }
        else
        {
            char letter = sc_ascii[(int)scancode];

            /* kprint accepts char [] */
            char str[2] = {letter, '\0'};
            append(key_buffer, letter);
            kprint(str);
        }
    }
    UNUSED(regs);
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
}