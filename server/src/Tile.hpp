#ifndef TILE_HPP
#define TILE_HPP

#include <string>
#include <memory>

enum class TileType {
    Coal,
    Iron,
    Cotton,
    Manufacturer,
    Pottery,
    Brewery,
    Merchant,
    NullTile
    // Add other tile types as needed
};

enum class MerchantType {
    Pottery,
    Manufacturer,
    Cotton,
    Any,
    Empty
};

/// Represents a tile in the game
class Tile {
public:
    class Builder;

    TileType type = TileType::NullTile;
    int owner = -1;
    int level = 0;
    bool flipped = false;

    // Points and income
    int income = 0;
    int victory_points = 0;
    int link_points = 0;

    // Costs
    int cost_money = 0;
    int cost_coal = 0;
    int cost_iron = 0;

    // Resources
    int resource_amount = 0;
    int beer_demand = 0;

    Tile(TileType t, int owner, int l, bool f, int i, int vp, int lp, 
         int cm, int cc, int ci, int ra, int bd);

    virtual ~Tile() = default;
    virtual std::shared_ptr<Tile> clone() const {
        return std::make_shared<Tile>(*this);
    }
    int consumeResources(int amount);

    static TileType stringToTileType(const std::string& typeStr);

protected:
    Tile() = default;

public:
    static Builder create(TileType type);
};

class MerchantTile : public Tile {
public:
    MerchantType merchantType;

    MerchantTile(MerchantType mt);

    static MerchantTile create(MerchantType mt);

    std::shared_ptr<Tile> clone() const override {
        return std::make_shared<MerchantTile>(*this);
    }
};

class Tile::Builder {
private:
    Tile tile;

public:
    Builder(TileType type);
    Builder& owner(int o);
    Builder& level(int l);
    Builder& flipped(bool f);
    Builder& income(int i);
    Builder& victoryPoints(int vp);
    Builder& linkPoints(int lp);
    Builder& costMoney(int cm);
    Builder& costCoal(int cc);
    Builder& costIron(int ci);
    Builder& resourceAmount(int ira);
    Builder& beerDemand(int bd);

    Tile build();
};

#endif // TILE_HPP
