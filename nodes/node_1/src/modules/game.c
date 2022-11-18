#include "game.h"

#include "adc.h"
#include "joystick.h"
#include "oled.h"

enum user_choice
{
    UP,
    DOWN,
    SELECT,
    NONE
};

static enum user_choice wait_for_user_input();

enum state_t game_menu(enum state_t current_state)
{
    enum state_t next_state = current_state;
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
            current_state--;
            oled_home(current_state);
            break;
        case DOWN:
            if (!can_change)
            {
                break;
            }
            can_change = false;
            printf("DOWN\n");
            current_state++;
            oled_home(current_state);
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

static enum user_choice wait_for_user_input()
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