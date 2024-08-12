#include "GameBoard.hpp"
#include "Tile.hpp"
#include <algorithm>
#include <queue>
#include <set>

void GameBoard::addCity(const std::string& name, int x, int y) {
    cities[name] = City{name, x, y, {}};
}

void GameBoard::addConnection(const std::string& city1, const std::string& city2) {
    connections.insert(Connection(city1, city2));
}

void GameBoard::addSlot(const std::string& cityName, const Slot& slot) {
    cities[cityName].slots.push_back(slot);
}

const std::unordered_map<std::string, City>& GameBoard::getCities() const {
    return cities;
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

    addSlot("Coventry", {{TileType::Coal, TileType::Iron}, nullptr});
    addSlot("Coventry", {{TileType::Cotton, TileType::Manufacturer}, nullptr});
    addSlot("Coventry", {{TileType::Coal, TileType::Iron, TileType::Cotton, TileType::Manufacturer}, nullptr});

    // Add slots for other cities...
}

std::vector<Connection> GameBoard::getPlacedConnections() const {
    std::vector<Connection> placedConnections;
    std::copy_if(connections.begin(), connections.end(), std::back_inserter(placedConnections),
                 [](const Connection& connection) { return connection.linkOwner != nullptr; });
    return placedConnections;
}

bool GameBoard::placeLink(const std::string& city1, const std::string& city2, Player* player) {
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

    // Remove the start city from the result
    connectedCities.erase(connectedCities.begin());

    return connectedCities;
}