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
    int m_next_id = 1;
    Market coal_market{7,2};
    Market iron_market{5,2};
    ERA era = ERA::Canal;


public:
    GameBoard m_board;
    GameState();
    std::shared_ptr<Player> addPlayer();
    void removePlayer(int id);
    bool handleAction(int playerId, const GameAction& action);
    bool handleTilePlacement(Player& player, const GameAction& action, const Tile tile);

    nlohmann::json getState() const;

    void setupBoardForTesting(const GameBoard& board);

private:
    std::vector<ResourceOption> findAvailableResources(const std::string& startCity, TileType resourceType, Player &player, int amountNeeded) const;
    int chooseAndConsumeResources(Player &player, const std::string& cityName, TileType resourceType, int amountNeeded);
    int getTilePrice(const std::string& cityName, const Tile tile);
    void handleFlippedTile(Tile &tile);
    bool handleDevelop(Player& player, const GameAction& action);
    bool handleSell(Player& player, const GameAction& action);
    int consumeResources(Tile& tile, int amount);
    bool handleLinkPlacement(Player& player, const GameAction& action);
    // Add any private helper methods here if needed
};

#endif // GAMESTATE_HPP