#include "Tile.hpp"
#include <stdexcept>

Tile::Tile(TileType t, std::shared_ptr<Player> o, int l, bool f, int i, int vp, int lp, 
           int cm, int cc, int ci, int rc, int ri, int ira, int sbc)
    : type(t), owner(o), level(l), flipped(f), income(i), victory_points(vp), link_points(lp),
      cost_money(cm), cost_coal(cc), cost_iron(ci), resource_coal(rc), resource_iron(ri),
      initial_resource_amount(ira), sell_beer_cost(sbc) {}

Tile::Builder Tile::create(TileType type) {
    return Builder(type);
}

Tile::Builder::Builder(TileType type) {
    tile.type = type;
    tile.owner = nullptr;
    tile.level = 1;
    tile.flipped = false;
    tile.income = 0;
    tile.victory_points = 0;
    tile.link_points = 0;
    tile.cost_money = 0;
    tile.cost_coal = 0;
    tile.cost_iron = 0;
    tile.resource_coal = 0;
    tile.resource_iron = 0;
    tile.initial_resource_amount = 0;
    tile.sell_beer_cost = 0;
}

Tile::Builder& Tile::Builder::owner(std::shared_ptr<Player> o) { tile.owner = o; return *this; }
Tile::Builder& Tile::Builder::level(int l) { tile.level = l; return *this; }
Tile::Builder& Tile::Builder::flipped(bool f) { tile.flipped = f; return *this; }
Tile::Builder& Tile::Builder::income(int i) { tile.income = i; return *this; }
Tile::Builder& Tile::Builder::victoryPoints(int vp) { tile.victory_points = vp; return *this; }
Tile::Builder& Tile::Builder::linkPoints(int lp) { tile.link_points = lp; return *this; }
Tile::Builder& Tile::Builder::costMoney(int cm) { tile.cost_money = cm; return *this; }
Tile::Builder& Tile::Builder::costCoal(int cc) { tile.cost_coal = cc; return *this; }
Tile::Builder& Tile::Builder::costIron(int ci) { tile.cost_iron = ci; return *this; }
Tile::Builder& Tile::Builder::resourceCoal(int rc) { tile.resource_coal = rc; return *this; }
Tile::Builder& Tile::Builder::resourceIron(int ri) { tile.resource_iron = ri; return *this; }
Tile::Builder& Tile::Builder::initialResourceAmount(int ira) { tile.initial_resource_amount = ira; return *this; }
Tile::Builder& Tile::Builder::sellBeerCost(int sbc) { tile.sell_beer_cost = sbc; return *this; }

Tile Tile::Builder::build() { return tile; }

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
