#pragma once

#include "Player.hpp"
#include "GameAction.hpp"
#include <nlohmann/json.hpp>
#include <map>
#include <memory>

class GameState {
public:
    GameState();
    std::shared_ptr<Player> addPlayer();
    void removePlayer(int id);
    bool handleAction(int playerId, const GameAction& action);
    nlohmann::json getState() const;

private:
    std::map<int, std::shared_ptr<Player>> m_players;
    int m_next_id;

    bool movePlayer(int id, int dx, int dy);
};