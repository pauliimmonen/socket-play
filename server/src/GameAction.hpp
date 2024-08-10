#pragma once
#include <string>
#include "Tile.hpp"

struct GameAction {
    enum class Type {
        PlaceTile,
        Unknown
    };

    Type type;
    std::string cityName;
    TileType tileType;
    int slotIndex;

    GameAction() : type(Type::Unknown), slotIndex(-1) {}
};
