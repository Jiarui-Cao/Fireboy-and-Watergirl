#include "model.hxx"

Model::Model(std::string filename1,
             std::string filename2,
             Game_Config config,
             Level_Layout level_layout)
        : fireboy_(filename1, Player::fireboy, config.fireboy_start_pos, config),
          watergirl_(filename2, Player::watergirl, config.watergirl_start_pos, config),
          trap_door_(level_layout.traps_1, level_layout.doors_1),
          map_(),
          config_(config),
          level_layout_(level_layout)
{
    int brick_width = config_.scene_dims.width / 39;
    int brick_height = config_.scene_dims.height / 30;
    for (auto brick : level_layout.bricks) {
        map_.push_back(ge211::Rect<int>(brick.x * brick_width,
                                        brick.y * brick_height,
                                        brick_width,
                                        brick_height));
    }
    for (auto brick : level_layout.half_bricks) {
        map_.push_back(ge211::Rect<int>(brick.x * brick_width,
                                        brick.y * brick_height + brick_height/2,
                                        brick_width,
                                        brick_height/2));
    }
    // for (int i=0; i<=8; i++) {
    //     ge211::Text_sprite score(std::to_string(i), {"sans.ttf", 25});
    //     score_sprite_.push_back(score);
    // }
}


///////////////////
// CORE FUNCTION //
///////////////////
void Model::on_frame(double dt)
{
    // Update the character's alive-ness
    if (touch_pond(fireboy_, level_layout_.blue_pond)
        || touch_pond(fireboy_, level_layout_.green_pond)) {
        fireboy_.update_alive(false);
    }
    if (touch_pond(watergirl_, level_layout_.red_pond)
        || touch_pond(watergirl_, level_layout_.green_pond)) {
        watergirl_.update_alive(false);
    }

    // Check if the game is supposed to end
    if (!fireboy_.alive() || !watergirl_.alive()) {
        game_state_ = -1;
    }

    // Check if the game is supposed to win
    if (at_door(fireboy_, level_layout_.fireboy_door)) {
        if (at_door(watergirl_, level_layout_.watergirl_door)) {
            game_state_ = 1;
        }
    }

    collect_gems();
    activate_trap_door();

    // "Flying frame": in the first few seconds, we want the characters to
    // float because otherwise they will fall off the map as the bricks hasn't
    // been fully initialized. once tick_ >= 3, we begin applying
    // gravitational acceleration
    tick_ += dt;

    // Having those checked, now let's actually begin updating the frame
    duration += dt;
    if (duration >= config_.character_animation_rate) {
        fireboy_.update_index();
        watergirl_.update_index();
        duration = 0;
    }

    if (watergirl_horizontal_ == 'l') {
        watergirl_.update_horizontal_velocity(-config_.horizontal_velocity.width);
    }
    if (watergirl_horizontal_ == 'r') {
        watergirl_.update_horizontal_velocity(config_.horizontal_velocity.width);
    }
    if (watergirl_horizontal_ == 'n') {
        watergirl_.update_horizontal_velocity(0);
    }
    if (watergirl_vertical_ == 'u') {
        watergirl_.jump();
        watergirl_vertical_ = 'n';
    }

    if (fireboy_horizontal_ == 'l') {
        fireboy_.update_horizontal_velocity(-config_.horizontal_velocity.width);
    }
    if (fireboy_horizontal_ == 'r') {
        fireboy_.update_horizontal_velocity(config_.horizontal_velocity.width);
    }
    if (fireboy_horizontal_ == 'n') {
        fireboy_.update_horizontal_velocity(0);
    }
    if (fireboy_vertical_ == 'u') {
        fireboy_.jump();
        fireboy_vertical_ = 'n';
    }

    compute_next(fireboy_, dt);
    compute_next(watergirl_, dt);

    // Now we actually update the positions
    fireboy_ = fireboy_.next(dt);
    watergirl_ = watergirl_.next(dt);
}



void Model::set_key(std::string target, char k)
{
    if (target == "FH") {
        fireboy_horizontal_ = k;
    }
    if (target == "FV") {
        fireboy_vertical_ = k;
    }
    if (target == "WH") {
        watergirl_horizontal_ = k;
    }
    if (target == "WV") {
        watergirl_vertical_ = k;
    }
}

char Model::get_key(std::string target)
{
    if (target == "FH") {
        return fireboy_horizontal_;
    }
    if (target == "FV") {
        return fireboy_vertical_;
    }
    if (target == "WH") {
        return watergirl_horizontal_;
    }
    if (target == "WV") {
        return watergirl_vertical_;
    }
    return 'z';
}

