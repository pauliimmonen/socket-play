#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include "Player.hpp"
#include "GameAction.hpp"
#include "Tile.hpp"

// Define a macro for unused parameters that works across different compilers
#if defined(__GNUC__) || defined(__clang__)
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

struct CitySlot {
    std::vector<std::string> allowedTileTypes;
    std::shared_ptr<Tile> placedTile;
};

struct City {
    std::string name;
    int x;
    int y;
    std::vector<std::string> connections;
    std::vector<CitySlot> slots;
};

struct GameBoard {
    std::map<std::string, City> cities;
};

class GameState {
public:
    GameState();
    std::shared_ptr<Player> addPlayer();
    void removePlayer(int id);
    bool handleAction(int playerId, const GameAction& action);
    nlohmann::json getState() const;

private:
    void initializeBoard();
    bool placeTile(int playerId, UNUSED const std::string& cityName, UNUSED int slotIndex, UNUSED const std::string& tileType);
    void generateAvailableTiles();
    void calculateLinkPoints(UNUSED const std::string& cityName);

    int m_next_id;
    std::map<int, std::shared_ptr<Player>> m_players;
    GameBoard m_board;
    std::vector<Tile> m_availableTiles;
};

#endif // GAMESTATE_HPP