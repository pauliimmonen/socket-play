#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <vector>
#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>
#include "GameBoard.hpp"
#include "Player.hpp"
#include "GameAction.hpp"
#include "Tile.hpp"
#include "Market.hpp"

enum class ERA{
    Canal,
    RailRoad
};

struct ResourceOption {
    std::string cityName;
    int slotIndex;
    int amount;
};

class GameState {
private:
    std::map<int, std::shared_ptr<Player>> m_players;
    GameBoard m_board;
    int m_next_id = 1;
    Market coal_market{7,2};
    Market iron_market{5,2};
    ERA era = ERA::Canal;


public:
    GameState();
    std::shared_ptr<Player> addPlayer();
    void removePlayer(int id);
    bool handleAction(int playerId, const GameAction& action);
    bool handleTilePlacement(int playerId, const std::string& cityName, int slotIndex, const Tile tile);
    nlohmann::json getState() const;

    void setupBoardForTesting(const GameBoard& board);

private:
    std::vector<ResourceOption> findAvailableResources(const std::string& startCity, TileType resourceType, int amountNeeded);
    int chooseAndConsumeResources(const std::string& cityName, TileType resourceType, int amountNeeded);
    int getTilePrice(const std::string& cityName, const Tile tile);
    void handleFlippedTile(Tile &tile);
    bool handleDevelop(TileType a, TileType b, Player& player);
    int consumeResources(Tile& tile, int amount);
    // Add any private helper methods here if needed
};

#endif // GAMESTATE_HPP