Character const&
Model::character(Player player) const
{
    if (player == Player::fireboy) {
        return fireboy_;
    } else {
        return watergirl_;
    }
}

Trap_Door const&
Model::trap_door() const
{
    return trap_door_;
}

int const Model::get_game_state() const
{
    return game_state_;
}

int const Model::get_score() const
{
    return score_;
}

// ge211::Text_sprite const&
// Model::get_score_sprite() const
// {
//     return score_sprite_.at(score_);
// }

std::vector<ge211::Rect<int>> const&
Model::get_map() const
{
    return map_;
}

Game_Config const&
Model::get_game_config() const
{
    return config_;
}

Level_Layout const&
Model::get_level_layout() const
{
    return level_layout_;
}




//////////////////////////////////////////
// Individual Block Collision Detectors //
//////////////////////////////////////////

bool Model::hit_top_block(Character character, ge211::Rect<int> block)
{
    if ((character.position().x < block.top_right().x
         && character.position().x + config_.character_dims.width >
            block.top_right().x)
        || (character.position().x + config_.character_dims.width >
            block.top_left().x
            && character.position().x < block.top_left().x)) {
        if (character.position().y < block.bottom_left().y && character.
        position().y + config_.character_dims.height > block.bottom_left()
        .y) {
            return true;
        }
    }
    return false;
}


bool Model::hit_bottom_block(Character character, ge211::Rect<int> block)
{
    if ((character.position().x < block.top_right().x
         && character.position().x + config_.character_dims.width >
            block.top_right().x)
        || (character.position().x + config_.character_dims.width >
            block.top_left().x
            && character.position().x < block.top_left().x)) {
        if (character.position().y + config_.character_dims.height > block
                .top_left().y && character.position().y < block.top_left().y) {
            return true;
        }
    }
    return false;
}


bool Model::hit_left_block(Character character, ge211::Rect<int> block)
{
    if ((character.position().y < block.bottom_left().y
         && character.position().y + config_.character_dims.height > block
            .bottom_left().y)
        || (character.position().y + config_.character_dims.height > block
            .top_left().y
            && character.position().y < block.top_left().y)) {
        if (character.position().x < block.top_right().x && character
        .position().x + config_.character_dims.width > block.top_right().x) {
            return true;
        }
    }
    return false;
}


bool Model::hit_right_block(Character character, ge211::Rect<int> block)
{
    if ((character.position().y < block.bottom_left().y
         && character.position().y + config_.character_dims.height > block
            .bottom_left().y)
        || (character.position().y + config_.character_dims.height > block
            .top_left().y
            && character.position().y < block.top_left().y)) {
        if (character.position().x + config_.character_dims.width
            > block.top_left().x && character.position().x < block.top_left().x) {
            return true;
        }
    }
    return false;
}




///////////////////////////////
// Full-Map Collision Search //
///////////////////////////////

bool Model::hit_top_map(Character character)
{
    for (auto block : map_) {
        if (hit_top_block(character, block)) {
            return true;
        }
    }
    for (auto trapdoor : trap_door_.get_active_doors()) {
        if (hit_top_block(character, trapdoor)) {
            return true;
        }
    }
    return false;
}

bool Model::hit_bottom_map(Character character)
{
    for (auto block : map_) {
        if (hit_bottom_block(character, block)) {
            return true;
        }
    }
    for (auto trapdoor : trap_door_.get_active_doors()) {
        if (hit_bottom_block(character, trapdoor)) {
            return true;
        }
    }
    return false;
}

bool Model::hit_left_map(Character character)
{
    for (auto block : map_) {
        if (hit_left_block(character, block)) {
            return true;
        }
    }
    for (auto trapdoor : trap_door_.get_active_doors()) {
        if (hit_left_block(character, trapdoor)) {
            return true;
        }
    }
    return false;
}

bool Model::hit_right_map(Character character)
{
    for (auto block : map_) {
        if (hit_right_block(character, block)) {
            return true;
        }
    }
    for (auto trapdoor : trap_door_.get_active_doors()) {
        if (hit_right_block(character, trapdoor)) {
            return true;
        }
    }
    return false;
}





/////////////////////////////////////
// Screen Edge Collision Detectors //
/////////////////////////////////////

std::pair<bool, bool>
Model::hit_bottom_screen()
{
    bool fireboy_hit = false;
    bool watergirl_hit = false;
    if (config_.scene_dims.height <=
        fireboy_.position().y + config_.character_dims.height) {
        fireboy_hit = true;
    }
    if (config_.scene_dims.height <=
        watergirl_.position().y + config_.character_dims.height) {
        watergirl_hit = true;
    }
    return {fireboy_hit, watergirl_hit};
}

