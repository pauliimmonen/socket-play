#include "GameBoard.hpp"
#include "Tile.hpp"
#include "Player.hpp"
#include <algorithm>
#include <queue>
#include <set>
#include <memory>
#include <stdexcept>
#include <iostream>

void GameBoard::addCity(const std::string& name, int x, int y) {
    cities[name] = City{name, x, y, {}};
}

void GameBoard::addConnection(const std::string& city1, const std::string& city2) {
    connections.insert(Connection(city1, city2));
}

void GameBoard::addSlot(const std::string& cityName, const Slot& slot) {
    cities[cityName].slots.push_back(slot);
}

// getCities() is now defined inline in the header file

std::vector<std::string> GameBoard::getConnections(const std::string& cityName) const {
    std::vector<std::string> connectedCities;
    for (const auto& connection : connections) {
        if (connection.city1 == cityName) {
            connectedCities.push_back(connection.city2);
        } else if (connection.city2 == cityName) {
            connectedCities.push_back(connection.city1);
        }
    }
    return connectedCities;
}

void GameBoard::initializeBrassBirminghamMap() {
    // Add all cities
    addCity("Birmingham", 0, 0);
    addCity("Dudley", -1, 0);
    addCity("Walsall", 0, -1);
    addCity("Wolverhampton", -1, -1);
    addCity("Coalbrookdale", -2, 1);
    addCity("Kidderminster", -1, 2);
    addCity("Worcester", 0, 3);
    addCity("Redditch", 1, 2);
    addCity("Gloucester", 0, 4);
    addCity("Oxford", 2, 4);
    addCity("Coventry", 2, 0);
    addCity("Nuneaton", 2, -1);
    addCity("Tamworth", 1, -2);
    addCity("Burton on Trent", 0, -3);
    addCity("Derby", 1, -4);
    addCity("Stoke on Trent", -1, -4);
    addCity("Stone", -1, -3);
    addCity("Stafford", -2, -2);
    addCity("Cannock", -1, -2);

    // Add connections
    addConnection("Birmingham", "Dudley");
    addConnection("Birmingham", "Walsall");
    addConnection("Birmingham", "Coventry");
    addConnection("Dudley", "Wolverhampton");
    addConnection("Walsall", "Wolverhampton");
    addConnection("Wolverhampton", "Coalbrookdale");
    addConnection("Coalbrookdale", "Kidderminster");
    addConnection("Kidderminster", "Worcester");
    addConnection("Worcester", "Gloucester");
    addConnection("Birmingham", "Redditch");
    addConnection("Redditch", "Worcester");
    addConnection("Coventry", "Oxford");
    addConnection("Coventry", "Nuneaton");
    addConnection("Nuneaton", "Tamworth");
    addConnection("Tamworth", "Burton on Trent");
    addConnection("Burton on Trent", "Derby");
    addConnection("Derby", "Stoke on Trent");
    addConnection("Stoke on Trent", "Stone");
    addConnection("Stone", "Stafford");
    addConnection("Stafford", "Cannock");
    addConnection("Cannock", "Wolverhampton");

    // Add slots (simplified for brevity, add all slots for each city)
    addSlot("Birmingham", {{TileType::Coal, TileType::Iron}, nullptr});
    addSlot("Birmingham", {{TileType::Cotton, TileType::Manufacturer}, nullptr});
    addSlot("Birmingham", {{TileType::Coal, TileType::Iron, TileType::Cotton, TileType::Manufacturer}, nullptr});
    addSlot("Birmingham", {{TileType::Coal, TileType::Iron, TileType::Cotton, TileType::Manufacturer}, nullptr});
    addSlot("Birmingham", {{TileType::Market}, nullptr});

    addSlot("Coventry", {{TileType::Coal, TileType::Iron}, nullptr});
    addSlot("Coventry", {{TileType::Cotton, TileType::Manufacturer}, nullptr});
    addSlot("Coventry", {{TileType::Coal, TileType::Iron, TileType::Cotton, TileType::Manufacturer}, nullptr});
    addSlot("Coventry", {{TileType::Market}, nullptr});

    // Add slots for other cities...

    // Add a market slot to Birmingham (if it doesn't already exist)
    addSlot("Birmingham", {{TileType::Market}, nullptr});

    
    try {
        auto birminghamIt = cities.find("Birmingham");
        if (birminghamIt == cities.end()) {
            throw std::runtime_error("Birmingham city not found");
        }

        if (birminghamIt->second.slots.size() < 5) {
            throw std::runtime_error("Birmingham doesn't have enough slots for market tile");
        }

        // Create a market tile
        Tile marketTile = Tile::Builder::createMarket(MarketType::Cotton).build();

        // Place the market tile in Birmingham
        if (!placeTile(nullptr, "Birmingham", 4, marketTile)) {
            throw std::runtime_error("Failed to place market tile in Birmingham");
        }
    } catch (const std::exception& e) {
        // Log the error or handle it appropriately
        std::cerr << "Error in initializeBrassBirminghamMap: " << e.what() << std::endl;
    }
   
}

