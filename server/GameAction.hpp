#pragma once

struct GameAction {
    enum class Type {
        Move,
        Unknown
    };

    Type type;
    int dx = 0;
    int dy = 0;
};