#include "PlayerBoard.hpp"
#include "TileFactory.hpp"
#include <algorithm>
#include <stdexcept>
#include <memory>

PlayerBoard::PlayerBoard() {
    initializeTilePiles();
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

void PlayerBoard::initializeTilePiles() {
    // Initialize Coal tiles
    tilePiles[TileType::Coal] = {
        std::make_shared<Tile>(TileFactory::createTile(TileType::Coal, 1, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Coal, 2, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Coal, 2, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Coal, 3, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Coal, 3, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Coal, 4, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Coal, 4, nullptr))
    };

    // Initialize Iron tiles
    tilePiles[TileType::Iron] = {
        std::make_shared<Tile>(TileFactory::createTile(TileType::Iron, 1, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Iron, 2, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Iron, 3, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Iron, 4, nullptr))
    };

    // Initialize Cotton tiles
    tilePiles[TileType::Cotton] = {
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 1, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 1, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 1, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 2, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 2, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 3, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 3, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 3, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 4, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 4, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Cotton, 4, nullptr))
    };

    // Initialize Manufacturer tiles
    tilePiles[TileType::Manufacturer] = {
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 1, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 2, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 2, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 3, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 4, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 5, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 5, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 6, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 7, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 8, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Manufacturer, 8, nullptr))
    };

    // Initialize Pottery tiles
    tilePiles[TileType::Pottery] = {
        std::make_shared<Tile>(TileFactory::createTile(TileType::Pottery, 1, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Pottery, 2, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Pottery, 3, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Pottery, 4, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Pottery, 5, nullptr))
    };

    // Initialize Brewery tiles
    tilePiles[TileType::Brewery] = {
        std::make_shared<Tile>(TileFactory::createTile(TileType::Brewery, 1, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Brewery, 1, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Brewery, 2, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Brewery, 2, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Brewery, 3, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Brewery, 3, nullptr)),
        std::make_shared<Tile>(TileFactory::createTile(TileType::Brewery, 4, nullptr))
    };

    // Sort each pile in descending order of level (so highest level is at the back)
    for (auto& pile : tilePiles) {
        std::sort(pile.second.begin(), pile.second.end(),
                  [](const std::shared_ptr<Tile>& a, const std::shared_ptr<Tile>& b) {
                      return a->level > b->level;
                  });
    }
}