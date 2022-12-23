//
// Created by Jerry C on 11/29/22.
//

// #ifndef GAME_CHARACTER_HXX
// #define GAME_CHARACTER_HXX
//
// #endif //GAME_CHARACTER_HXX

#pragma once

#include <ge211.hxx>
// #include "ge211.hxx"
#include <iostream>
#include <string>
#include <vector>
#include "game_config.hxx"


enum class Player
{
    fireboy,
    watergirl,
    neither,
};


class Character{

public:
    using Position = ge211::Posn<float>;
    using Velocity = ge211::Dims<float>;

    // Constructor
    Character(std::string filename,
              Player player,
              Position position,
              Game_Config config);

    // Setters
    void update_velocity(Velocity new_vel);
    void update_horizontal_velocity(float new_vel);
    void update_vertical_velocity(float new_vel);
    void update_alive(bool new_alive);
    void update_in_air(bool new_ia);
    void move(float x, float y);
    void move_to(float x, float y);

    // Getters
    Position const position() const;
    Velocity const velocity() const;
    bool const in_air() const;
    bool const alive() const;

    // Additional Methods
    Character next(double dt, bool bottom, bool left, bool right, bool top);
    Character next(double dt);
    ge211::Image_sprite const& get_sprite() const;
    void update_index();
    void jump();





private:

    Position position_;
    Velocity velocity_{0,0};
    Player player_;
    bool alive_;
    bool in_air_;
    bool at_side;

    std::vector<ge211::Image_sprite> character_static_spites_;
    std::vector<ge211::Image_sprite> character_left_sprites;
    std::vector<ge211::Image_sprite> character_right_sprites;
    unsigned int index_ = 0;

    Velocity horizontal_velocity_;
    Velocity jump_velocity_;
    Velocity anti_jump_velocity_;
    Velocity grav_acceleration_;


public:
};
