#include "GameBoard.hpp"
#include "Tile.hpp"
#include "Player.hpp"
#include <algorithm>
#include <queue>
#include <set>
#include <memory>
#include <stdexcept>
#include <iostream>

City* GameBoard::addCity(const std::string& name) {
    auto city = std::make_unique<City>(name);
    auto* cityPtr = city.get();
    cities[name] = std::move(city);
    return cityPtr;
}

MerchantCity* GameBoard::addMerchantCity(const std::string& name, MerchantBonus mb) {
    auto city = std::make_unique<MerchantCity>(name, mb);
    auto* cityPtr = city.get();
    cities[name] = std::move(city);
    return cityPtr;
}

Connection& GameBoard::addConnection(const std::string& city1, const std::string& city2) {
    auto conn = connections.insert(Connection(city1, city2));
    return const_cast<Connection&>(*conn.first);
}

void GameBoard::addSlot(const std::string& cityName, const Slot& slot) {
    cities[cityName]->slots.push_back(slot);
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
    addCity("Coventry");
    addCity("Nuneaton");
    addCity("Redditch");
    addCity("Dudley");
    addCity("Coalbrookdale");
    addCity("Wolverhampton");
    addCity("Kidderminster");
    addCity("Worcester");
    addCity("Walsall");
    addCity("Cannock");
    addCity("Tamworth");
    addCity("Burton-on-Trent");
    addCity("Stafford");
    addCity("Stone");
    addCity("Uttoxeter");
    addCity("Stoke-on-Trent");
    addCity("Leek");
    addCity("Belper");
    addCity("Derby");
    addMerchantCity("Oxford", MerchantBonus::Income2);
    addMerchantCity("Gloucester", MerchantBonus::Develop);
    addMerchantCity("Shrewsbury", MerchantBonus::Points4);
    addMerchantCity("Warrington", MerchantBonus::Money5);
    addMerchantCity("Nottingham", MerchantBonus::Points3);
    // Add connections
    addConnection("Birmingham", "Dudley");
    addConnection("Birmingham", "Walsall");
    addConnection("Birmingham", "Coventry");
    addConnection("Birmingham", "Oxford");
    addConnection("Birmingham", "Tamworth");
    addConnection("Birmingham", "Worcester");
    addConnection("Redditch", "Oxford");
    addConnection("Redditch", "Gloucester");
    addConnection("Worcester", "Gloucester");
    addConnection("Worcester", "Kidderminster");
    addConnection("Kidderminster","Dudley");
    addConnection("Kidderminster","Coalbrookdale");
    addConnection("Coalbrookdale", "Wolverhampton");
    addConnection("Coalbrookdale", "Sherewsbury");
    addConnection("Dudley", "Wolverhampton");
    addConnection("Walsall", "Wolverhampton");
    addConnection("Walsall", "Cannock");
    addConnection("Walsall", "Burton-on-Trent");
    addConnection("Nuneaton", "Tamworth");
    addConnection("Burton-on-Trent", "Tamworth");
    addConnection("Burton-on-Trent", "Stone");
    addConnection("Burton-on-Trent", "Derby");
    addConnection("Stafford", "Cannock");
    addConnection("Stafford", "Stone");
    addConnection("Stoke-on-Trent", "Stone");
    addConnection("Stoke-on-Trent", "Leek");
    addConnection("Stoke-on-Trent", "Warrington");
    addConnection("Derby", "Belper");
    addConnection("Derby", "Nottingham");
    addConnection("Belper", "Leek");


    // Add slots 
    addSlot("Birmingham", {{TileType::Manufacturer, TileType::Cotton}, nullptr});
    addSlot("Birmingham", {{TileType::Manufacturer}, nullptr});
    addSlot("Birmingham", {{TileType::Iron}, nullptr});
    addSlot("Birmingham", {{TileType::Manufacturer}, nullptr});

    addSlot("Coventry", {{TileType::Pottery}, nullptr});
    addSlot("Coventry", {{TileType::Manufacturer, TileType::Coal}, nullptr});
    addSlot("Coventry", {{TileType::Iron, TileType::Manufacturer}, nullptr});

    addSlot("Nuneaton", {{TileType::Coal, TileType::Cotton}, nullptr});
    addSlot("Nuneaton", {{TileType::Manufacturer, TileType::Brewery}, nullptr});

    addSlot("Redditch", {{TileType::Manufacturer, TileType::Coal}, nullptr});
    addSlot("Redditch", {{TileType::Iron}, nullptr});

    addSlot("Worcester", {{TileType::Cotton}, nullptr});
    addSlot("Worcester", {{TileType::Cotton}, nullptr});

    addSlot("Kidderminster", {{TileType::Cotton, TileType::Coal}, nullptr});
    addSlot("Kidderminster", {{TileType::Cotton}, nullptr});

    addSlot("Dudley", {{TileType::Coal}, nullptr});
    addSlot("Dudley", {{TileType::Iron}, nullptr});

    addSlot("Coalbrookdale", {{TileType::Iron, TileType::Brewery}, nullptr});
    addSlot("Coalbrookdale", {{TileType::Iron}, nullptr});
    addSlot("Coalbrookdale", {{TileType::Coal}, nullptr});

    addSlot("Wolverhampton", {{TileType::Manufacturer}, nullptr});
    addSlot("Wolverhampton", {{TileType::Manufacturer, TileType::Coal}, nullptr});

    addSlot("Walsall", {{TileType::Iron, TileType::Manufacturer}, nullptr});
    addSlot("Walsall", {{TileType::Manufacturer, TileType::Brewery}, nullptr});

    addSlot("Tamworth", {{TileType::Cotton, TileType::Coal}, nullptr});
    addSlot("Tamworth", {{TileType::Cotton, TileType::Coal}, nullptr});

    addSlot("Cannock", {{TileType::Manufacturer, TileType::Coal}, nullptr});
    addSlot("Cannock", {{TileType::Coal}, nullptr});

    addSlot("Burton-on-Trent", {{TileType::Manufacturer, TileType::Brewery}, nullptr});
    addSlot("Burton-on-Trent", {{TileType::Pottery}, nullptr});

    addSlot("Stafford", {{TileType::Manufacturer, TileType::Coal}, nullptr});
    addSlot("Stafford", {{TileType::Coal}, nullptr});

    addSlot("Stone", {{TileType::Cotton, TileType::Brewery}, nullptr});
    addSlot("Stone", {{TileType::Manufacturer, TileType::Coal}, nullptr});

    addSlot("Uttoxeter", {{TileType::Manufacturer, TileType::Brewery}, nullptr});
    addSlot("Uttoxeter", {{TileType::Cotton, TileType::Brewery}, nullptr});

    addSlot("Stoke-on-Trent", {{TileType::Cotton, TileType::Manufacturer}, nullptr});
    addSlot("Stoke-on-Trent", {{TileType::Pottery, TileType::Iron}, nullptr});
    addSlot("Stoke-on-Trent", {{TileType::Manufacturer}, nullptr});

    addSlot("Leek", {{TileType::Cotton, TileType::Manufacturer}, nullptr});
    addSlot("Leek", {{TileType::Cotton, TileType::Coal}, nullptr});

    addSlot("Derby", {{TileType::Cotton, TileType::Brewery}, nullptr});
    addSlot("Derby", {{TileType::Cotton, TileType::Manufacturer}, nullptr});
    addSlot("Derby", {{TileType::Iron}, nullptr});

    addSlot("Belper", {{TileType::Cotton, TileType::Brewery}, nullptr});
    addSlot("Belper", {{TileType::Coal}, nullptr});
    addSlot("Belper", {{TileType::Pottery}, nullptr});

    //Merhcant cities slots
    addSlot("Oxford", {{TileType::Merchant}, nullptr});
    addSlot("Oxford", {{TileType::Merchant}, nullptr});

    addSlot("Gloucester", {{TileType::Merchant}, nullptr});
    addSlot("Gloucester", {{TileType::Merchant}, nullptr});

    addSlot("Shrewsbury", {{TileType::Merchant}, nullptr});

    addSlot("Nottingham", {{TileType::Merchant}, nullptr});
    addSlot("Nottingham", {{TileType::Merchant}, nullptr});

    addSlot("Warrington", {{TileType::Merchant}, nullptr});
    addSlot("Warrington", {{TileType::Merchant}, nullptr});
}

