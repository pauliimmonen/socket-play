#include "Tile.hpp"
#include <stdexcept>
#include <iostream>

Tile::Tile(TileType t, int o, int l, bool f, int i, int vp, int lp, 
           int cm, int cc, int ci, int ra, int bd)
    : type(t), owner(o), level(l), flipped(f), income(i), victory_points(vp), link_points(lp),
      cost_money(cm), cost_coal(cc), cost_iron(ci), resource_amount(ra),beer_demand(bd) {}
    
int Tile::consumeResources(int amount){
    if (amount>=resource_amount){
        amount -= resource_amount;
        resource_amount = 0;
        flipped = true;
        return amount;
    } else{
        resource_amount -= amount;
    }
    return 0;
}

Tile::Builder Tile::create(TileType type) {
    return Builder(type);
}

Tile::Builder::Builder(TileType type) {
    tile.type = type;
    tile.owner = -1;
    tile.level = 1;
    tile.flipped = false;
    tile.income = 0;
    tile.victory_points = 0;
    tile.link_points = 0;
    tile.cost_money = 0;
    tile.cost_coal = 0;
    tile.cost_iron = 0;
    tile.resource_amount = 0;
    tile.beer_demand = 0;
}

Tile::Builder& Tile::Builder::owner(int o) { tile.owner = o; return *this; }
Tile::Builder& Tile::Builder::level(int l) { tile.level = l; return *this; }
Tile::Builder& Tile::Builder::flipped(bool f) { tile.flipped = f; return *this; }
Tile::Builder& Tile::Builder::income(int i) { tile.income = i; return *this; }
Tile::Builder& Tile::Builder::victoryPoints(int vp) { tile.victory_points = vp; return *this; }
Tile::Builder& Tile::Builder::linkPoints(int lp) { tile.link_points = lp; return *this; }
Tile::Builder& Tile::Builder::costMoney(int cm) { tile.cost_money = cm; return *this; }
Tile::Builder& Tile::Builder::costCoal(int cc) { tile.cost_coal = cc; return *this; }
Tile::Builder& Tile::Builder::costIron(int ci) { tile.cost_iron = ci; return *this; }
Tile::Builder& Tile::Builder::resourceAmount(int ra) { tile.resource_amount = ra; return *this; }
Tile::Builder& Tile::Builder::beerDemand(int bd) { tile.beer_demand = bd; return *this; }

Tile Tile::Builder::build() { return tile; }

// MerchantTile implementation
MerchantTile::MerchantTile(MerchantType mt) : Tile(), merchantType(mt) {
    type = TileType::Merchant;
}

TileType Tile::stringToTileType(const std::string& typeStr) {
    if (typeStr == "Coal") return TileType::Coal;
    if (typeStr == "Iron") return TileType::Iron;
    if (typeStr == "Cotton") return TileType::Cotton;
    if (typeStr == "Manufacturer") return TileType::Manufacturer;
    if (typeStr == "Pottery") return TileType::Pottery;
    if (typeStr == "Brewery") return TileType::Brewery;
    if (typeStr == "Null") return TileType::NullTile;
    throw std::invalid_argument("Invalid tile type string");
}
