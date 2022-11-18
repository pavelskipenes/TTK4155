enum user_choice
{
    UP,
    DOWN,
    SELECT,
    NONE
};

enum state_t
{
    PLAY,
    HIGHSCORE,
    RESTART
};
enum state_t game_menu(enum state_t current_state);
enum user_choice wait_for_user_input();