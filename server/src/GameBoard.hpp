#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <memory>
#include "Tile.hpp"
#include "Player.hpp"

struct Slot {
    std::vector<TileType> allowedTileTypes;
    std::shared_ptr<Tile> placedTile;
};

struct City {
    std::string name;
    int x, y;
    std::vector<Slot> slots;
};

struct Connection {
    std::string city1;
    std::string city2;
    std::shared_ptr<Player> linkOwner;

    Connection(const std::string& c1, const std::string& c2) 
        : city1(std::min(c1, c2)), city2(std::max(c1, c2)), linkOwner(nullptr) {}

    bool operator<(const Connection& other) const {
        return std::tie(city1, city2) < std::tie(other.city1, other.city2);
    }
};

class GameBoard {
private:
    std::unordered_map<std::string, City> cities;
    std::set<Connection> connections;

    void calculateLinkPoints(const std::string& cityName);

public:
    void addCity(const std::string& name, int x, int y);
    void addConnection(const std::string& city1, const std::string& city2);
    void addSlot(const std::string& cityName, const Slot& slot);
    std::unordered_map<std::string, City>& getCities() { return cities; }
    const std::unordered_map<std::string, City>& getCities() const { return cities; }
    std::vector<std::string> getConnections(const std::string& cityName) const;
    void initializeBrassBirminghamMap();
    bool placeLink(const std::string& city1, const std::string& city2, std::shared_ptr<Player> player);
    std::vector<Connection> getPlacedConnections() const;
    std::vector<std::string> getConnectedCities(const std::string& startCity) const;
    int getTotalResourceCoal(const std::string& startCity) const;
    bool placeTile(std::shared_ptr<Player> player, const std::string& cityName, int slotIndex, const Tile& tile);
};

#endif // GAMEBOARD_HPP
