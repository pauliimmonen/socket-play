#include "GameState.hpp"
#include "Tile.hpp"
#include "TileFactory.hpp"
#include "GameBoard.hpp"
#include "IncomeFunctions.hpp"
#include <nlohmann/json.hpp>
#include <algorithm>
#include <random>
#include <iostream>

GameState::GameState(){
    m_board.initializeBrassBirminghamMap();
    coal_market.buy(1);
    iron_market.buy(2);
}

std::shared_ptr<Player> GameState::addPlayer() {
    auto new_player = std::make_shared<Player>(m_next_id++);
    new_player->income_level=10;
    new_player->money=30;
    m_players[new_player->id] = new_player;
    return new_player;
}

void GameState::removePlayer(int id) {
    m_players.erase(id);
}

bool GameState::handleAction(int playerId, const GameAction& action) {

    auto playerIt = m_players.find(playerId);
    if (playerIt == m_players.end()) {
        return false;  // Player not found
    }
    auto& player = playerIt->second;
    switch (action.type) {
        case GameAction::Type::PlaceTile: {
            Tile *newTile = player->player_board.takeTile(action.tileType).get();
            if (newTile==nullptr) return false;
            return handleTilePlacement(playerId, action.cityName, action.slotIndex, *newTile);
            break;
        }
        case GameAction::Type::PlaceLink: {
            if (m_board.isCityInPlayerNetwork(*player.get(), action.cityName) || m_board.isCityInPlayerNetwork(*player.get(), action.cityName2)){
                return m_board.placeLink(action.cityName,action.cityName2, player);
            }else
                return false;
            break;
        }
        case GameAction::Type::Develop: {
            return handleDevelop(action.tileType,action.tileType2,*player.get());
            break;
        }
        case GameAction::Type::TakeLoan: {
            if (player->income_level > 2){
                player->income_level = takeLoan(player->income_level);
                return true;
            }else
                return false;
            break;
        }
 
        // Add other action types here as needed
        default:
            return false;
    }
    return false;
}

int GameState::getTilePrice(const std::string& cityName, const Tile tile){
    const int CANT_BUY=2147483647;//max int
    int total_cost = tile.cost_money;
    int cost_coal = tile.cost_coal;
    if (cost_coal> 0){
        cost_coal= cost_coal- m_board.getTotalResourceCoal(cityName);
    }
    if (cost_coal > 0 && m_board.isConnectedToMerchantCity(cityName)){
        total_cost += coal_market.getPrice(cost_coal);
    }
    if (cost_coal > 0 && not m_board.isConnectedToMerchantCity(cityName)){
        return CANT_BUY;
    }
    int cost_iron = tile.cost_iron;
    if (cost_iron> 0){
        cost_iron = cost_iron - m_board.getTotalResourceIron();
    }
    if (cost_iron> 0){
        total_cost += coal_market.getPrice(cost_coal);
    }
    return total_cost;
}

std::vector<ResourceOption> GameState::findAvailableResources(const std::string& startCity, TileType resourceType, int amountNeeded) {
    std::vector<ResourceOption> options;
    
    if (resourceType == TileType::Coal) {
        // For coal, only consider connected cities
        std::vector<std::string> connectedCities = m_board.getConnectedCities(startCity);
        
        for (const auto& cityName : connectedCities) {
            const auto* city = m_board.getCity(cityName);
            if (!city) continue;
            
            for (size_t i = 0; i < city->slots.size(); ++i) {
                const auto& slot = city->slots[i];
                if (slot.placedTile && slot.placedTile->type == TileType::Coal) {
                    int available = slot.placedTile->resource_amount;
                    if (available > 0) {
                        options.push_back({cityName, static_cast<int>(i), std::min(available, amountNeeded)});
                    }
                }
            }
        }
    } 
    else if (resourceType == TileType::Iron) {
        // For iron, consider all cities on the board
        const auto& allCities = m_board.getCities();
        
        for (const auto& [cityName, city] : allCities) {
            for (size_t i = 0; i < city->slots.size(); ++i) {
                const auto& slot = city->slots[i];
                if (slot.placedTile && slot.placedTile->type == TileType::Iron) {
                    int available = slot.placedTile->resource_amount;
                    if (available > 0) {
                        options.push_back({cityName, static_cast<int>(i), std::min(available, amountNeeded)});
                    }
                }
            }
        }
    }
    
    return options;
}
int GameState::consumeResources(Tile& tile, int amount){
    if (amount>=tile.resource_amount){
        amount -= tile.resource_amount;
        tile.resource_amount = 0;
        tile.flipped = true;
        handleFlippedTile(tile);
        return amount;
    }else{
        tile.resource_amount -= amount;
    }
    return 0;
}
void GameState::handleFlippedTile(Tile& tile){
    auto playerIt = m_players.find(tile.owner);
    playerIt->second->income_level+=tile.income;
}

