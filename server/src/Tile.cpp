#include "Tile.hpp"
#include <stdexcept>

Tile::Tile(TileType t, std::shared_ptr<Player> o, int l, bool f, int i, int vp, int lp, 
           int cm, int cc, int ci, int rc, int ri, int ira, int bd, MarketType mt)
    : type(t), owner(o), level(l), flipped(f), income(i), victory_points(vp), link_points(lp),
      cost_money(cm), cost_coal(cc), cost_iron(ci), resource_coal(rc), resource_iron(ri),
      initial_resource_amount(ira), beer_demand(bd), marketType(mt) {}

Tile::Tile(MarketType mt) : type(TileType::Market), marketType(mt) {
    // Initialize other properties with default values
    owner = nullptr;
    level = 1;
    flipped = false;
    income = 0;
    victory_points = 0;
    link_points = 0;
    cost_money = 0;
    cost_coal = 0;
    cost_iron = 0;
    resource_coal = 0;
    resource_iron = 0;
    initial_resource_amount = 0;
    beer_demand = 0;
}

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
    tile.resource_beer = 0;
    tile.initial_resource_amount = 0;
    tile.beer_demand = 0;
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
Tile::Builder& Tile::Builder::beerDemand(int bd) { tile.beer_demand = bd; return *this; }

Tile Tile::Builder::build() { return tile; }

Tile::Builder Tile::Builder::createMarket(MarketType mt) {
    Builder builder(TileType::Market);
    builder.tile.marketType = mt;
    if (mt != MarketType::Empty){
        builder.tile.resource_beer=1;
    }
    return builder;
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
