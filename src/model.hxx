#pragma once

#include "character.hxx"
#include "level_layout.hxx"
#include "trap_door.hxx"

#include <ge211.hxx>


class Model
{

public:

    // Constructor
    Model(std::string filename1,
          std::string filename2,
          Game_Config config,
          Level_Layout level_layout);

    // Getters
    Character const& character(Player player) const;
    Trap_Door const& trap_door() const;
    std::vector<ge211::Rect<int>> const& get_map() const;
    Game_Config const& get_game_config() const;
    Level_Layout const& get_level_layout() const;


    void on_frame(double dt);
    void set_key(std::string, char k);
    char get_key(std::string);
    int const get_game_state() const;
    int const get_score() const;
    // ge211::Text_sprite const& get_score_sprite() const;


    // Functions that detect collision
    // Individual block collisions
    bool hit_top_block(Character, ge211::Rect<int>);
    bool hit_bottom_block(Character, ge211::Rect<int>);
    bool hit_left_block(Character, ge211::Rect<int>);
    bool hit_right_block(Character, ge211::Rect<int>);
    // Full-map collision search
    bool hit_top_map(Character);
    bool hit_bottom_map(Character);
    bool hit_left_map(Character);
    bool hit_right_map(Character);
    // Screen edge collision
    std::pair<bool, bool> hit_bottom_screen();
    std::pair<bool, bool> hit_top_screen();
    std::pair<bool, bool> hit_left_screen();
    std::pair<bool, bool> hit_right_screen();
    // Helper function for on_frame
    void compute_next(Character&, double dt);
    // Door, pond, and gems
    bool at_door(Character, ge211::Rect<int>);
    bool touch_pond(Character, ge211::Rect<int>);
    bool touch_gem(Character, ge211::Rect<int>);
    void collect_gems();
    // Trapdoor
    void activate_trap_door();
    bool touch(Character, ge211::Rect<int>);


#ifdef CS211_TESTING
    // When this class is compiled for testing, members of a struct named
    // Test_access will be allowed to access private members of this class.
    friend struct Test_Access;
#endif

private:
    Character fireboy_;
    Character watergirl_;
    char fireboy_horizontal_ = 'n';
    char watergirl_horizontal_ = 'n';
    char fireboy_vertical_ = 'n';
    char watergirl_vertical_ = 'n';
    double duration = 0;
    float tick_ = 0;
    int game_state_ = 0; // 0 for normal, 1 for win, -1 for lose
    int score_ = 0;
    // std::vector<ge211::Text_sprite> score_sprite_;

    Trap_Door trap_door_;
    std::vector<ge211::Rect<int>> map_;
    Game_Config config_;
    Level_Layout level_layout_;
};