std::pair<bool, bool>
Model::hit_top_screen()
{
    bool fireboy_hit = false;
    bool watergirl_hit = false;
    if (fireboy_.position().y <= 0) {
        fireboy_hit = true;
    }
    if (watergirl_.position().y <= 0) {
        watergirl_hit = true;
    }
    return {fireboy_hit, watergirl_hit};
}

std::pair<bool, bool>
Model::hit_left_screen()
{
    bool fireboy_hit = false;
    bool watergirl_hit = false;
    if (fireboy_.position().x <= 0) {
        fireboy_hit = true;
    }
    if (watergirl_.position().x <= 0) {
        watergirl_hit = true;
    }
    return {fireboy_hit, watergirl_hit};
}

std::pair<bool, bool>
Model::hit_right_screen()
{
    bool fireboy_hit = false;
    bool watergirl_hit = false;
    if (fireboy_.position().x + config_.character_dims.width >=
        config_.scene_dims.width) {
        fireboy_hit = true;
    }
    if (watergirl_.position().x + config_.character_dims.width >=
        config_.scene_dims.width) {
        watergirl_hit = true;
    }
    return {fireboy_hit, watergirl_hit};
}




//////////////////////////////////
// Helper Function for On_Frame //
//////////////////////////////////

void Model::compute_next(Character& character, double dt)
{
    //// Tentatively update the position to check if there's collision
    //// in the next frame, and change character velocities accordingly

    Character character_next = character.next(dt);

    character_next.move(-5,0);
    if (hit_left_map(character_next)) {
        character.update_horizontal_velocity(0);
    }

    character_next.move(10,0);
    if (hit_right_map(character_next)) {
        character.update_horizontal_velocity(0);
    }

    character_next.move(-5,0);
    if (hit_top_map(character_next)) {
        character.update_vertical_velocity(config_.anti_jump_velocity.height);
    }

    if (hit_bottom_map(character_next)) {
        character.update_in_air(false);
        if (character.velocity().height >= 0) {
            character.update_vertical_velocity(0);
        }
    }
    character_next.move(0,10);
    if (!hit_bottom_map(character_next)) {
        if (tick_ > 3) {
            if (!character.in_air()) {
                character.update_in_air(true);
            }
        }
    }
}



//////////////////////////
// Door, Pond, and Gems //
//////////////////////////

bool Model::at_door(Character character, ge211::Rect<int> door)
{
    if (character.position().x >= door.top_left().x) {
        if (character.position().x + config_.character_dims.width <= door
                .top_right().x) {
            if (character.position().y >= door.top_left().y) {
                if (character.position().y + config_.character_dims.height <=
                    door.bottom_left().y) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Model::touch_pond(Character character, ge211::Rect<int> pond)
{
    return touch(character, pond);
}

bool Model::touch_gem(Character character, ge211::Rect<int> gem)
{
    return touch(character, gem);
}



///////////////
// Trapdoors //
///////////////

void Model::activate_trap_door()
{
    bool activation = false;
    for (auto trap : trap_door_.get_traps()) {
        if (touch(fireboy_, trap)
            || touch(watergirl_, trap)) {
            activation = true;
        }
    }
    if (activation) {
        trap_door_.change_state(true);
    }
    else if (!activation) {
        trap_door_.change_state(false);
    }
}




// Helper for on_frame()
void Model::collect_gems()
{
    for (auto& red_gem : level_layout_.red_gems) {
        if (touch_gem(fireboy_, red_gem)) {
            red_gem = level_layout_.red_gems.back();
            level_layout_.red_gems.pop_back();
            score_++;
        }
    }
    for (auto& blue_gem : level_layout_.blue_gems) {
        if (touch_gem(watergirl_, blue_gem)) {
            blue_gem = level_layout_.blue_gems.back();
            level_layout_.blue_gems.pop_back();
            score_++;
        }
    }
}

// Helper function
bool Model::touch(Character character, ge211::Rect<int> whatever)
{
    float x_left, x_right, y_top, y_bottom;
    x_left = character.position().x;
    x_right = character.position().x + config_.character_dims.width;
    y_top = character.position().y;
    y_bottom = character.position().y + config_.character_dims.height;

    if ((x_right < whatever.top_left().x || x_left > whatever.top_right().x)
        || (y_bottom < whatever.top_left().y || y_top > whatever.bottom_left().y)) {
        return false;
    }
    return true;
}

