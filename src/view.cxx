#include "view.hxx"

using Color = ge211::Color;
using Font = ge211::Font;

View::View(const Model& model)
        : model_(model),
          background_sprite("Background5.jpeg"),
          brick_sprite("Brick3.png"),
          half_brick_sprite("Brick3_Half.png"),
          fireboy_door_sprite("Door_Fireboy.png"),
          watergirl_door_sprite("Door_Watergirl.png"),
          green_pond_sprite("Pond_Green.png"),
          red_pond_sprite("Pond_Red.png"),
          blue_pond_sprite("Pond_Blue.png"),
          red_gem_sprite("Gem_Red.png"),
          blue_gem_sprite("Gem_Blue.png"),
          victory_sprite("Victory.png"),
          defeat_sprite("Defeat.png"),
          scoreboard_sprite("Scoreboard.png"),
          score_title("SCORE:", {"sans.ttf", 25}),
          trapdoor_on_sprite("Trapdoor_1.png"),
          trapdoor_off_sprite("Trapdoor_2.png"),
          pressure_plate_sprite("Pressure_Plate.png")
{
    for (int i=0; i<=8; i++) {
        ge211::Text_sprite score(std::to_string(i), {"sans.ttf", 25});
        score_sprite_.push_back(score);
    }
}

void
View::draw(ge211::Sprite_set& set)
{
    // Character sprites
    if (model_.get_game_state() == 0) {
        set.add_sprite(model_.character(Player::fireboy).get_sprite(),
                       model_.character(Player::fireboy).position().into<int>(),
                       10);
        set.add_sprite(model_.character(Player::watergirl).get_sprite(),
                       model_.character(Player::watergirl).position().into<int>(),
                       10);
    }
    if (model_.get_game_state() == -1) {
        if (model_.character(Player::fireboy).alive()) {
            set.add_sprite(model_.character(Player::fireboy).get_sprite(),
                           model_.character(Player::fireboy).position().into<int>(),
                           10);
        }
        if (model_.character(Player::watergirl).alive()) {
            set.add_sprite(model_.character(Player::watergirl).get_sprite(),
                           model_.character(Player::watergirl).position().into<int>(),
                           10);
        }
    }

    // Background & scoreboard sprite
    set.add_sprite(background_sprite, {0,0}, 0);
    int x = (model_.get_game_config().scene_dims.width - 331) / 2 - 10;
    set.add_sprite(scoreboard_sprite, {x,0}, 3);
    set.add_sprite(score_title, {x + 125,2}, 4);
    set.add_sprite(score_sprite_.at(model_.get_score()),
                   {x + 230,2}, 4);

    // Brick wall sprites
    for (auto brick : model_.get_map()) {
        int block_height = model_.get_game_config().scene_dims.height / 30;
        if (brick.height == block_height) {
            set.add_sprite(brick_sprite, brick.top_left(), 2);
        }
        else {
            set.add_sprite(half_brick_sprite, brick.top_left(), 2);
        }
    }

    // Trapdoor sprites
    for (auto trapdoor : model_.trap_door().get_active_doors()) {
        set.add_sprite(trapdoor_on_sprite, trapdoor.top_left(), 2);
    }
    for (auto trapdoor : model_.trap_door().get_inactive_doors()) {
        set.add_sprite(trapdoor_off_sprite, trapdoor.top_left(), 2);
    }
    for (auto trap : model_.trap_door().get_traps()) {
        set.add_sprite(pressure_plate_sprite, trap.top_left(), 2);
    }

    // Escape door sprites
    set.add_sprite(fireboy_door_sprite,
                   model_.get_level_layout().fireboy_door.top_left(), 1);
    set.add_sprite(watergirl_door_sprite,
                   model_.get_level_layout().watergirl_door.top_left(), 1);

    // Pond sprites
    ge211::Posn<int> green = model_.get_level_layout().green_pond.top_left();
    ge211::Posn<int> red = model_.get_level_layout().red_pond.top_left();
    ge211::Posn<int> blue = model_.get_level_layout().blue_pond.top_left();
    green.y -= 5;
    red.y -= 5;
    blue.y -= 5;
    set.add_sprite(green_pond_sprite,green, 1);
    set.add_sprite(red_pond_sprite,red, 1);
    set.add_sprite(blue_pond_sprite,blue, 1);

    // Gem sprites
    for (auto red_gem : model_.get_level_layout().red_gems) {
        set.add_sprite(red_gem_sprite, red_gem.top_left(),3);
    }
    for (auto blue_gem : model_.get_level_layout().blue_gems) {
        set.add_sprite(blue_gem_sprite, blue_gem.top_left(),3);
    }

    // Victory & defeat sprite
    if (model_.get_game_state() == 1) {
        set.add_sprite(victory_sprite, {50,264}, 20);
    }
    else if (model_.get_game_state() == -1) {
        set.add_sprite(defeat_sprite, {50,264},20);
    }
}


ge211::Dims<int>
View::initial_window_dimensions() const
{
    return model_.get_game_config().scene_dims;
}


std::string
View::initial_window_title() const
{
    return "Fireboy and Watergirl";
}