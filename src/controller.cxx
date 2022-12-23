#include "controller.hxx"
#include "game_config.hxx"

Controller::Controller(const std::string filename_1,
                       const std::string filename_2,
                       Game_Config config,
                       Level_Layout level_layout)
        : model_(filename_1, filename_2, config, level_layout),
          view_(model_)
{ }


void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set);
}


ge211::Dims<int>
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}


std::string
Controller::initial_window_title() const
{
    return "Fireboy and Watergirl";
}


void
Controller::on_key_down(ge211::Key key)
{
    if (key == ge211::Key::code('a')) {
        model_.set_key("WH", 'l');
    }
    if (key == ge211::Key::code('d')) {
        model_.set_key("WH", 'r');
    }
    if (key == ge211::Key::code('w')) {
        model_.set_key("WV", 'u');
    }

    if (key == ge211::Key::up()) {
        model_.set_key("FV", 'u');
    }
    if (key == ge211::Key::left()) {
        model_.set_key("FH", 'l');
    }
    if (key == ge211::Key::right()) {
        model_.set_key("FH", 'r');
    }

    if (key == ge211::Key::command()) {
        if (key == ge211::Key::code('q')) {
            quit();
        }
    }
}


void
Controller::on_key_up(ge211::Key key)
{
    if (key == ge211::Key::code('a')) {
        model_.set_key("WH", 'n');
    }
    if (key == ge211::Key::code('d')) {
        model_.set_key("WH", 'n');
    }
    if (key == ge211::Key::code('w')) {
        model_.set_key("WV", 'n');
    }

    if (key == ge211::Key::up()) {
        model_.set_key("FV", 'n');
    }
    if (key == ge211::Key::left()) {
        model_.set_key("FH", 'n');
    }
    if (key == ge211::Key::right()) {
        model_.set_key("FH", 'n');
    }
}


void
Controller::on_frame(double dt)
{
    model_.on_frame(dt);
}