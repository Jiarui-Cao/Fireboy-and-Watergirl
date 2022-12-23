#include "model.hxx"
#include <catch.hxx>
#include <iostream>

// using namespace ge211;

struct Test_Access
{
    Model& model;

    explicit Test_Access(Model&);
    Character& fireboy();
    Character& watergirl();
    Game_Config& game_config();
};


TEST_CASE("example test")
{
    CHECK(1 + 1 == 2);
}

TEST_CASE("Check Initial State")
{
    Model model("Fireboy", "Watergirl", Game_Config(), Level_Layout());
    // Test_Access access(model);

    ge211::Posn<int> fireboy_door = model.get_level_layout().fireboy_door
            .top_left();
    ge211::Posn<int> watergirl_door = model.get_level_layout().watergirl_door
                                         .top_left();

    CHECK(fireboy_door.x == 767);
    CHECK(fireboy_door.y == 95);
    CHECK(watergirl_door.x == 855);
    CHECK(watergirl_door.y == 95);

    // Check that players are alive and the game hasn't been won
    CHECK(model.character(Player::fireboy).alive());
    CHECK(model.character(Player::watergirl).alive());
    CHECK(model.get_score() == 0);
    CHECK(model.get_game_state() == 0);
}



TEST_CASE("Physics Engine (Grav Acceleration and Jump)")
{
    Model model("Fireboy", "Watergirl", Game_Config(), Level_Layout());
    Test_Access access(model);

    // Update the model pass "flying frame" (see Model::on_frame(float dt)
    // definition for more details
    model.on_frame(1);
    CHECK(access.fireboy().velocity().height == 0);
    CHECK(access.watergirl().velocity().height == 0);

    model.on_frame(1);
    CHECK(access.fireboy().velocity().height == 0);
    CHECK(access.watergirl().velocity().height == 0);

    model.on_frame(1);
    CHECK(access.fireboy().velocity().height == 0);
    CHECK(access.watergirl().velocity().height == 0);
    CHECK(!access.fireboy().in_air());
    CHECK(!access.watergirl().in_air());

    // Now that "flying frame" is over, both characters free-fall
    model.on_frame(0.05);
    CHECK(access.fireboy().velocity().height == 50);
    CHECK(access.watergirl().velocity().height == 50);
    CHECK(access.fireboy().in_air());
    CHECK(access.watergirl().in_air());
    model.on_frame(0.05);
    CHECK(access.fireboy().velocity().height == 100);
    CHECK(access.watergirl().velocity().height == 100);
    model.on_frame(0.05);
    CHECK(access.fireboy().velocity().height == 150);
    CHECK(access.watergirl().velocity().height == 150);
    model.on_frame(0.05);
    model.on_frame(0.05);
    model.on_frame(0.05);
    model.on_frame(0.05);

    // By this time both characters should have hit the ground, so their
    // velocities would be set to 0 and in_air_ = false
    CHECK(access.fireboy().velocity().height == 0);
    CHECK(access.watergirl().velocity().height == 0);
    CHECK(!access.fireboy().in_air());
    CHECK(!access.watergirl().in_air());

    // Move fireboy to the right a bit on some empty space and have him jump
    access.fireboy().move(300,0);
    access.fireboy().update_in_air(false);
    access.fireboy().update_vertical_velocity(0);
    CHECK(access.fireboy().velocity().height == 0);
    CHECK(access.fireboy().velocity().width == 0);
    access.fireboy().jump();
    CHECK(access.fireboy().velocity().height == model.get_game_config()
    .jump_velocity.height);

    // Now gravitational acceleration kicks in
    float vertical_v = -400;
    for (int i=0; i<=14; i++) {
        model.on_frame(0.05);
        CHECK(access.fireboy().in_air());
        vertical_v += model.get_game_config().grav_acceleration.height * 0.05;
        CHECK(access.fireboy().velocity().height == vertical_v);
        // check that horizontal velocity doesn't change
        CHECK(access.fireboy().velocity().width == 0);
    }
    // By this time fireboy should have hit the ground
    model.on_frame(0.05);
    CHECK(!access.fireboy().in_air());
    CHECK(access.fireboy().velocity().height == 0);


    // Let's try parabolic jump
    access.game_config().horizontal_velocity = {10,0};
    model.set_key("FH", 'r');
    access.fireboy().jump();
    CHECK(access.fireboy().velocity().width == 0);
    CHECK(access.fireboy().velocity().height == model.get_game_config()
                                                     .jump_velocity.height);
    vertical_v = -400;
    for (int i=0; i<=14; i++) {
        model.on_frame(0.05);
        CHECK(access.fireboy().in_air());
        vertical_v += model.get_game_config().grav_acceleration.height * 0.05;
        CHECK(access.fireboy().velocity().height == vertical_v);
        // check that horizontal velocity doesn't change
        CHECK(access.fireboy().velocity().width == 10);
    }
    // By this time fireboy should have hit the ground
    model.on_frame(0.05);
    CHECK(!access.fireboy().in_air());
    CHECK(access.fireboy().velocity().height == 0);
}


