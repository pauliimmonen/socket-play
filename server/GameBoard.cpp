#include "GameBoard.hpp"
#include "Tile.hpp"

void GameBoard::addCity(const std::string& name, int x, int y) {
    cities[name] = City{name, {}, x, y, {}};
}

void GameBoard::addConnection(const std::string& city1, const std::string& city2) {
    cities[city1].connections.push_back(city2);
    cities[city2].connections.push_back(city1);
}

void GameBoard::addSlot(const std::string& cityName, const Slot& slot) {
    cities[cityName].slots.push_back(slot);
}

const std::unordered_map<std::string, City>& GameBoard::getCities() const {
    return cities;
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