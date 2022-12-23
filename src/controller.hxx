#pragma once

#include "model.hxx"
#include "view.hxx"


class Controller : public ge211::Abstract_game
{
public:
    Controller(const std::string,
               const std::string,
               Game_Config config,
               Level_Layout level_layout);
    void on_key_down(ge211::Key key) override;
    void on_key_up(ge211::Key key) override;
    void on_frame(double dt) override;
    ge211::Dims<int> initial_window_dimensions() const override;
    std::string initial_window_title() const override;

protected:
    void draw(ge211::Sprite_set& set) override;

private:
    Model model_;
    View view_;
};