TEST_CASE("Physics Engine (Horizontal & Vertical Collisions)")
{
    Model model("Fireboy", "Watergirl", Game_Config(), Level_Layout());
    Test_Access access(model);

    // Same setup as the previous test
    model.on_frame(1);
    model.on_frame(1);
    model.on_frame(1);
    CHECK(access.fireboy().velocity().height == 0);
    CHECK(access.watergirl().velocity().height == 0);
    CHECK(!access.fireboy().in_air());
    CHECK(!access.watergirl().in_air());
    model.on_frame(0.05);
    CHECK(access.fireboy().velocity().height == 50);
    CHECK(access.watergirl().velocity().height == 50);
    CHECK(access.fireboy().in_air());
    CHECK(access.watergirl().in_air());
    model.on_frame(0.05);
    CHECK(access.fireboy().velocity().height == 100);
    CHECK(access.watergirl().velocity().height == 100);
    model.on_frame(0.05);
    CHECK(access.fireboy().velocity().height == 150);
    CHECK(access.watergirl().velocity().height == 150);
    model.on_frame(0.05);
    model.on_frame(0.05);
    model.on_frame(0.05);
    model.on_frame(0.05);
    CHECK(access.watergirl().velocity().height == 0);

    // Move fireboy to the left until it hits the left wall
    CHECK(model.get_key("FH") == 'n');
    model.set_key("FH", 'l');
    CHECK(model.get_key("FH") == 'l');
    CHECK(access.fireboy().velocity().width == 0);
    CHECK(!model.hit_left_map(access.fireboy().next(0.05)));
    CHECK(!model.hit_right_map(access.fireboy().next(0.05)));
    CHECK(!model.hit_top_map(access.fireboy().next(0.05)));

    for (int i=0; i<=6; i++) {
        model.on_frame(0.01);
        CHECK(access.fireboy().velocity().width == -275);
    }

    // Now fireboy hits left wall and stops
    CHECK(model.hit_left_map(access.fireboy().next(0.05)));
    model.on_frame(0.01);
    CHECK(access.fireboy().velocity().width == 0);
    // Let fireboy run towards right and check if it worked properly
    model.set_key("FH", 'r');
    for (int i=0; i<=20; i++) {
        model.on_frame(0.05);
        CHECK(access.fireboy().velocity().width == 275);
    }

    // We will now check whether watergirl jumping and hitting the ceiling
    // would work properly
    CHECK(!access.watergirl().in_air());
    model.set_key("WV", 'u');
    float vertical_v = -400;
    for (int i=0; i<=3; i++) {
        model.on_frame(0.01);
        CHECK(access.watergirl().in_air());
        vertical_v += model.get_game_config().grav_acceleration.height * 0.01;
        CHECK(access.watergirl().velocity().height == vertical_v);
    }
    // Will hit ceiling in the next frame, and upon hitting the ceiling
    // vertical velocity immediately becomes 10
    CHECK(model.hit_top_map(access.watergirl().next(0.01)));
    model.on_frame(0.01);
    CHECK(access.watergirl().velocity().height == model.get_game_config()
    .anti_jump_velocity.height + model.get_game_config().grav_acceleration
    .height * 0.01);
    vertical_v = 20;
    // Free falls
    for (int i=0; i<=15; i++) {
        model.on_frame(0.01);
        CHECK(access.watergirl().in_air());
        vertical_v += model.get_game_config().grav_acceleration.height * 0.01;
        CHECK(access.watergirl().velocity().height == vertical_v);
    }
    // Will hit the ground in the next frame
    CHECK(model.hit_bottom_map(access.watergirl().next(0.01)));
    model.on_frame(0.01);
    CHECK(access.watergirl().velocity().height == 0);
}


TEST_CASE("Win the Game")
{
    Model model("Fireboy", "Watergirl", Game_Config(), Level_Layout());
    Test_Access access(model);

    // Check that the game doesn't win
    model.on_frame(1);
    model.on_frame(1);
    model.on_frame(1);
    model.on_frame(0.01);
    CHECK(model.get_game_state() == 0);
    CHECK(!model.at_door(access.fireboy(),model.get_level_layout()
                                              .fireboy_door));
    CHECK(!model.at_door(access.watergirl(),model.get_level_layout()
                                               .watergirl_door));

    // Move fireboy to its door and the game shouldn't end yet
    access.fireboy().move_to(770, 100);
    CHECK(model.at_door(access.fireboy(),model.get_level_layout()
                                              .fireboy_door));
    CHECK(!model.at_door(access.watergirl(),model.get_level_layout()
                                               .watergirl_door));
    CHECK(model.get_game_state() == 0);

    // Move watergirl to its door, and the game should be won now
    access.watergirl().move_to(860, 100);
    CHECK(model.at_door(access.fireboy(),model.get_level_layout()
                                              .fireboy_door));
    CHECK(model.at_door(access.watergirl(),model.get_level_layout()
                                               .watergirl_door));
    model.on_frame(0.01);
    CHECK(model.get_game_state() == 1);
}


