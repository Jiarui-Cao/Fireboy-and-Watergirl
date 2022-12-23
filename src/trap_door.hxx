//
// Created by Jerry C on 12/2/22.
//
#pragma once

#include <ge211.hxx>
// #include "ge211.hxx"
#include <vector>
#include <map>

class Trap_Door
{

public:
    // Constructor
    Trap_Door(std::vector<ge211::Rect<int>> traps,
              std::vector<std::pair<ge211::Rect<int>, bool>> doors);

    // Getters
    bool const get_state() const;
    std::vector<ge211::Rect<int>> const& get_traps() const;
    std::vector<ge211::Rect<int>> get_active_doors() const;
    std::vector<ge211::Rect<int>> get_inactive_doors() const;

    // Setter
    void change_state(bool);



private:
    std::vector<ge211::Rect<int>> traps_;
    std::vector<std::pair<ge211::Rect<int>, bool>> doors_;
    bool state_ = false;

    // std::vector<ge211::Image_sprite> trap_activate_sprite;
    // std::vector<ge211::Image_sprite> trap_inactive_sprite;
    // std::vector<ge211::Image_sprite> door_activate_sprite;
    // std::vector<ge211::Image_sprite> door_inactive_sprite;
};