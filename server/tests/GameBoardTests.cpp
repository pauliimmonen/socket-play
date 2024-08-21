#include <gtest/gtest.h>
#include <string>
#include <algorithm>
#include <memory>
#include "GameBoard.hpp"
#include "GameState.hpp"
#include "TileFactory.hpp"
#include "Player.hpp"
#include "Tile.hpp"

class GameBoardTest : public ::testing::Test {
protected:
    GameBoard board;
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;

    GameBoardTest() {
        player1 = std::make_shared<Player>(1);
        player2 = std::make_shared<Player>(2);
        board.initializeBrassBirminghamMap();
    }

    Tile createTestTile(TileType type, int lvl, std::shared_ptr<Player> player) {
        return TileFactory::createTile(type, lvl, player);
    }
};

TEST_F(GameBoardTest, AddConnection) {
    board.addConnection("CityA", "CityB");
    auto connections = board.getConnections("CityA");
    ASSERT_EQ(connections.size(), 1);
    ASSERT_EQ(connections[0], "CityB");
}

TEST_F(GameBoardTest, PlaceLink) {
    board.addConnection("CityA", "CityB");
    ASSERT_TRUE(board.placeLink("CityA", "CityB", player1));
    ASSERT_FALSE(board.placeLink("CityA", "CityB", player2)); // Already placed
    ASSERT_FALSE(board.placeLink("CityA", "CityC", player1)); // Non-existent connection
}

TEST_F(GameBoardTest, GetPlacedConnections) {
    board.addConnection("CityA", "CityB");
    board.addConnection("CityB", "CityC");
    board.placeLink("CityA", "CityB", player1);

    auto placedConnections = board.getPlacedConnections();
    ASSERT_EQ(placedConnections.size(), 1);
    ASSERT_EQ(placedConnections[0].city1, "CityA");
    ASSERT_EQ(placedConnections[0].city2, "CityB");
    ASSERT_EQ(placedConnections[0].linkOwner, player1);
}

TEST_F(GameBoardTest, InitializedMapConnections) {
    board.initializeBrassBirminghamMap();
    auto birminghamConnections = board.getConnections("Birmingham");
    ASSERT_EQ(birminghamConnections.size(), 4);

    std::vector<std::string> expectedConnections = {"Dudley", "Walsall", "Coventry", "Redditch"};
    
    for (const auto& expectedCity : expectedConnections) {
        ASSERT_TRUE(std::find(birminghamConnections.begin(), birminghamConnections.end(), expectedCity) != birminghamConnections.end())
            << "Expected connection to " << expectedCity << " not found";
    }
}

TEST_F(GameBoardTest, GetConnectedCities) {
    // Set up a simple game board
    board.addCity("CityA", 0, 0);
    board.addCity("CityB", 1, 0);
    board.addCity("CityC", 2, 0);
    board.addCity("CityD", 1, 1);
    board.addCity("CityE", 2, 1);

    // Add connections
    board.addConnection("CityA", "CityB");
    board.addConnection("CityB", "CityC");
    board.addConnection("CityB", "CityD");
    board.addConnection("CityC", "CityE");
    board.addConnection("CityD", "CityE");

    board.addSlot("CityA", {{TileType::Coal, TileType::Iron}, nullptr});
    // Place links
    ASSERT_TRUE(board.placeLink("CityA", "CityB", player1));
    ASSERT_TRUE(board.placeLink("CityB", "CityC", player2));
    ASSERT_TRUE(board.placeLink("CityB", "CityD", player1));

    // Test connected cities from CityA
    std::vector<std::string> connectedCities = board.getConnectedCities("CityA");
    ASSERT_EQ(connectedCities.size(), 4);
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityB") != connectedCities.end());
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityC") != connectedCities.end());
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityD") != connectedCities.end());

    // CityE should not be in the list as there's no link to it
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityE") == connectedCities.end());

    // Test connected cities from CityD
    connectedCities = board.getConnectedCities("CityD");
    ASSERT_EQ(connectedCities.size(), 4);
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityA") != connectedCities.end());
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityB") != connectedCities.end());
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityC") != connectedCities.end());

    // Place the last link
    ASSERT_TRUE(board.placeLink("CityC", "CityE", player2));

    // Test connected cities from CityA again
    connectedCities = board.getConnectedCities("CityA");
    ASSERT_EQ(connectedCities.size(), 5);
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityB") != connectedCities.end());
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityC") != connectedCities.end());
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityD") != connectedCities.end());
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityE") != connectedCities.end());
}

TEST_F(GameBoardTest, GetTotalResourceCoal) {
    // Set up a simple game board
    board.addCity("CityA", 0, 0);
    board.addCity("CityB", 1, 0);
    board.addCity("CityC", 2, 0);

    // Add connections
    board.addConnection("CityA", "CityB");
    board.addConnection("CityB", "CityC");

    board.addSlot("CityA", {{TileType::Coal}, nullptr});
    board.addSlot("CityB", {{TileType::Coal}, nullptr});
    board.addSlot("CityC", {{TileType::Coal}, nullptr});

    // Place links
    ASSERT_TRUE(board.placeLink("CityA", "CityB", player1));
    ASSERT_TRUE(board.placeLink("CityB", "CityC", player2));

    Tile Coal_a = createTestTile(TileType::Coal, 1, player1);
    Tile Coal_b = createTestTile(TileType::Coal, 1, player2);

    // Place tiles
    ASSERT_TRUE(board.placeTile(player1, "CityA", 0, Coal_a));
    ASSERT_TRUE(board.placeTile(player2, "CityC", 0, Coal_b));

    // Test total resource_coal from CityA
    int totalCoal = board.getTotalResourceCoal("CityA");
    ASSERT_EQ(totalCoal, Coal_a.resource_coal + Coal_b.resource_coal);

    // Test total resource_coal from CityC
    totalCoal = board.getTotalResourceCoal("CityC");
    ASSERT_EQ(totalCoal, Coal_a.resource_coal + Coal_b.resource_coal);
}

TEST_F(GameBoardTest, PlaceMarketTile) {
    // Add a new city
    board.addCity("CityA", 0, 0);
    
    // Add a slot that allows market tiles
    board.addSlot("CityA", {{TileType::Market}, nullptr});

    // Create a market tile
    Tile marketTile = Tile::Builder::createMarket(MarketType::Cotton).build();

    // Place the market tile in CityA
    ASSERT_TRUE(board.placeTile(nullptr, "CityA", 0, marketTile));

    // Verify that the tile was placed correctly
    const auto& cities = board.getCities();
    auto cityIt = cities.find("CityA");
    ASSERT_NE(cityIt, cities.end());
    const auto& city = cityIt->second;
    
    ASSERT_EQ(city.slots.size(), 1);
    const auto& placedTile = city.slots[0].placedTile;
    ASSERT_NE(placedTile, nullptr);
    ASSERT_EQ(placedTile->type, TileType::Market);
    ASSERT_EQ(placedTile->marketType, MarketType::Cotton);
    ASSERT_EQ(placedTile->owner, nullptr);
}

