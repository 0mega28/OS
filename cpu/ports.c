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

uint8_t port_byte_in(uint16_t port)
{
    /*
     * The value of port goes as input in DX register
     * and the output of register AL goes into result variable
     */
    uint8_t result;
    asm("in %%dx, %%al"
            : "=a"(result)
            : "d"(port));
    return result;
}

void port_byte_out(uint16_t port, uint8_t data)
{
    /*
     * Both registers are mapped to C variables
     * Output section is empty
     */
    asm volatile("out %%al, %%dx"
            :
            : "a"(data), "d"(port));
}

uint16_t port_word_in(uint16_t port)
{
    uint16_t result;
    asm("in %%dx, %%ax"
            : "=a"(result)
            : "d"(port));
    return result;
}

void port_word_out(uint16_t port, uint16_t data)
{
    asm volatile("out %%ax, %%dx"
            :
            : "a"(data), "d"(port));
}