std::vector<Connection> GameBoard::getPlacedLinks() const {
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
        for (const auto& slot : city->slots) {
            if (slot.placedTile && slot.placedTile->type == TileType::Coal) {
                totalCoal += slot.placedTile->resource_amount;
            }
        }
    }
    return totalCoal;
}

int GameBoard::getTotalResourceIron() const {
    int totalIron = 0;
    for (const auto& [cityName, city] : cities) {
        for (const auto& slot : city->slots) {
            if (slot.placedTile && slot.placedTile->type == TileType::Iron) {
                totalIron += slot.placedTile->resource_amount;
            }
        }
    }
    return totalIron;
}

bool GameBoard::canPlaceTile(const std::string& cityName, int slotIndex, const Tile& tile) const {
    auto cityIt = cities.find(cityName);
    if (cityIt == cities.end()) return false;
    const auto& city = cityIt->second;
    if (slotIndex < 0 || slotIndex >= static_cast<int>(city->slots.size())) return false;
    const auto& slot = city->slots[slotIndex];
    if (slot.placedTile) return false;
    return std::find(slot.allowedTileTypes.begin(), slot.allowedTileTypes.end(), tile.type) != slot.allowedTileTypes.end();
}

bool GameBoard::placeTile(const std::string& cityName, int slotIndex, const Tile& tile) {
    if (!canPlaceTile(cityName, slotIndex, tile)) return false;
    auto& slot = cities[cityName]->slots[slotIndex];
    slot.placedTile = tile.clone();  // Use the clone method instead of direct copying
    return true;
}

