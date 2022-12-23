// //
// // Created by Jerry C on 12/1/22.
// //

#pragma once

#include <ge211.hxx>
// #include "ge211.hxx"
#include <vector>

struct Level_Layout
{
    using Position = ge211::Posn<float>;
    using Velocity = ge211::Dims<float>;
    using Block = ge211::Rect<int>;

    Level_Layout();

    std::vector<ge211::Posn<int>> bricks;
    std::vector<ge211::Posn<int>> half_bricks;
    ge211::Rect<int> fireboy_door;
    ge211::Rect<int> watergirl_door;
    ge211::Rect<int> green_pond;
    ge211::Rect<int> red_pond;
    ge211::Rect<int> blue_pond;
    std::vector<ge211::Rect<int>> red_gems;
    std::vector<ge211::Rect<int>> blue_gems;
    std::vector<ge211::Rect<int>> traps_1;
    std::vector<std::pair<ge211::Rect<int>, bool>> doors_1;
};