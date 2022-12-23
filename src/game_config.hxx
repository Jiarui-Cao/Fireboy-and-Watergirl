//
// Created by Jerry C on 11/29/22.
//
//

#pragma once

#include <ge211.hxx>
#include <vector>
// #include "ge211.hxx"


struct Game_Config
{
    using Position = ge211::Posn<float>;
    using Velocity = ge211::Dims<float>;

    Game_Config();

    ge211::Dims<int> scene_dims;
    ge211::Dims<int> character_dims;
    Velocity horizontal_velocity;
    Velocity jump_velocity;
    Velocity anti_jump_velocity;
    Velocity grav_acceleration;
    Position fireboy_start_pos;
    Position watergirl_start_pos;
    std::vector<ge211::Posn<int>> bricks;
    float character_animation_rate;
};