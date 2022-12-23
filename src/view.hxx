#pragma once

#include "model.hxx"

class View
{
public:
    explicit View(Model const& model);

    void draw(ge211::Sprite_set& set);

    ge211::Dims<int> initial_window_dimensions() const;

    std::string initial_window_title() const;

private:
    Model const& model_;
    ge211::Image_sprite background_sprite;
    ge211::Image_sprite brick_sprite;
    ge211::Image_sprite half_brick_sprite;
    ge211::Image_sprite fireboy_door_sprite;
    ge211::Image_sprite watergirl_door_sprite;
    ge211::Image_sprite green_pond_sprite;
    ge211::Image_sprite red_pond_sprite;
    ge211::Image_sprite blue_pond_sprite;
    ge211::Image_sprite red_gem_sprite;
    ge211::Image_sprite blue_gem_sprite;
    ge211::Image_sprite victory_sprite;
    ge211::Image_sprite defeat_sprite;
    ge211::Image_sprite scoreboard_sprite;
    ge211::Text_sprite score_title;
    ge211::Image_sprite trapdoor_on_sprite;
    ge211::Image_sprite trapdoor_off_sprite;
    ge211::Image_sprite pressure_plate_sprite;
    std::vector<ge211::Text_sprite> score_sprite_;
};
