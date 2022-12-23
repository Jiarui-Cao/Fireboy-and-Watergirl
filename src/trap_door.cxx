//
// Created by Jerry C on 12/2/22.
//

#include "trap_door.hxx"

Trap_Door::Trap_Door(
        std::vector<ge211::Rect<int>> traps,
        std::vector<std::pair<ge211::Rect<int>, bool>> doors):
    traps_(traps),
    doors_(doors)
{}


bool const Trap_Door::get_state() const
{
    return state_;
}

std::vector<ge211::Rect<int>> const&
Trap_Door::get_traps() const
{
    return traps_;
}

std::vector<ge211::Rect<int>>
Trap_Door::get_active_doors() const
{
    std::vector<ge211::Rect<int>> result;
    if (!state_) {
        for (auto pair : doors_) {
            if (pair.second) {
                result.push_back(pair.first);
            }
        }
    }
    else if (state_) {
        for (auto pair : doors_) {
            if (!pair.second) {
                result.push_back(pair.first);
            }
        }
    }
    return result;
}

std::vector<ge211::Rect<int>>
Trap_Door::get_inactive_doors() const
{
    std::vector<ge211::Rect<int>> result;
    if (state_) {
        for (auto pair : doors_) {
            if (pair.second) {
                result.push_back(pair.first);
            }
        }
    }
    else if (!state_) {
        for (auto pair : doors_) {
            if (!pair.second) {
                result.push_back(pair.first);
            }
        }
    }
    return result;
}



void Trap_Door::change_state(bool state)
{
    state_ = state;
}


