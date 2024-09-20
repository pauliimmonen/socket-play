#include "TileFactory.hpp"
#include <stdexcept>

Tile TileFactory::createTile(TileType type, int level, int owner) {
    Tile::Builder builder = Tile::create(type);
    builder.owner(owner).level(level);

    switch (type) {
        case TileType::Coal:
            switch (level) {
                case 1:
                    builder.income(4)
                           .victoryPoints(1)
                           .costMoney(5)
                           .linkPoints(2)
                           .resourceAmount(2);
                    break;
                case 2:
                    builder.income(7)
                           .victoryPoints(2)
                           .costMoney(7)
                           .linkPoints(1)
                           .resourceAmount(3);
                    break;
                case 3:
                    builder.income(6)
                           .victoryPoints(3)
                           .costMoney(8)
                           .costIron(1)
                           .linkPoints(1)
                           .resourceAmount(4);
                    break;
                case 4:
                    builder.income(5)
                           .victoryPoints(4)
                           .costMoney(10)
                           .costIron(1)
                           .linkPoints(1)
                           .resourceAmount(4);
                    break;
                default:
                    throw std::invalid_argument("Invalid Coal tile level");
            }
            break;
        case TileType::Iron:
            switch (level) {
                case 1:
                    builder.income(3)
                           .victoryPoints(3)
                           .costMoney(5)
                           .costCoal(1)
                           .linkPoints(1)
                           .resourceAmount(4);
                    break;
                case 2:
                    builder.income(3)
                           .victoryPoints(5)
                           .costMoney(7)
                           .costCoal(1)
                           .linkPoints(1)
                           .resourceAmount(4);
                    break;
                case 3:
                    builder.income(2)
                           .victoryPoints(7)
                           .costMoney(9)
                           .costCoal(1)
                           .linkPoints(1)
                           .resourceAmount(5);
                    break;
                case 4:
                    builder.income(1)
                           .victoryPoints(9)
                           .costMoney(12)
                           .costCoal(1)
                           .linkPoints(1)
                           .resourceAmount(6);
                    break;
                default:
                    throw std::invalid_argument("Invalid Iron tile level");
            }
            break;
        case TileType::Cotton:
            builder.linkPoints(1).beerDemand(1);
            switch (level) {
                case 1:
                    builder.income(5)
                           .victoryPoints(5)
                           .costMoney(12);
                    break;
                case 2:
                    builder.income(4)
                           .victoryPoints(5)
                           .costMoney(14)
                           .costCoal(1)
                           .linkPoints(2);
                    break;
                case 3:
                    builder.income(3)
                           .victoryPoints(9)
                           .costMoney(16)
                           .costCoal(1)
                           .costIron(1);
                    break;
                case 4:
                    builder.income(2)
                           .victoryPoints(12)
                           .costMoney(18)
                           .costCoal(1)
                           .costIron(1);
                    break;
                default:
                    throw std::invalid_argument("Invalid Cotton tile level");
            }
            break;
        case TileType::Manufacturer:
            builder.linkPoints(1).beerDemand(1);
            switch (level) {
                case 1:
                    builder.income(5)
                           .victoryPoints(3)
                           .costMoney(8)
                           .costCoal(1)
                           .linkPoints(2);
                    break;
                case 2:
                    builder.income(1)
                           .victoryPoints(5)
                           .costMoney(10)
                           .costIron(1);
                    break;
                case 3:
                    builder.income(4)
                           .victoryPoints(4)
                           .costMoney(12)
                           .costCoal(2)
                           .linkPoints(0)
                           .beerDemand(0);
                    break;
                case 4:
                    builder.income(8)
                           .victoryPoints(3)
                           .costMoney(8)
                           .costIron(1);
                    break;
                case 5:
                    builder.income(2)
                           .victoryPoints(8)
                           .costMoney(16)
                           .costCoal(1)
                           .linkPoints(2);
                    break;
                case 6:
                    builder.income(6)
                           .victoryPoints(7)
                           .costMoney(20);
                    break;
                case 7:
                    builder.income(4)
                           .victoryPoints(9)
                           .costMoney(16)
                           .costCoal(1)
                           .costIron(1)
                           .linkPoints(0)
                           .beerDemand(0);
                    break;
                case 8:
                    builder.income(1)
                           .victoryPoints(11)
                           .costMoney(20)
                           .costIron(2)
                           .linkPoints(1);
                    break;
                default:
                    throw std::invalid_argument("Invalid Manufactor tile level");
            }
            break;
        case TileType::Pottery:
            builder.linkPoints(1);
            switch (level) {
                case 1:
                    builder.income(5)
                           .victoryPoints(10)
                           .costMoney(17)
                           .costIron(1)
                           .beerDemand(1);
                    break;
                case 2:
                    builder.income(1)
                           .victoryPoints(1)
                           .costMoney(0)
                           .costIron(1)
                           .beerDemand(1);
                    break;
                case 3:
                    builder.income(5)
                           .victoryPoints(11)
                           .costMoney(22)
                           .costCoal(2)
                           .beerDemand(2);
                    break;
                case 4:
                    builder.income(1)
                           .victoryPoints(1)
                           .costMoney(0)
                           .costCoal(1)
                           .beerDemand(1);
                    break;
                case 5:
                    builder.income(5)
                           .victoryPoints(22)
                           .costMoney(24)
                           .costCoal(2)
                           .beerDemand(2);
                    break;
                default:
                    throw std::invalid_argument("Invalid Pottery tile level");
            }
            break;
        case TileType::Brewery:
            builder.linkPoints(2).costIron(1).resourceAmount(1); //canal default resource amount
            switch (level) {
                case 1:
                    builder.income(4)
                           .victoryPoints(4)
                           .costMoney(5);
                    break;
                case 2:
                    builder.income(5)
                           .victoryPoints(5)
                           .costMoney(7);
                    break;
                case 3:
                    builder.income(5)
                           .victoryPoints(7)
                           .costMoney(9);
                    break;
                case 4:
                    builder.income(5)
                           .victoryPoints(9)
                           .costMoney(9);
                    break;
                default:
                    throw std::invalid_argument("Invalid Brewery tile level");
            }
            break;
        default:
            throw std::invalid_argument("Invalid tile type");
    }

    return builder.build();
}