const MerchantCity* GameBoard::getMerchantCity(const std::string& cityName) const {
    auto it = cities.find(cityName);
    if (it != cities.end()) {
        const auto& city = it->second;
        return dynamic_cast<const MerchantCity*>(city.get());
    }
    return nullptr;
}

bool GameBoard::isConnectedToMerchantCity(const std::string& cityName) const {
    std::vector<std::string> connectedCities = getConnectedCities(cityName);
    
    for (const auto& connectedCity : connectedCities) {
        if (getMerchantCity(connectedCity) != nullptr) {
            return true;
        }
    }
    
    return false;
}

std::vector<const MerchantCity*> GameBoard::getConnectedMerchantCities(const std::string& cityName) const {
    std::vector<const MerchantCity*> connectedMerchantCities;
    std::vector<std::string> connectedCities = getConnectedCities(cityName);

    for (const auto& connectedCity : connectedCities) {
        const MerchantCity* merchantCity = getMerchantCity(connectedCity);
        if (merchantCity != nullptr) {
            connectedMerchantCities.push_back(merchantCity);
        }
    }
    return connectedMerchantCities;
}

std::set<MerchantType> GameBoard::getConnectedMerchantTypes(const std::string& cityName) const {
    std::set<MerchantType> merchantTypes;
    std::vector<const MerchantCity*> connectedMerchantCities = getConnectedMerchantCities(cityName);

    for (const auto* merchantCity : connectedMerchantCities) {
        for (const auto& slot : merchantCity->slots) {
            if (slot.placedTile && slot.placedTile->type == TileType::Merchant) {
                const MerchantTile* merchantTile = dynamic_cast<const MerchantTile*>(slot.placedTile.get());
                if (merchantTile) {
                    merchantTypes.insert(merchantTile->merchantType);
                }
            }
        }
    }

    return merchantTypes;
}

std::vector<std::string> GameBoard::getPlayerPlacedTiles(const Player& player) const {
    std::vector<std::string> playerTiles;
    for (const auto& [cityName, city] : cities) {
        for (const auto& slot : city->slots) {
            if (slot.placedTile && slot.placedTile->owner.get() == &player) {
                playerTiles.push_back(cityName);
            }
        }
    }
    return playerTiles;
}

std::vector<Connection> GameBoard::getPlayerPlacedLinks(const Player& player) const {
    std::vector<Connection> playerLinks;
    for (const auto& connection : connections) {
        if (connection.linkOwner.get() == &player) {
            playerLinks.push_back(connection);
        }
    }
    return playerLinks;
}

bool GameBoard::isCityInPlayerNetwork(const Player& player, const std::string& cityName) {
    auto links = getPlayerPlacedLinks(player);
    auto tiles = getPlayerPlacedTiles(player);

    std::set<std::string> networkCities;

    // Add cities from links
    for (const auto& link : links) {
        networkCities.insert(link.city1);
        networkCities.insert(link.city2);
    }

    // Add cities from tiles
    for (const auto& tile : tiles) {
        networkCities.insert(tile);
    }
    //No placed link or tiles free to place anywhere
    if (networkCities.size()==0) return true;
    // Check if the given cityName is in the network
    return networkCities.find(cityName) != networkCities.end();
}

const City* GameBoard::getCity(const std::string& cityName) const {
    auto it = cities.find(cityName);
    if (it != cities.end()) {
        return it->second.get();
    }
    return nullptr;
}
