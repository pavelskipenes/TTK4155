#include "menu.h"
#include "oled.h"

void menu_main_print_main_menu(FILE *oled)
{
    const char *play_text = "play";
    fprintf(oled, "         \n");
    fprintf(oled, "         \n");
    fprintf(oled, "         \n");
    fprintf(oled, "         \n");
    fprintf(oled, "         \n");

    fprintf(oled, "%s\n", play_text);
}
