#pragma once
#include <string>
#include "Tile.hpp"

struct GameAction {
    enum class Type {
        PlaceTile,
        PlaceLink,
        TakeLoan,
        Develop,
        Scout,
        Sell,
        Unknown
    };

    Type type;
    std::string cityName;
    std::string cityName2;
    TileType tileType;
    TileType tileType2;
    int slotIndex;
    

    GameAction() : type(Type::Unknown), slotIndex(-1) {}
};
