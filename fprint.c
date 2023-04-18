#include "fprint.h"

static void putnbr(int sum)
{
    char c;

    if (sum > 10)
    {
        putnbr(sum / 10);
        putnbr(sum % 10);
    }
    else
    {
        c = sum + 48;
        write(1, &c, 1);
    }
}

static int char_small(int fd, int i)
{
    int count;
    int c;

    count = 0;
    while (read(fd, &c, 1) > 0)
    {
        if (c == i)
            break;
        write(1, &c, 1);
        count++;
    }
    return (count);
}

static int char_big(int fd, int i)
{
    int count;
    int sum;
    int c;

    count = 0;
    sum = 0;
    while (read(fd, &c, 1) > 0)
    {
        if (c == i)
            sum++;
        count++;
    }
    putnbr(sum);
    return (count);
}

static int pchar(int c)
{
    write(1, &c, 1);
}

static int print_end(int fd)
{
    int c;
    int i;

    i = 0;
    while (read(fd, &c, 1) > 0)
    {
        write(1, &c, 1);
        i++;
    }
    return (i);
}

static int print_ascii(int fd)
{
    int arr[127];
    char c;
    int i;

    i = 0;
    while (i < 127)
        arr[i++] = 0;
    i = 0;
    while (read(fd, &c, 1) > 0)
    {
        arr[c - 1]++;
        i++;
    }
    c = 32;
    while (c < 127)
    {
        if (arr[c - 1] != 0)
        {
            write(1, &c, 1);
            write(1, ": ", 2);
            putnbr(arr[c - 1]);
            write(1, "\n", 1);
        }
        c++;
    }
    return (i);
}

static int print_ascii_all(int fd)
{
    int arr[127];
    char c;
    int i;

    i = 0;
    while (i < 127)
        arr[i++] = 0;
    i = 0;
    while (read(fd, &c, 1) > 0)
    {
        arr[c - 1]++;
        i++;
    }
    c = 32;
    while (c < 127)
    {
        write(1, &c, 1);
        write(1, ": ", 2);
        putnbr(arr[c - 1]);
        write(1, "\n", 1);
        c++;
    }
    return (i);
}

static int index_tiny(int fd, int i)
{
    int i2;
    char c;

    i2 = 0;
    while (i2 < i)
    {
        read(fd, &c, 1);
        write(1, &c, 1);
        i2++;
    }
    return (i2);
}

static int index_big(int fd, int i)
{
    int i2;
    char c;

    i2 = 0;
    while (i2 < i)
    {
        read(fd, &c, 1);
        if (i2 == i - 1)
            write(1, &c, 1);
        i2++;
    }
    return (i2);
}

static int newline_small(int fd, int i)
{
    int nl;
    int count;
    int c;

    nl = 0;
    count = 0;
    while (nl < i)
    {
        if (read(fd, &c, 1) < 0)
            break;
        if (c == '\n')
            nl++;
        write(1, &c, 1);
        count++;
    }
    return (count);
}

static int newline_big(int fd, int i)
{
    int nl;
    int count;
    int c;
    int one;

    nl = 0;
    count = 0;
    one = 0;
    while (nl < i)
    {
        if (read(fd, &c, 1) < 0)
            break;
        if (c == '\n')
            nl++;
        if (nl == i - 1)
        {
            if (one > 0)
                write(1, &c, 1);
            one++;
        }
        count++;
    }
    return (count);
}

int controller(int fd, char param, va_list args)
{
    if (param == 'i')
        return (index_tiny(fd, va_arg(args, int)));
    else if (param == 'I')
        return (index_big(fd, va_arg(args, int)));
    else if (param == 'a')
        return (print_ascii(fd));
    else if (param == 'A')
        return (print_ascii_all(fd));
    else if (param == 'n')
        return (newline_small(fd, va_arg(args, int)));
    else if (param == 'N')
        return (newline_big(fd, va_arg(args, int)));
    else if (param == 'e')
        return (print_end(fd));
    else if (param == 'c') // bis zu dem character
        return (char_small(fd, va_arg(args, int)));
    else if (param == 'C') // wie viele von dem character
        return (char_big(fd, va_arg(args, int)));
}

int fprint(char *filename, char *args, ...)
{
    va_list vargs;
    int fd;
    int i;
    int c;

    va_start(vargs, args);
    i = 0;
    c = 0;
    if (filename == 0 || filename[0] == 0)
    {
        write(1, "File cant be empty\n", 20);
        return (20);
    }
    while (args[i])
    {
        fd = open(filename, O_RDONLY);
        if (args[i] == '%')
        {
            c += controller(fd, args[i + 1], vargs);
            i += 2;
        }
        else
            c += pchar(args[i++]);
        close(fd);
    }
    va_end(vargs);
    return (c);
}