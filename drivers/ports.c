#include "ports.h"

/*
 * Read a byte from the specified port   
 */

/*
 * Inline Assembly
 * For more read https://www.codeproject.com/Articles/15971/Using-Inline-Assembly-in-C-C
 * The order is source first and destination last unlike Intel convention
 * Register names are prefixed with %
 * suffix should be used in opcode, b(8-bit), w(16-bit), l(32-bit)
 * Immediate operand are marked with a $ "addl $5, %eax"
 * for variable if operand is prefixed with $ it denotes it's address
 *  otherwise it's content
 * movl 8(%ebp), %eax moves the content at offset 8 from the cell pointed by ebp
 * 
 * +---+--------------------+
 * | r |    Register(s)     |
 * +---+--------------------+
 * | a |   %eax, %ax, %al   |
 * | b |   %ebx, %bx, %bl   |
 * | c |   %ecx, %cx, %cl   |
 * | d |   %edx, %dx, %dl   |
 * | S |   %esi, %si        |
 * | D |   %edi, %di        |
 * +---+--------------------+
 */

u8 port_byte_in(u16 port)
{
    /*
     * The value of port goes as input in DX register
     * and the output of register AL goes into result variable
     */
    u8 result;
    __asm__("in %%dx, %%al"
            : "=a"(result)
            : "d"(port));
    return result;
}

void port_byte_out(u16 port, u8 data)
{
    /*
     * Both registers are mapped to C variables
     * Output section is empty
     */
    __asm__("out %%al, %%dx"
            :
            : "a"(data), "d"(port));
}

u16 port_word_in(u16 port)
{
    u16 result;
    __asm__("in %%dx, %%ax"
            : "=a"(result)
            : "d"(port));
    return result;
}

void port_word_out(u16 port, u16 data)
{
    __asm__("out %%ax, %%dx"
            :
            : "a"(data), "d"(port));
}