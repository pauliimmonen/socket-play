#ifndef TILE_HPP
#define TILE_HPP

#include <string>
#include <memory>
#include "Player.hpp"

enum class TileType {
    Coal,
    Iron,
    Cotton,
    Manufacturer,
    Pottery,
    Brewery,
    NullTile
    // Add other tile types as needed
};

class Tile {
public:
    class Builder;

    TileType type;
    std::shared_ptr<Player> owner;
    int level;
    bool flipped;
    int income;
    int victory_points;
    int link_points;
    int cost_money;
    int cost_coal;
    int cost_iron;
    int resource_coal;
    int resource_iron;
    int initial_resource_amount;
    int beer_demand;

    Tile(TileType t, std::shared_ptr<Player> o, int l, bool f, int i, int vp, int lp, 
         int cm, int cc, int ci, int rc, int ri, int ira, int bd);

    static TileType stringToTileType(const std::string& typeStr);

private:
    Tile() = default;

public:
    static Builder create(TileType type);
};

class Tile::Builder {
private:
    Tile tile;

public:
    Builder(TileType type);
    Builder& owner(std::shared_ptr<Player> o);
    Builder& level(int l);
    Builder& flipped(bool f);
    Builder& income(int i);
    Builder& victoryPoints(int vp);
    Builder& linkPoints(int lp);
    Builder& costMoney(int cm);
    Builder& costCoal(int cc);
    Builder& costIron(int ci);
    Builder& resourceCoal(int rc);
    Builder& resourceIron(int ri);
    Builder& initialResourceAmount(int ira);
    Builder& beerDemand(int bd);

    Tile build();
};

#endif // TILE_HPP