std::vector<Connection> GameBoard::getPlacedConnections() const {
    std::vector<Connection> placedConnections;
    std::copy_if(connections.begin(), connections.end(), std::back_inserter(placedConnections),
                 [](const Connection& connection) { return connection.linkOwner != nullptr; });
    return placedConnections;
}

bool GameBoard::placeLink(const std::string& city1, const std::string& city2, std::shared_ptr<Player> player) {
    auto it = connections.find(Connection(city1, city2));
    if (it != connections.end() && it->linkOwner == nullptr) {
        Connection updatedConnection = *it;
        updatedConnection.linkOwner = player;
        connections.erase(it);
        connections.insert(std::move(updatedConnection));
        return true;
    }
    return false;
}

std::vector<std::string> GameBoard::getConnectedCities(const std::string& startCity) const {
    std::vector<std::string> connectedCities;
    std::set<std::string> visited;
    std::queue<std::string> queue;

    visited.insert(startCity);
    queue.push(startCity);

    while (!queue.empty()) {
        std::string currentCity = queue.front();
        queue.pop();
        connectedCities.push_back(currentCity);

        for (const auto& connection : connections) {
            if (connection.linkOwner == nullptr) continue;  // Skip if no link is placed

            std::string nextCity;
            if (connection.city1 == currentCity) {
                nextCity = connection.city2;
            } else if (connection.city2 == currentCity) {
                nextCity = connection.city1;
            } else {
                continue;  // This connection doesn't involve the current city
            }

            if (visited.find(nextCity) == visited.end()) {
                visited.insert(nextCity);
                queue.push(nextCity);
            }
        }
    }

    return connectedCities;
}

int GameBoard::getTotalResourceCoal(const std::string& startCity) const {
    int totalCoal = 0;
    std::vector<std::string> connectedCities = getConnectedCities(startCity);

    for (const auto& cityName : connectedCities) {
        const auto& city = cities.at(cityName);
        for (const auto& slot : city.slots) {
            if (slot.placedTile && slot.placedTile->type == TileType::Coal) {
                totalCoal += slot.placedTile->resource_coal;
            }
        }
    }

    return totalCoal;
}

bool GameBoard::placeTile(std::shared_ptr<Player> player, const std::string& cityName, int slotIndex, const Tile& tile) {
    auto cityIt = cities.find(cityName);
    if (cityIt == cities.end()) {
        return false;  // City not found
    }
    auto& city = cityIt->second;

    if (slotIndex < 0 || slotIndex >= static_cast<int>(city.slots.size())) {
        return false;  // Invalid slot index
    }
    auto& slot = city.slots[slotIndex];

    if (slot.placedTile) {
        return false;  // Slot already occupied
    }

    if (std::find(slot.allowedTileTypes.begin(), slot.allowedTileTypes.end(), tile.type) == slot.allowedTileTypes.end()) {
        return false;  // Tile type not allowed in this slot
    }

    // For market tiles or other special cases, player can be nullptr
    if (player) {
        if (player->money < tile.cost_money) {
            return false;  // Not enough money
        }
        player->money -= tile.cost_money;
        player->score += tile.victory_points;
    }

    slot.placedTile = std::make_shared<Tile>(tile);
    if (player) {
        slot.placedTile->owner = player;  // Set the owner of the placed tile
    }

    calculateLinkPoints(cityName);

    return true;
}

void GameBoard::calculateLinkPoints(const std::string& cityName) {
    // TODO: Implement link points calculation logic
    // This function should update the link_points for tiles in the given city
    // and potentially in connected cities based on the game rules
    (void)cityName; // Silence unused parameter warning
}

