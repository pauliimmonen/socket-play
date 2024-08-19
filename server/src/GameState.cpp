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

            try {
                Tile newTile = TileFactory::createTile(action.tileType, 1, player);
                return placeTile(playerId, action.cityName, action.slotIndex, newTile);
            } catch (const std::invalid_argument& e) {
                return false;  // Invalid tile type
            }
        }
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
            {"score", player->score},
            {"money", player->money}
        });
    }

    state["board"] = nlohmann::json::object();
    for (const auto& pair : m_board.getCities()) {
        const auto& city = pair.second;
        nlohmann::json cityJson = {
            {"x", city.x},
            {"y", city.y},
            {"slots", nlohmann::json::array()}
        };
        for (const auto& slot : city.slots) {
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
        state["board"]["cities"][city.name] = cityJson;
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

bool GameState::placeTile(int playerId, const std::string& cityName, int slotIndex, const Tile& tile) {
    auto playerIt = m_players.find(playerId);
    if (playerIt == m_players.end()) {
        return false;  // Player not found
    }
    auto& player = playerIt->second;

    const auto& cities = m_board.getCities();
    auto cityIt = cities.find(cityName);
    if (cityIt == cities.end()) {
        return false;  // City not found
    }
    const auto& city = cityIt->second;

    if (slotIndex < 0 || slotIndex >= static_cast<int>(city.slots.size())) {
        return false;  // Invalid slot index
    }
    auto& slot = const_cast<Slot&>(city.slots[slotIndex]);

    if (slot.placedTile) {
        return false;  // Slot already occupied
    }

    if (std::find(slot.allowedTileTypes.begin(), slot.allowedTileTypes.end(), tile.type) == slot.allowedTileTypes.end()) {
        return false;  // Tile type not allowed in this slot
    }

    if (player->money < tile.cost_money) {
        return false;  // Not enough money
    }

    slot.placedTile = std::make_shared<Tile>(tile);
    player->money -= tile.cost_money;
    player->score += tile.victory_points;

    calculateLinkPoints(cityName);

    return true;
}

void GameState::calculateLinkPoints(const std::string& cityName) {
    // TODO: Implement link points calculation logic
    // This function should update the link_points for tiles in the given city
    // and potentially in connected cities based on the game rules
    (void)cityName; // Silence unused parameter warning
}
