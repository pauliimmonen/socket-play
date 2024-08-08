#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "Tile.hpp"

struct Slot {
    std::vector<TileType> allowedTileTypes;
    std::shared_ptr<Tile> placedTile;
};

struct City {
    std::string name;
    std::vector<std::string> connections;
    int x, y;
    std::vector<Slot> slots;
};

class GameBoard {
private:
    std::unordered_map<std::string, City> cities;

public:
    void addCity(const std::string& name, int x, int y);
    void addConnection(const std::string& city1, const std::string& city2);
    void addSlot(const std::string& cityName, const Slot& slot);
    const std::unordered_map<std::string, City>& getCities() const;
    void initializeBrassBirminghamMap();
};

#endif // GAMEBOARD_HPP
