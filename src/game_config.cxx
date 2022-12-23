//
// Created by Jerry C on 11/29/22.
//

#include "game_config.hxx"


Game_Config::Game_Config():
    scene_dims ({975, 750}),
    character_dims ({27, 57}),
    horizontal_velocity ({275, 0}),
    jump_velocity ({0, -400}),
    anti_jump_velocity ({0,10}),
    grav_acceleration ({0, 1000}),

    // // Bottom left of the screen
    // fireboy_start_pos (50, 650),
    // watergirl_start_pos (50, 550),

    // // Top of the screen
    // fireboy_start_pos (500, 100),
    // watergirl_start_pos (500, 100),

    // Middle of the screen
    fireboy_start_pos (200, 450),
    watergirl_start_pos (200, 450),
    bricks(),
    character_animation_rate(0.1)
{
}