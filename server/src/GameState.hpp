#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <vector>
#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>
#include "GameBoard.hpp"
#include "Player.hpp"
#include "GameAction.hpp"

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
    nlohmann::json getState() const;

private:
};

#endif // GAMESTATE_HPP