TEST_CASE("Lose the Game")
{
    Model model("Fireboy", "Watergirl", Game_Config(), Level_Layout());
    Test_Access access(model);

    // Initialize the game pass "flying frame"
    model.on_frame(1);
    model.on_frame(1);
    model.on_frame(1);
    CHECK(model.get_game_state() == 0);

    // Throw fireboy on top of the red pond, nothing should happen
    access.fireboy().move_to(550, 650);
    for (int i=0; i<=10; i++) {
        model.on_frame(0.05);
    }
    CHECK(model.touch_pond(access.fireboy(), model.get_level_layout()
                                                    .red_pond));
    CHECK(!model.touch_pond(access.watergirl(), model.get_level_layout()
                                                    .blue_pond));
    CHECK(model.get_game_state() == 0);

    // Throw watergirl on top of the blue pond, nothing should happen
    access.watergirl().move_to(700, 650);
    for (int i=0; i<=10; i++) {
        model.on_frame(0.05);
    }
    CHECK(model.touch_pond(access.fireboy(), model.get_level_layout()
                                                  .red_pond));
    CHECK(model.touch_pond(access.watergirl(), model.get_level_layout()
                                                  .blue_pond));
    CHECK(model.get_game_state() == 0);

    // Throw fireboy on top of the blue pond, game should end
    access.fireboy().move_to(700, 650);
    for (int i=0; i<=10; i++) {
        model.on_frame(0.05);
    }
    CHECK(!model.touch_pond(access.fireboy(), model.get_level_layout()
                                                  .red_pond));
    CHECK(model.touch_pond(access.fireboy(), model.get_level_layout()
                                                   .blue_pond));
    CHECK(model.touch_pond(access.watergirl(), model.get_level_layout()
                                                    .blue_pond));
    CHECK(model.get_game_state() == -1);



    Model model_2("Fireboy", "Watergirl", Game_Config(), Level_Layout());
    Test_Access access_2(model_2);

    // Initialize the game pass "flying frame"
    model_2.on_frame(1);
    model_2.on_frame(1);
    model_2.on_frame(1);
    CHECK(model_2.get_game_state() == 0);
    CHECK(!model_2.touch_pond(access_2.fireboy(), model_2.get_level_layout()
                                                  .green_pond));
    CHECK(!model_2.touch_pond(access_2.watergirl(), model_2.get_level_layout()
                                                  .green_pond));

    // Throw fireboy on top of the green pond, game should end
    access_2.fireboy().move_to(700, 500);
    for (int i=0; i<=10; i++) {
        model_2.on_frame(0.05);
    }
    CHECK(model_2.touch_pond(access_2.fireboy(), model_2.get_level_layout()
                                                         .green_pond));
    CHECK(!model_2.touch_pond(access_2.watergirl(), model_2.get_level_layout()
                                                           .green_pond));
    CHECK(model_2.get_game_state() == -1);
}


TEST_CASE("Collect Gems")
{
    Model model("Fireboy", "Watergirl", Game_Config(), Level_Layout());
    Test_Access access(model);
    model.on_frame(1);
    model.on_frame(1);
    model.on_frame(1);
    CHECK(model.get_score() == 0);
    CHECK(model.get_level_layout().red_gems.size() == 4);
    CHECK(model.get_level_layout().blue_gems.size() == 4);

    // Fireboy collects the first red gem
    access.fireboy().move_to(545, 675);
    model.on_frame(0.01);
    CHECK(model.get_score() == 1);
    CHECK(model.get_level_layout().red_gems.size() == 3);

    // Watergirl collects the first blue gem
    access.watergirl().move_to(745, 675);
    model.on_frame(0.01);
    CHECK(model.get_score() == 2);
    CHECK(model.get_level_layout().blue_gems.size() == 3);

    // Fireboy doesn't collect blue gem and watergirl doesn't collect red gem
    access.fireboy().move_to(550, 375);
    model.on_frame(0.01);
    CHECK(model.get_score() == 2);
    CHECK(model.get_level_layout().blue_gems.size() == 3);
    access.watergirl().move_to(550, 375);
    model.on_frame(0.01);
    CHECK(model.get_score() == 3);
    CHECK(model.get_level_layout().blue_gems.size() == 2);

    access.watergirl().move_to(150, 350);
    model.on_frame(0.01);
    CHECK(model.get_score() == 3);
    CHECK(model.get_level_layout().red_gems.size() == 3);
    access.fireboy().move_to(150, 350);
    model.on_frame(0.01);
    CHECK(model.get_score() == 4);
    CHECK(model.get_level_layout().red_gems.size() == 2);
}




///
/// Member function definitions for Test_access
///

Test_Access::Test_Access(Model& model)
        : model(model)
{}

Character&
Test_Access::fireboy()
{
    return model.fireboy_;
}

Character&
Test_Access::watergirl()
{
    return model.watergirl_;
}

Game_Config&
Test_Access::game_config()
{
    return model.config_;
}