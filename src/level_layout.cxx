//
// Created by Jerry C on 12/1/22.
//

#include "level_layout.hxx"

Level_Layout::Level_Layout():
    bricks(),
    half_bricks(),
    fireboy_door(767,95,75,80),
    watergirl_door(855,95,75,80),
    green_pond(650,575,125,20),
    red_pond(500,725,125,20),
    blue_pond(700,725,125,20),
    red_gems(),
    blue_gems(),
    traps_1(),
    doors_1()
{
    // Vertical boundaries
    for (int i=1; i<=29; i++) {
        bricks.push_back({0,i});
        bricks.push_back({38, i});
    }
    // Horizontal boundaries
    for (int i=1; i<=39; i++) {
        bricks.push_back({i,1});
    }
    for (int i=1; i<=19; i++) {
        bricks.push_back({i,29});
    }
    for (int i=25; i<=27; i++) {
        bricks.push_back({i,29});
    }
    for (int i=33; i<=39; i++) {
        bricks.push_back({i,29});
    }
    // The rest
    for (int i=10; i<=13; i++) {
        bricks.push_back({i,5});
    }
    for (int i=12; i<=15; i++) {
        bricks.push_back({i,6});
    }
    for (int i=12; i<=37; i++) {
        bricks.push_back({i,7});
    }
    for (int i=1; i<=5; i++) {
        bricks.push_back({i,8});
    }
    for (int i=12; i<=16; i++) {
        bricks.push_back({i,8});
    }
    for (int i=1; i<=5; i++) {
        bricks.push_back({i,9});
    }
    // for (int i=12; i<=16; i++) {
    //     bricks.push_back({i,9});
    // }
    for (int i=1; i<=7; i++) {
        bricks.push_back({i,10});
    }
    // for (int i=20; i<=25; i++) {
    //     bricks.push_back({i,10});
    // }
    for (int i=1; i<=5; i++) {
        bricks.push_back({i,11});
    }
    for (int i=20; i<=26; i++) {
        bricks.push_back({i,11});
    }
    for (int i=1; i<=33; i++) {
        bricks.push_back({i,12});
    }
    for (int i=26; i<=33; i++) {
        bricks.push_back({i,13});
    }
    for (int i=37; i<=37; i++) {
        bricks.push_back({i,14});
    }
    for (int i=36; i<=37; i++) {
        bricks.push_back({i,15});
    }
    for (int i=6; i<=19; i++) {
        bricks.push_back({i,16});
    }
    for (int i=36; i<=37; i++) {
        bricks.push_back({i,16});
    }
    for (int i=18; i<=37; i++) {
        bricks.push_back({i,17});
    }
    for (int i=33; i<=37; i++) {
        bricks.push_back({i,18});
    }
    for (int i=35; i<=37; i++) {
        bricks.push_back({i,19});
    }
    for (int i=36; i<=37; i++) {
        bricks.push_back({i,20});
    }
    for (int i=1; i<=17; i++) {
        bricks.push_back({i,21});
    }
    for (int i=17; i<=18; i++) {
        bricks.push_back({i,22});
    }
    for (int i=18; i<=25; i++) {
        bricks.push_back({i,23});
    }
    for (int i=31; i<=33; i++) {
        bricks.push_back({i,23});
    }
    for (int i=1; i<=12; i++) {
        bricks.push_back({i,25});
    }
    for (int i=36; i<=37; i++) {
        bricks.push_back({i,26});
    }
    for (int i=35; i<=37; i++) {
        bricks.push_back({i,27});
    }
    for (int i=35; i<=37; i++) {
        bricks.push_back({i,28});
    }


    /////////////////
    // Half-Bricks //
    /////////////////
    for (int i=20; i<=24; i++) {
        half_bricks.push_back({i,29});
    }
    for (int i=28; i<=32; i++) {
        half_bricks.push_back({i,29});
    }
    for (int i=26; i<=30; i++) {
        half_bricks.push_back({i,23});
    }


    //////////
    // Gems //
    //////////
    red_gems.push_back(Block(545,675,35,30));
    red_gems.push_back(Block(150,350,35,30));
    red_gems.push_back(Block(275,75,35,30));
    red_gems.push_back(Block(475,125,35,30));

    blue_gems.push_back(Block(745,675,35,30));
    blue_gems.push_back(Block(550,375,35,30));
    blue_gems.push_back(Block(50,125,35,30));
    blue_gems.push_back(Block(600,125,35,30));


    ///////////////
    // Trapdoors //
    ///////////////
    traps_1.push_back(Block(250,375,50,25));
    traps_1.push_back(Block(200,500,50,25));
    traps_1.push_back(Block(725,275,50,25));

    doors_1.push_back({Block(25,450,25,25), false});
    doors_1.push_back({Block(50,450,25,25), false});
    doors_1.push_back({Block(925,300,25,25), true});
    doors_1.push_back({Block(900,300,25,25), true});
    doors_1.push_back({Block(875,300,25,25), true});
    doors_1.push_back({Block(850,300,25,25), true});
}