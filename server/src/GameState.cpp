#include "GameState.hpp"
#include "Tile.hpp"
#include "TileFactory.hpp"
#include "GameBoard.hpp"
#include <algorithm>
#include <random>
#include <stdexcept>

GameState::GameState() : m_next_id(1) {
    m_board.initializeBrassBirminghamMap();
}

std::shared_ptr<Player> GameState::addPlayer() {
    auto new_player = std::make_shared<Player>(m_next_id++);
    m_players[new_player->id] = new_player;
    return new_player;
}

void GameState::removePlayer(int id) {
    m_players.erase(id);
}

bool GameState::handleAction(int playerId, const GameAction& action) {
    switch (action.type) {
        case GameAction::Type::PlaceTile: {
            auto playerIt = m_players.find(playerId);
            if (playerIt == m_players.end()) {
                return false;  // Player not found
            }
            auto& player = playerIt->second;

            Tile newTile = TileFactory::createTile(action.tileType, 1, player);
            return handleTilePlacement(playerId, action.cityName, action.slotIndex, newTile);
        }
        // Add other action types here as needed
        default:
            return false;
    }
}

bool GameState::handleTilePlacement(int playerId, const std::string& cityName, int slotIndex, const Tile& tile) {
    auto playerIt = m_players.find(playerId);
    if (playerIt == m_players.end()) return false;
    auto& player = playerIt->second;

    // Check if player has enough money
    if (player->money < tile.cost_money) return false;

    // Check if the tile can be placed on the board
    if (!m_board.canPlaceTile(cityName, slotIndex, tile)) return false;

    // If all checks pass, place the tile and update player state
    if (m_board.placeTile(cityName, slotIndex, tile)) {
        player->money -= tile.cost_money;
        player->score += tile.victory_points;
        // Handle any other game state changes
        return true;
    }

    return false;
}

nlohmann::json GameState::getState() const {
    nlohmann::json state;
    state["players"] = nlohmann::json::array();
    for (const auto& pair : m_players) {
        const auto& player = pair.second;
        state["players"].push_back({
            {"id", player->id},
            {"score", player->score},
            {"money", player->money}
        });
    }

    state["board"] = nlohmann::json::object();
    for (const auto& pair : m_board.getCities()) {
        const auto& cityPtr = pair.second;
        nlohmann::json cityJson = {
            {"slots", nlohmann::json::array()}
        };
        for (const auto& slot : cityPtr->slots) {
            cityJson["slots"].push_back({
                {"allowedTileTypes", slot.allowedTileTypes},
                {"placedTile", slot.placedTile ? slot.placedTile->type : TileType::NullTile},
                {"owner", slot.placedTile && slot.placedTile->owner ? slot.placedTile->owner->id : -1},
                {"level", slot.placedTile ? slot.placedTile->level : 0},
                {"flipped", slot.placedTile ? slot.placedTile->flipped : false},
                {"resource_coal", slot.placedTile ? slot.placedTile->resource_coal : 0},
                {"resource_iron", slot.placedTile ? slot.placedTile->resource_iron : 0},
            });
        }
        state["board"]["cities"][cityPtr->name] = cityJson;
    }

    state["connections"] = nlohmann::json::array();
    for (const auto& connection : m_board.getPlacedConnections()) {
        state["connections"].push_back({
            {"city1", connection.city1},
            {"city2", connection.city2},
            {"owner", connection.linkOwner ? connection.linkOwner->id : -1}
        });
    }

    return state;
}