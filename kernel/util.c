#include "util.h"

void memory_copy(char *source, char *dest, int nbytes)
{
    for (int i = 0; i < nbytes; i++)
    {
        dest[i] = source[i];
    }
}

void memory_set(u8 *dest, u8 val, u32 len)
{
    u8 *temp = (u8 *)dest;
    for (; len != 0; len--)
        *temp++ = val;
}

void int_to_ascii(int n, char str[])
{
    int sign, i = 0;
    if ((sign = n) < 0)
        n = -1 * n;

    do
    {
        str[i++] = n % 10 + '0';

    } while ((n /= 10) != 0);

    if (sign < 0)
        str[i++] = '-';
    str[i] = '\0';

    // Reverse the string;
    int start = 0, end = i - 1;

    while (start < end)
    {
        char temp = str[start];
        str[start++] = str[end];
        str[end--] = temp;
    }
}