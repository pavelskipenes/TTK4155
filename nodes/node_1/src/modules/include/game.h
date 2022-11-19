#pragma once

enum state_t
{
    PLAY,
    HIGHSCORE,
    RESTART
};

/**
 * @brief prints main menu on oled display. Returns selected entry.
 *
 * @param current_state default selection
 * @return enum state_t selected entry
 */
enum state_t game_menu(enum state_t current_state);
