#include "PlayerBoard.hpp"
#include "TileFactory.hpp"
#include <algorithm>
#include <stdexcept>
#include <memory>

PlayerBoard::PlayerBoard(int owner) {
    initializeTilePiles(owner);
}

std::shared_ptr<Tile> PlayerBoard::peekTile(TileType type) const {
    auto it = tilePiles.find(type);
    if (it != tilePiles.end() && !it->second.empty()) {
        return it->second.back();
    }
    return nullptr;
}

std::shared_ptr<Tile> PlayerBoard::takeTile(TileType type) {
    auto it = tilePiles.find(type);
    if (it != tilePiles.end() && !it->second.empty()) {
        auto tile = it->second.back();
        it->second.pop_back();
        return tile;
    }
    return nullptr;
}

bool PlayerBoard::hasTiles(TileType type) const {
    auto it = tilePiles.find(type);
    return (it != tilePiles.end() && !it->second.empty());
}

void PlayerBoard::initializeTilePiles(int owner) {
    // Initialize Coal tiles
    tilePiles[TileType::Coal] = {
        std::make_shared<Tile>(TileFactory::createTile(TileType::Coal, 1, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Coal, 2, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Coal, 2, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Coal, 3, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Coal, 3, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Coal, 4, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Coal, 4, owner))
    };

    // Initialize Iron tiles
    tilePiles[TileType::Iron] = {
        std::make_shared<Tile>(TileFactory::createTile(TileType::Iron, 1, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Iron, 2, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Iron, 3, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Iron, 4, owner))
    };

    // Initialize Cotton tiles
    tilePiles[TileType::Cotton] = {
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 1, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 1, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 1, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 2, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 2, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 3, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 3, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 3, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 4, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 4, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 4, owner))
    };

    // Initialize Manufacturer tiles
    tilePiles[TileType::Manufacturer] = {
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 1, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 2, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 2, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 3, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 4, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 5, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 5, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 6, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 7, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 8, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 8, owner))
    };

    // Initialize Pottery tiles
    tilePiles[TileType::Pottery] = {
        std::make_shared<Tile>(TileFactory::createTile(TileType::Pottery, 1, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Pottery, 2, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Pottery, 3, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Pottery, 4, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Pottery, 5, owner))
    };

    // Initialize Brewery tiles
    tilePiles[TileType::Brewery] = {
        std::make_shared<Tile>(TileFactory::createTile(TileType::Brewery, 1, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Brewery, 1, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Brewery, 2, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Brewery, 2, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Brewery, 3, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Brewery, 3, owner)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Brewery, 4, owner))
    };

    // Sort each pile in descending order of level (so highest level is at the back)
    for (auto& pile : tilePiles) {
        std::sort(pile.second.begin(), pile.second.end(),
                  [](const std::shared_ptr<Tile>& a, const std::shared_ptr<Tile>& b) {
                      return a->level > b->level;
                  });
    }
}