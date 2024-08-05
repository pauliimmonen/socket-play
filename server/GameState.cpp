#include "GameState.hpp"
#include <algorithm>
#include <random>

GameState::GameState() : m_next_id(1) {
    initializeBoard();
}

std::shared_ptr<Player> GameState::addPlayer() {
    auto new_player = std::make_shared<Player>(m_next_id++);
    m_players[new_player->id] = new_player;
    generateAvailableTiles();
    return new_player;
}

void GameState::removePlayer(int id) {
    m_players.erase(id);
    generateAvailableTiles();
}

bool GameState::handleAction(int playerId, const GameAction& action) {
    switch (action.type) {
        case GameAction::Type::PlaceTile:
            return placeTile(playerId, action.cityName, action.slotIndex, action.tileType);
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
    
    state["cities"] = nlohmann::json::object();
    for (const auto& pair : m_cities) {
        const auto& city = pair.second;
        nlohmann::json cityJson = {
            {"x", city.x},
            {"y", city.y},
            {"connections", city.connections},
            {"slots", nlohmann::json::array()}
        };
        for (const auto& slot : city.slots) {
            cityJson["slots"].push_back({
                {"allowedTileTypes", slot.allowedTileTypes},
                {"placedTile", slot.placedTile}
            });
        }
        state["cities"][city.name] = cityJson;
    }
    
    state["availableTiles"] = nlohmann::json::array();
    for (const auto& tile : m_availableTiles) {
        state["availableTiles"].push_back({
            {"type", tile.type},
            {"playerId", tile.playerId}
        });
    }
    
    return state;
}

void GameState::initializeBoard() {
    // Create a simple board with a few cities and connections
    m_cities["Birmingham"] = {
        "Birmingham", 0, 0, 
        {"Coventry", "Worcester"},
        {
            {{"Coal", "Iron"}, ""},
            {{"Cotton", "Manufacturer"}, ""},
            {{"Coal", "Iron", "Cotton", "Manufacturer"}, ""},
            {{"Coal", "Iron", "Cotton", "Manufacturer"}, ""}
        }
    };
    m_cities["Coventry"] = {
        "Coventry", 1, 0, 
        {"Birmingham", "Oxford"},
        {
            {{"Coal", "Iron"}, ""},
            {{"Cotton", "Manufacturer"}, ""},
            {{"Coal", "Iron", "Cotton", "Manufacturer"}, ""}
        }
    };
    m_cities["Worcester"] = {
        "Worcester", 0, 1, 
        {"Birmingham", "Oxford"},
        {
            {{"Coal", "Cotton"}, ""},
            {{"Iron", "Manufacturer"}, ""},
            {{"Coal", "Iron", "Cotton", "Manufacturer"}, ""}
        }
    };
    m_cities["Oxford"] = {
        "Oxford", 1, 1, 
        {"Coventry", "Worcester"},
        {
            {{"Coal", "Cotton"}, ""},
            {{"Iron", "Manufacturer"}, ""},
            {{"Coal", "Iron", "Cotton", "Manufacturer"}, ""},
            {{"Coal", "Iron", "Cotton", "Manufacturer"}, ""}
        }
    };
}

bool GameState::placeTile(int playerId, const std::string& cityName, int slotIndex, const std::string& tileType) {
    auto cityIt = m_cities.find(cityName);
    if (cityIt == m_cities.end() || slotIndex < 0 || static_cast<std::vector<CitySlot>::size_type>(slotIndex) >= cityIt->second.slots.size()) {
        return false;
    }
    
    auto& slot = cityIt->second.slots[slotIndex];
    if (!slot.placedTile.empty() || std::find(slot.allowedTileTypes.begin(), slot.allowedTileTypes.end(), tileType) == slot.allowedTileTypes.end()) {
        return false;
    }
    
    auto tileIt = std::find_if(m_availableTiles.begin(), m_availableTiles.end(),
                               [&](const Tile& tile) { return tile.type == tileType && tile.playerId == playerId; });
    if (tileIt == m_availableTiles.end()) {
        return false;
    }
    
    slot.placedTile = tileType;
    m_availableTiles.erase(tileIt);
    
    // Update player score and money (simplified)
    auto playerIt = m_players.find(playerId);
    if (playerIt != m_players.end()) {
        playerIt->second->score += 1;
        playerIt->second->money -= 5; // Assume each tile costs 5 money to place
    }
    
    return true;
}

void GameState::generateAvailableTiles() {
    m_availableTiles.clear();
    std::vector<std::string> tileTypes = {"Coal", "Iron", "Cotton", "Manufacturer"};
    for (const auto& player : m_players) {
        for (const auto& tileType : tileTypes) {
            m_availableTiles.push_back({tileType, player.first});
        }
    }
    
    // Shuffle the tiles
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_availableTiles.begin(), m_availableTiles.end(), g);
}