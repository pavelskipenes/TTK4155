#include "game.h"

#include "adc.h"
#include "joystick.h"
#include "oled.h"

enum state_t game_menu(enum state_t current_state)
{
    enum state_t next_state = PLAY;
    oled_home(current_state);
    bool can_change = true;
    while (true)
    {
        switch (wait_for_user_input())
        {
        case UP:
            if (!can_change)
            {
                break;
            }
            can_change = false;
            printf("UP\n");
            next_state--;
            oled_home(next_state);
            break;
        case DOWN:
            if (!can_change)
            {
                break;
            }
            can_change = false;
            printf("DOWN\n");
            next_state++;
            oled_home(next_state);
            break;
        case SELECT:
            if (!can_change)
            {
                break;
            }
            can_change = false;
            printf("SELECT\n");
            return next_state;
        case NONE:
            can_change = true;
            break;
        }
    }

    return next_state;
}

enum user_choice wait_for_user_input()
{
    while (true)
    {
        struct adc_sample_t sample = adc_read();
        struct joystick_percent_t percent = joystick_get_percent(sample.joystick[JOYSTICK_X], sample.joystick[JOYSTICK_Y]);

        if (percent.percent_y > 80)
        {
            return UP;
        }
        if (percent.percent_y < -80)
        {
            return DOWN;
        }
        return NONE;
    }
}