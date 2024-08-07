#include "TileFactory.hpp"
#include <stdexcept>

Tile TileFactory::createTile(const std::string& type, int level, std::shared_ptr<Player> owner) {
    if (type == "Coal" && level == 1) {
        return Tile::create(type)
            .owner(owner)
            .level(level)
            .income(4)
            .victoryPoints(1)
            .linkPoints(2)
            .costMoney(5)
            .initialResourceAmount(2)
            .build();
    } else if (type == "Iron" && level == 1) {
        return Tile::create(type)
            .owner(owner)
            .level(level)
            .income(4)
            .victoryPoints(1)
            .linkPoints(2)
            .costMoney(5)
            .initialResourceAmount(2)
            .build();
    } else if (type == "Cotton" && level == 1) {
        return Tile::create(type)
            .owner(owner)
            .level(level)
            .income(5)
            .victoryPoints(2)
            .costMoney(10)
            .costCoal(1)
            .build();
    } else if (type == "Manufacturer" && level == 1) {
        return Tile::create(type)
            .owner(owner)
            .level(level)
            .income(5)
            .victoryPoints(2)
            .costMoney(10)
            .costCoal(1)
            .costIron(1)
            .build();
    }
    // Add more tile types and levels as needed
    throw std::runtime_error("Invalid tile type or level");
}