#pragma once

struct Player {
    int id;
    int score;
    int money;

    Player(int _id) : id(_id), score(0), money(30) {}
};