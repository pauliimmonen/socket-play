#pragma once

struct Player {
    int id;
    int score = 0;
    int money = 30;
    int income_level = 10;

    Player(int _id) : id(_id), score(0), money(30), income_level(10) {}
};