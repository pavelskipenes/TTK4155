
#include "sam.h"
#include "uart.h"
#include <sys/stat.h>

// https://interrupt.memfault.com/blog/boostrapping-libc-with-newlib
// TLDR: this provides minimal OS-like features. They are required if some code somehow invokes for instance malloc. Pretty common when using printf() with formatted output.

int fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int lseek(int file, int offset, int whence)
{
    return 0;
}

int close(int fd)
{
    return -1;
}

int _write(int fd, char *buf, int count)
{
    int written = 0;

    for (; count != 0; --count)
    {

        if (uart_putchar((uint8_t)*buf++))
        {
            return -1;
        }
        ++written;
    }
    return written;
}

int _read(int fd, char *buf, int count)
{
    int read = 0;

    for (; count > 0; --count)
    {
        uart_getchar((uint8_t *)buf++);
        read++;
    }

    return read;
}
extern int __ram_end__;
void *_sbrk(int incr)
{
    static unsigned char *heap = NULL;
    unsigned char *prev_heap;

    if (heap == NULL)
    {
        heap = (unsigned char *)&__ram_end__;
    }
    prev_heap = heap;

    heap += incr;

    return prev_heap;
}
int _getpid(void)
{
    return -1;
}

int _isatty(int file)
{
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

void _exit(int status)
{
// Set a breakpoint here to get stack trace from assert statements easier
    __asm("BKPT #0");
}

void _kill(int pid, int sig)
{
    return;
}

int _close(int file)
{
    return -1;
}

int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;

    return 0;
}