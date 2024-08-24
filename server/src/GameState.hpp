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

class GameState {
private:
    std::unordered_map<int, std::shared_ptr<Player>> m_players;
    GameBoard m_board;
    int m_next_id;

public:
    GameState();
    std::shared_ptr<Player> addPlayer();
    void removePlayer(int id);
    bool handleAction(int playerId, const GameAction& action);
    bool handleTilePlacement(int playerId, const std::string& cityName, int slotIndex, const Tile& tile);
    nlohmann::json getState() const;

private:
    // Add any private helper methods here if needed
};

#endif // GAMESTATE_HPP