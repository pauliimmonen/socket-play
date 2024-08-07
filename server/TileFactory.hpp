#ifndef TILEFACTORY_HPP
#define TILEFACTORY_HPP

#include "Tile.hpp"
#include <memory>

class TileFactory {
public:
    static Tile createTile(TileType type, int level, std::shared_ptr<Player> owner);
};

#endif // TILEFACTORY_HPP
