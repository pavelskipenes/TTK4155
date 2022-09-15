#include <stdbool.h>
#include <stdio.h>
#include "usart.h"
#include "memory.h"

int main()
{

    memory_init();
    memory_test();
    FILE *uart0 = usart_init(9600);
    char buffer[256] = {0};
    while (true)
    {
        printf("Listening...\n\r");
        fgets(buffer, 30, uart0);
        printf("received: %s\n\r", buffer);

    }
}