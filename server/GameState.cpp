#include "GameState.hpp"
#include "Tile.hpp"
#include "TileFactory.hpp"
#include <algorithm>
#include <random>
#include <stdexcept>

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

    state["board"] = nlohmann::json::object();
    for (const auto& pair : m_board.cities) {
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
                {"placedTile", slot.placedTile ? slot.placedTile->type : ""},
                {"owner", slot.placedTile && slot.placedTile->owner ? slot.placedTile->owner->id : -1},
                {"level", slot.placedTile ? slot.placedTile->level : 0},
                {"flipped", slot.placedTile ? slot.placedTile->flipped : false},
                {"income", slot.placedTile ? slot.placedTile->income : 0},
                {"victory_points", slot.placedTile ? slot.placedTile->victory_points : 0},
                {"link_points", slot.placedTile ? slot.placedTile->link_points : 0},
                {"cost_money", slot.placedTile ? slot.placedTile->cost_money : 0},
                {"cost_coal", slot.placedTile ? slot.placedTile->cost_coal : 0},
                {"cost_iron", slot.placedTile ? slot.placedTile->cost_iron : 0},
                {"resource_coal", slot.placedTile ? slot.placedTile->resource_coal : 0},
                {"resource_iron", slot.placedTile ? slot.placedTile->resource_iron : 0},
                {"initial_resource_amount", slot.placedTile ? slot.placedTile->initial_resource_amount : 0}
            });
        }
        state["board"]["cities"][city.name] = cityJson;
    }

    state["availableTiles"] = nlohmann::json::array();
    for (const auto& tile : m_availableTiles) {
        state["availableTiles"].push_back({
            {"type", tile.type},
            {"playerId", tile.owner ? tile.owner->id : -1},
            {"level", tile.level},
            {"flipped", tile.flipped},
            {"income", tile.income},
            {"victory_points", tile.victory_points},
            {"link_points", tile.link_points},
            {"cost_money", tile.cost_money},
            {"cost_coal", tile.cost_coal},
            {"cost_iron", tile.cost_iron},
            {"resource_coal", tile.resource_coal},
            {"resource_iron", tile.resource_iron},
            {"initial_resource_amount", tile.initial_resource_amount}
        });
    }

    return state;
    // ... (keep existing implementation)
}

void GameState::initializeBoard() {
    // Create a simple board with a few cities and connections
    m_board.cities["Birmingham"] = {
        "Birmingham", 0, 0,
        {"Coventry", "Worcester"},
        {
            {{"Coal", "Iron"}, nullptr},
            {{"Cotton", "Manufacturer"}, nullptr},
            {{"Coal", "Iron", "Cotton", "Manufacturer"}, nullptr},
            {{"Coal", "Iron", "Cotton", "Manufacturer"}, nullptr}
        }
    };
    m_board.cities["Coventry"] = {
        "Coventry", 1, 0,
        {"Birmingham", "Oxford"},
        {
            {{"Coal", "Iron"}, nullptr},
            {{"Cotton", "Manufacturer"}, nullptr},
            {{"Coal", "Iron", "Cotton", "Manufacturer"}, nullptr}
        }
    };
    m_board.cities["Worcester"] = {
        "Worcester", 0, 1,
        {"Birmingham", "Oxford"},
        {
            {{"Coal", "Cotton"}, nullptr},
            {{"Iron", "Manufacturer"}, nullptr},
            {{"Coal", "Iron", "Cotton", "Manufacturer"}, nullptr}
        }
    };
    m_board.cities["Oxford"] = {
        "Oxford", 1, 1,
        {"Coventry", "Worcester"},
        {
            {{"Coal", "Cotton"}, nullptr},
            {{"Iron", "Manufacturer"}, nullptr},
            {{"Coal", "Iron", "Cotton", "Manufacturer"}, nullptr},
            {{"Coal", "Iron", "Cotton", "Manufacturer"}, nullptr}
        }
    };
}


bool GameState::placeTile(int playerId, const std::string& cityName, int slotIndex, const std::string& tileType) {
    // Silence unused parameter warnings
    (void)cityName;
    (void)slotIndex;
    (void)tileType;

    auto playerIt = m_players.find(playerId);
    if (playerIt == m_players.end()) {
        return false;  // Player not found
    }

    // TODO: Implement actual tile placement logic using cityName, slotIndex, and tileType
    // For now, we'll just return true to indicate success
    return true;
}

void GameState::generateAvailableTiles() {
    m_availableTiles.clear();
    std::vector<std::string> tileTypes = {"Coal", "Iron", "Cotton", "Manufacturer"};
    for (const auto& player : m_players) {
        for (const auto& tileType : tileTypes) {
            m_availableTiles.push_back(TileFactory::createTile(tileType, 1, player.second));
        }
    }
    
    // Shuffle the tiles
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_availableTiles.begin(), m_availableTiles.end(), g);
}

void GameState::calculateLinkPoints(const std::string& cityName) {
    // TODO: Implement link points calculation logic
    // This function should update the link_points for tiles in the given city
    // and potentially in connected cities based on the game rules
    (void)cityName; // Silence unused parameter warning
}
