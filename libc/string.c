#include "string.h"

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

void reverse(char s[])
{
    int c, i, j;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

int strlen(char s[])
{
    int i = 0;
    while (s[i] != '\0')
        i++;
    return i;
}

void append(char s[], char n)
{
    int len = strlen(s);
    s[len] = n;
    s[len + 1] = '\0';
}

void backspace(char s[])
{
    int len = strlen(s);
    if (len > 0)
        s[len - 1] = '\0';
    else
        s[0] = '\0';
}

/* Returns 0 if s1 == s2 */
int strcmp(char s1[], char s2[])
{
    int i;
    for (i = 0; s1[i] == s2[i]; i++)
        if (s1[i] == '\0')
            return 0;

    return s1[i] - s2[i];
}

void hex_to_ascii(int n, char str[])
{
    append(str, '0');
    append(str, 'x');

    int tmp;
    for (int i = 28; i >= 0; i -= 4)
    {
        if (i != 0)
            tmp = (n >> i) & 0xF;
        else
            tmp = n & 0xF;

        if (tmp >= 0xA)
            append(str, tmp - 0xA + 'A');
        else
            append(str, tmp + '0');
    }
}