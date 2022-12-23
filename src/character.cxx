//
// Created by Jerry C on 11/29/22.
//
#include "character.hxx"

using namespace ge211;

/////////////////
// Constructor //
/////////////////
Character::Character(std::string filename,
                     Player player,
                     Position position,
                     Game_Config config)
    :position_(position),
     player_(player),
     alive_(true),
     in_air_(false),
     at_side(false)
{
    horizontal_velocity_ = config.horizontal_velocity;
    jump_velocity_ = config.jump_velocity;
    anti_jump_velocity_ = config.anti_jump_velocity;
    grav_acceleration_ = config.grav_acceleration;

    for (int i = 0; i <= 6; i++) {
        std::string file = filename + "_" + std::to_string(i)
                + ".png";
        ge211::Image_sprite img(file);
        character_static_spites_.push_back(img);
    }
    for (int i = 7; i <= 13; i++) {
        std::string file = filename + "_" + std::to_string(i) + ".png";
        ge211::Image_sprite img(file);
        character_left_sprites.push_back(img);
    }
    for (int i = 14; i <= 20; i++) {
        std::string file = filename + "_" + std::to_string(i) + ".png";
        ge211::Image_sprite img(file);
        character_right_sprites.push_back(img);
    }
}


/////////////
// Setters //
/////////////

void Character::update_velocity(Character::Velocity new_vel)
{
    velocity_ = new_vel;
}

void Character::update_horizontal_velocity(float new_vel)
{
    velocity_.width = new_vel;
}

void Character::update_vertical_velocity(float new_vel)
{
    velocity_.height = new_vel;
}

void Character::update_alive(bool new_alive)
{
    alive_ = new_alive;
}

void Character::update_in_air(bool new_ia)
{
    in_air_ = new_ia;
}

void Character::move(float x, float y)
{
    position_.x += x;
    position_.y += y;
}

void Character::move_to(float x, float y)
{
    position_.x = x;
    position_.y = y;
}





/////////////
// Getters //
/////////////

Character::Position const
Character::position() const
{
    return position_;
}

Character::Velocity const
Character::velocity() const
{
    return velocity_;
}

bool const Character::in_air() const
{
    return in_air_;
}

bool const Character::alive() const
{
    return alive_;
}



////////////////////////
// Additional Methods //
////////////////////////

Character Character::next(double dt,
                          bool bottom,
                          bool left,
                          bool right,
                          bool top)
{
    Character result(*this);
    Character::Velocity update = result.velocity_ * dt;

    if (bottom) {
        if (update.height >= 0) {
            result.velocity_.height = 0;
            result.in_air_ = false;
        }
    }
    if (result.in_air_) {
        result.velocity_.height += grav_acceleration_.height * dt;
    }

    if (top) {
        result.velocity_.height = anti_jump_velocity_.height;
        update.height = anti_jump_velocity_.height;
    }

    if (left) {
        if (update.width <= 0) {
            update.width = 0;
        }
    }
    if (right) {
        if (update.width >= 0) {
            update.width = 0;
        }
    }

    result.position_ += update;
    return result;
}



Character Character::next(double dt)
{
    Character result(*this);
    if (in_air_) {
        result.velocity_.height += grav_acceleration_.height * dt;
    }
    Character::Velocity update = result.velocity_ * dt;
    result.position_ += update;
    return result;
}



ge211::Image_sprite const&
Character::get_sprite() const
{
    if (velocity_.width < 0) {
        return character_left_sprites.at(index_);
    }
    else if (velocity_.width > 0) {
        return character_right_sprites.at(index_);
    }
    else {
        return character_static_spites_.at(index_);
    }
}

void Character::update_index()
{
    index_++;
    if (index_ >= character_static_spites_.size()) {
        index_ = 0;
    }
}

void Character::jump()
{
    if (!in_air_) {
        in_air_ = true;
        velocity_ += jump_velocity_;
    }
}
