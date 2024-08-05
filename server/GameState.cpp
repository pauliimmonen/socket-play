#include "GameState.hpp"

GameState::GameState() : m_next_id(1) {}

std::shared_ptr<Player> GameState::addPlayer() {
    auto new_player = std::make_shared<Player>(Player{m_next_id++, 0, 0, 0});
    m_players[new_player->id] = new_player;
    return new_player;
}

void GameState::removePlayer(int id) {
    m_players.erase(id);
}

bool GameState::handleAction(int playerId, const GameAction& action) {
    switch (action.type) {
        case GameAction::Type::Move:
            return movePlayer(playerId, action.dx, action.dy);
        default:
            return false;
    }
}

nlohmann::json GameState::getState() const {
    nlohmann::json state;
    state["players"] = nlohmann::json::array();
    for (const auto& pair : m_players) {
        const auto& player = pair.second;
        state["players"].push_back({
            {"id", player->id},
            {"x", player->x},
            {"y", player->y},
            {"score", player->score}
        });
    }
    return state;
}

bool GameState::movePlayer(int id, int dx, int dy) {
    auto it = m_players.find(id);
    if (it != m_players.end()) {
        it->second->x += dx;
        it->second->y += dy;
        return true;
    }
    return false;
}