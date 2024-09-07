#ifndef PLAYERBOARD_HPP
#define PLAYERBOARD_HPP

#include "Tile.hpp"
#include <map>
#include <vector>
#include <memory>

class PlayerBoard {
public:
    PlayerBoard();
    
    // Get the top tile of a specific type without removing it
    std::shared_ptr<Tile> peekTile(TileType type) const;
    
    // Remove and return the top tile of a specific type
    std::shared_ptr<Tile> takeTile(TileType type);
    
    // Check if there are any tiles left of a specific type
    bool hasTiles(TileType type) const;

private:
    std::map<TileType, std::vector<std::shared_ptr<Tile>>> tilePiles;
    
    void initializeTilePiles();
};

#endif // PLAYERBOARD_HPP