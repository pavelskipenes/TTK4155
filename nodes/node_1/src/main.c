#include <stdbool.h>
#include <stdio.h>

#include "usart.h"

int main()
{

    FILE *uart0 = usart_init(9600);
    char buffer[256] = {0};
    while (true)
    {
        printf("Listening...\n\r");
        fgets(buffer, 30, uart0);
        printf("received: %s\n\r", buffer);
    }

    while (1)
    {
    }

}