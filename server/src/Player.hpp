#pragma once
#include "PlayerBoard.hpp"

struct Player {
    int id;
    int score = 0;
    int money = 30;
    int income_level = 10;
    PlayerBoard player_board;

    Player(int _id) : id(_id), score(0), money(30), income_level(10), player_board(PlayerBoard(_id)){}
};