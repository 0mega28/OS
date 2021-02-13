void memory_copy(char *source, char *dest, int nbytes)
{
    for (int i = 0; i < nbytes; i++)
    {
        dest[i] = source[i];
    }
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