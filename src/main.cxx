#include "controller.hxx"
#include "game_config.hxx"
#include "level_layout.hxx"

int
main()
{
    Controller("Fireboy", "Watergirl", Game_Config(), Level_Layout()).run();

    return 0;
}
