#include "game.h"

#include "adc.h"
#include "joystick.h"
#include "oled.h"

#include <avr/io.h>

enum user_choice
{
    UP,
    DOWN,
    SELECT,
    NONE
};

static enum user_choice get_for_user_input();

void game_home(uint8_t selection_index)
{
    oled_reset();
    oled_print("Main menu\n\r");
    oled_print("\n\r");
    oled_print("\n\r");
    oled_print("   Play\n\r");
    oled_print("   High score\n\r");
    oled_print("   Restart?\n\r");

    oled_goto_line(selection_index + 3);
    oled_print("->");
}

enum state_t game_menu(enum state_t current_state)
{
    enum state_t next_state = current_state;
    game_home(current_state);
    bool can_change = true;
    while (true)
    {
        switch (get_for_user_input())
        {
        case UP:
            if (!can_change)
            {
                break;
            }
            can_change = false;
            printf("UP\n");
            current_state--;
            game_home(current_state);
            break;
        case DOWN:
            if (!can_change)
            {
                break;
            }
            can_change = false;
            printf("DOWN\n");
            current_state++;
            game_home(current_state);
            break;
        case SELECT:

            if (!can_change)
            {
                break;
            }
            can_change = false;
            printf("SELECT\n");
            return current_state;
        case NONE:
            can_change = true;
            break;
        }
    }

    return current_state;
}

static enum user_choice get_for_user_input()
{
    struct adc_sample_t sample = adc_read();
    struct joystick_percent_t percent = joystick_get_percent(sample.joystick[ADC_JOYSTICK_X], sample.joystick[ADC_JOYSTICK_Y]);

    if (percent.percent_y > 80)
    {
        return UP;
    }
    if (percent.percent_y < -80)
    {
        return DOWN;
    };
    return NONE;
}