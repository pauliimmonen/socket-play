#pragma once

#include "Player.hpp"
#include "GameAction.hpp"
#include <nlohmann/json.hpp>
#include <map>
#include <vector>
#include <string>
#include <memory>

struct CitySlot {
    std::vector<std::string> allowedTileTypes;
    std::string placedTile;
};

struct City {
    std::string name;
    int x;
    int y;
    std::vector<std::string> connections;
    std::vector<CitySlot> slots;
};

struct Board {
    std::map<std::string, City> cities;
};

struct Tile {
    std::string type;
    int playerId;
};

class GameState {
public:
    GameState();
    std::shared_ptr<Player> addPlayer();
    void removePlayer(int id);
    bool handleAction(int playerId, const GameAction& action);
    nlohmann::json getState() const;
    void initializeBoard();

private:
    std::map<int, std::shared_ptr<Player>> m_players;
    int m_next_id;
    Board m_board;
    std::vector<Tile> m_availableTiles;

    bool placeTile(int playerId, const std::string& cityName, int slotIndex, const std::string& tileType);
    void generateAvailableTiles();
};