int GameState::chooseAndConsumeResources(const std::string &cityName, TileType resourceType, int amountNeeded)
{
    if (amountNeeded==0) return 0;

    auto options = findAvailableResources(cityName, resourceType, amountNeeded);
    if (options.empty())
    {
        return amountNeeded; // Not resources available
    }

    // If only one option, use it automatically
    if (options.size() == 1)
    {
        auto city = m_board.getCity(options[0].cityName);
        auto tile = city->slots[options[0].slotIndex].placedTile;
        amountNeeded = consumeResources(*tile,amountNeeded);
    } else {
        auto city = m_board.getCity(options[0].cityName);
        auto tile = city->slots[options[0].slotIndex].placedTile;
        amountNeeded = consumeResources(*tile,amountNeeded);
        if (amountNeeded > 0){
            amountNeeded = chooseAndConsumeResources(cityName, resourceType, amountNeeded);
            
        }
    }

    return amountNeeded;
}

bool GameState::handleTilePlacement(int playerId, const std::string& cityName, int slotIndex, const Tile tile) {
    auto playerIt = m_players.find(playerId);
    if (playerIt == m_players.end()) return false;
    auto& player = playerIt->second;
    // Check if the tile can be placed on the board
    if (!m_board.canPlaceTile(cityName, slotIndex, tile)) return false;


    // Check if player has enough money
    if (player->money < getTilePrice(cityName, tile)) return false;

    // If all checks pass, place the tile and update player state
    if (m_board.placeTile(cityName, slotIndex, tile)) {
        player->money -= getTilePrice(cityName, tile);
        int coal_amount = chooseAndConsumeResources(cityName, TileType::Coal, tile.cost_coal);
        int iron_amount = chooseAndConsumeResources(cityName, TileType::Iron, tile.cost_iron);
        coal_market.buy(coal_amount);
        iron_market.buy(iron_amount);
        return true;
    }
    return false;
}

nlohmann::json GameState::getState() const {
    nlohmann::json state;
    state["players"] = nlohmann::json::array();
    for (const auto& pair : m_players) {
        const auto& player = pair.second;
        nlohmann::json playerJson = {
            {"id", player->id},
            {"score", player->score},
            {"money", player->money},
            {"income_level", player->income_level},
            {"player_board", nlohmann::json::object()}
        };

        // Add PlayerBoard status
        nlohmann::json& boardJson = playerJson["player_board"];
        boardJson["coal"] = player->player_board.getRemainingTileAmount(TileType::Coal);
        boardJson["iron"] = player->player_board.getRemainingTileAmount(TileType::Iron);
        boardJson["cotton"] = player->player_board.getRemainingTileAmount(TileType::Cotton);
        boardJson["manufacturer"] = player->player_board.getRemainingTileAmount(TileType::Manufacturer);
        boardJson["pottery"] = player->player_board.getRemainingTileAmount(TileType::Pottery);
        boardJson["brewery"] = player->player_board.getRemainingTileAmount(TileType::Brewery);

        state["players"].push_back(playerJson);
    }

    state["board"] = nlohmann::json::object();
    for (const auto& pair : m_board.getCities()) {
        const auto& cityPtr = pair.second;
        nlohmann::json cityJson = {
            {"slots", nlohmann::json::array()}
        };
        for (const auto& slot : cityPtr->slots) {
            nlohmann::json slotJson;
            if (slot.placedTile) {
                // Nested structure if placedTile exists
                slotJson["placedTile"] = {
                    {"type", slot.placedTile->type},
                    {"owner", slot.placedTile->owner},
                    {"level", slot.placedTile->level},
                    {"flipped", slot.placedTile->flipped},
                    {"resource_amount", slot.placedTile->resource_amount},
                };
            } else {
                // No placedTile
                slotJson["placedTile"] = nullptr;
            }
            cityJson["slots"].push_back(slotJson);
        }
        state["board"]["cities"][cityPtr->name] = cityJson;
    }

    state["connections"] = nlohmann::json::array();
    for (const auto& connection : m_board.getPlacedLinks()) {
        state["connections"].push_back({
            {"city1", connection.city1},
            {"city2", connection.city2},
            {"owner", connection.linkOwner ? connection.linkOwner->id : -1}
        });
    }

    return state;
}

void GameState::setupBoardForTesting(const GameBoard& board) {
    // Clear existing board
    m_board = GameBoard();

    // Copy cities and their slots
    for (const auto& [cityName, cityPtr] : board.getCities()) {
        m_board.addCity(cityName);
        for (const auto& slot : cityPtr->slots) {
            m_board.addSlot(cityName, slot);
            if (slot.placedTile) {
                m_board.placeTile(cityName, m_board.getCity(cityName)->slots.size() - 1, *slot.placedTile);
            }
        }
    }

    // Copy connections
    for (const auto& connection : board.getPlacedLinks()) {
        m_board.addConnection(connection.city1, connection.city2);
        if (connection.linkOwner) {
            m_board.placeLink(connection.city1, connection.city2, connection.linkOwner);
        }
    }
}
bool GameState::handleDevelop(TileType a, TileType b, Player& player){
    if (a==TileType::NullTile) return false;
    int spent_iron;
    b==TileType::NullTile?spent_iron=1:spent_iron=2;
    int cost = iron_market.getPrice(spent_iron);
    std::cout << "Cost:" << cost << " Iron:" << spent_iron << std::endl; 
    if (cost>player.money) return false;
    player.player_board.takeTile(a);
    player.player_board.takeTile(b);
    iron_market.buy(2);
    player.money-=cost;
    return true;

}

