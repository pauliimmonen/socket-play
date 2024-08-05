#pragma once
#include <string>

struct GameAction {
    enum class Type {
        PlaceTile,
        Unknown
    };

    Type type;
    std::string cityName;
    std::string tileType;
    int slotIndex;

    GameAction() : type(Type::Unknown), slotIndex(-1) {}
};