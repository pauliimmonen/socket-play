#include "TileFactory.hpp"
#include <stdexcept>

Tile TileFactory::createTile(TileType type, int level, std::shared_ptr<Player> owner) {
    Tile::Builder builder = Tile::create(type);
    builder.owner(owner).level(level);

    switch (type) {
        case TileType::Coal:
            builder.income(4)
                   .victoryPoints(1)
                   .linkPoints(2)
                   .costMoney(5)
                   .costCoal(0)
                   .costIron(0)
                   .initialResourceAmount(2);
            break;
        case TileType::Iron:
            builder.income(4)
                   .victoryPoints(1)
                   .linkPoints(2)
                   .costMoney(5)
                   .costCoal(0)
                   .costIron(0)
                   .initialResourceAmount(2);
            break;
        case TileType::Cotton:
            builder.income(5)
                   .victoryPoints(2)
                   .linkPoints(0)
                   .costMoney(10)
                   .costCoal(1)
                   .costIron(0);
            break;
        case TileType::Manufacturer:
            builder.income(5)
                   .victoryPoints(2)
                   .linkPoints(0)
                   .costMoney(10)
                   .costCoal(1)
                   .costIron(1);
            break;
        case TileType::Pottery:
            builder.income(3)
                   .victoryPoints(2)
                   .linkPoints(1)
                   .costMoney(10)
                   .costCoal(2)
                   .costIron(0);
            break;
        case TileType::Brewery:
            builder.income(5)
                   .victoryPoints(2)
                   .linkPoints(0)
                   .costMoney(15)
                   .costCoal(1)
                   .costIron(1);
            break;
        default:
            throw std::invalid_argument("Invalid tile type");
    }

    return builder.build();
}
