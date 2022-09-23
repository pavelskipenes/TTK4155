#include <stdbool.h>
#include <stdio.h>
#include "usart.h"
#include "memory.h"
#include "timer.h"

int main()
{

    FILE *uart0 = usart_init(9600);
    memory_init();
    memory_test();
    timer_init_clock_signal_output();
    char buffer[256] = {0};
    while (true)
    {
        printf("Listening...\n\r");
        fgets(buffer, 30, uart0);
        printf("received: %s\n\r", buffer);

    }
}
