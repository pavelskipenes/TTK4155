#pragma once

enum state_t
{
    PLAY,
    HIGHSCORE,
    RESTART
};
enum state_t game_menu(enum state_t current_state);
