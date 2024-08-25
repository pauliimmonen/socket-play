#include "GameBoard.hpp"
#include "Tile.hpp"
#include "Player.hpp"
#include <algorithm>
#include <queue>
#include <set>
#include <memory>
#include <stdexcept>
#include <iostream>

City& GameBoard::addCity(const std::string& name) {
    cities[name] = City{name, {}};
    return cities[name];
}

MerchantCity& GameBoard::addMerchantCity(const std::string& name ,MerchantBonus mb) {
    cities[name] = MerchantCity{name, mb};
    return static_cast<MerchantCity&>(cities[name]);
}

Connection& GameBoard::addConnection(const std::string& city1, const std::string& city2) {
    auto conn = connections.insert(Connection(city1, city2));
    return const_cast<Connection&>(*conn.first);
}

void GameBoard::addSlot(const std::string& cityName, const Slot& slot) {
    cities[cityName].slots.push_back(slot);
}

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
    addCity("Birmingham");
    addCity("Dudley");
    addCity("Walsall");
    addCity("Wolverhampton");
    addCity("Coalbrookdale");
    addCity("Kidderminster");
    addCity("Worcester");
    addCity("Redditch");
    addCity("Gloucester");
    addCity("Oxford");
    addCity("Coventry");
    addCity("Nuneaton");
    addCity("Tamworth");
    addCity("Burton on Trent");
    addCity("Derby");
    addCity("Stoke on Trent");
    addCity("Stone");
    addCity("Stafford");
    addCity("Cannock");

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
    addSlot("Birmingham", {{TileType::Merchant}, nullptr});

    addSlot("Coventry", {{TileType::Coal, TileType::Iron}, nullptr});
    addSlot("Coventry", {{TileType::Cotton, TileType::Manufacturer}, nullptr});
    addSlot("Coventry", {{TileType::Coal, TileType::Iron, TileType::Cotton, TileType::Manufacturer}, nullptr});
    addSlot("Coventry", {{TileType::Merchant}, nullptr});

    // Add slots for other cities...

    // Add a market slot to Birmingham (if it doesn't already exist)
    addMerchantCity("Oxford", MerchantBonus::Income2);
    addSlot("Oxford", {{TileType::Merchant}, nullptr});
    addSlot("Oxford", {{TileType::Merchant}, nullptr});

    auto birminghamIt = cities.find("Birmingham");
    if (birminghamIt == cities.end()) {
        throw std::runtime_error("Birmingham city not found");
    }

    if (birminghamIt->second.slots.size() < 5) {
        throw std::runtime_error("Birmingham doesn't have enough slots for market tile");
    }

    // Create a market tile
    MerchantTile merchantTile = MerchantTile(MerchantType::Cotton);

    // Place the market tile in Birmingham
    if (!placeTile("Birmingham", 4, merchantTile)) {
        throw std::runtime_error("Failed to place market tile in Birmingham");
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

bool GameBoard::canPlaceTile(const std::string& cityName, int slotIndex, const Tile& tile) const {
    auto cityIt = cities.find(cityName);
    if (cityIt == cities.end()) return false;
    const auto& city = cityIt->second;
    if (slotIndex < 0 || slotIndex >= static_cast<int>(city.slots.size())) return false;
    const auto& slot = city.slots[slotIndex];
    if (slot.placedTile) return false;
    return std::find(slot.allowedTileTypes.begin(), slot.allowedTileTypes.end(), tile.type) != slot.allowedTileTypes.end();
}

bool GameBoard::placeTile(const std::string& cityName, int slotIndex, const Tile& tile) {
    if (!canPlaceTile(cityName, slotIndex, tile)) return false;
    auto& slot = cities[cityName].slots[slotIndex];
    slot.placedTile = std::make_shared<Tile>(tile);
    return true;
}
