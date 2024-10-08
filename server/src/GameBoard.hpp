#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <memory>
#include "Tile.hpp"
#include "Player.hpp"

enum class MerchantBonus
{
    Income2,
    Develop,
    Money5,
    Points4,
    Points3,
};

struct Slot
{
    std::vector<TileType> allowedTileTypes;
    std::shared_ptr<Tile> placedTile;
};

struct City
{
    std::string name;
    std::vector<Slot> slots;

    City(const std::string &cityName) : name(cityName) {}
    virtual ~City() = default;
};

struct MerchantCity : public City
{
    MerchantBonus merchant_bonus;

    MerchantCity(const std::string &cityName, MerchantBonus mb)
        : City(cityName), merchant_bonus(mb) {}

    ~MerchantCity() override = default;
};

struct Connection
{
    std::string city1;
    std::string city2;
    std::shared_ptr<Player> linkOwner;

    Connection(const std::string &c1, const std::string &c2)
        : city1(std::min(c1, c2)), city2(std::max(c1, c2)), linkOwner(nullptr) {}

    bool operator<(const Connection &other) const
    {
        return std::tie(city1, city2) < std::tie(other.city1, other.city2);
    }
};

class GameBoard
{
private:
    std::unordered_map<std::string, std::unique_ptr<City>> cities;
    std::set<Connection> connections;
    std::vector<std::string> getPlayerPlacedTiles(const Player &player) const;
    std::vector<Connection> getPlayerPlacedLinks(const Player &player) const;
    int getAvailableBeerFromMerchantSlots(const std::string &cityName, const MerchantType &allowedType) const;
    int getAvailableBeerFromBreweries(const std::string &cityName, const Player &player) const;
    bool hasEnoughBeer(const std::string &cityName, const Player &player, int beerDemand, const MerchantType &allowedType) const;

public:
    // Initialization
    void initializeBrassBirminghamMap();

    // Create the board
    City *addCity(const std::string &name);
    void addSlot(const std::string &cityName, const Slot &slot);
    MerchantCity *addMerchantCity(const std::string &name, MerchantBonus mb);
    Connection &addConnection(const std::string &city1, const std::string &city2);

    // City and Connection Queries
    const City *getCity(const std::string &cityName) const;
    const std::unordered_map<std::string, std::unique_ptr<City>> &getCities() const { return cities; }
    std::vector<std::string> getConnections(const std::string &cityName) const;
    std::vector<std::string> getConnectedCities(const std::string &startCity) const;
    bool isConnectedToMerchantCity(const std::string &cityName) const;
    std::vector<const MerchantCity *> getConnectedMerchantCities(const std::string &cityName) const;
    std::set<MerchantType> getConnectedMerchantTypes(const std::string &cityName) const;

    // Link Management
    bool placeLink(const std::string &city1, const std::string &city2, std::shared_ptr<Player> player);
    std::vector<Connection> getPlacedLinks() const;

    // Tile Management
    bool canPlaceTile(const std::string &cityName, int slotIndex, const Tile &tile) const;
    bool placeTile(const std::string &cityName, int slotIndex, const Tile &tile);
    std::vector<std::pair<std::string, int>> findSellableTiles(const Player &player) const;

    // Resource Queries
    int getTotalResourceCoal(const std::string &startCity) const;
    int getTotalResourceIron() const;

    // Player Network
    bool isCityInPlayerNetwork(const Player &player, const std::string &cityName) const;

    // Merchant City
    const MerchantCity *getMerchantCity(const std::string &cityName) const;
};

#endif // GAMEBOARD_HPP