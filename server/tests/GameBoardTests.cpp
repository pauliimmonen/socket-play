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

    void setupSimpleGameBoard() {
        board.addCity("CityA");
        board.addCity("CityB");
        board.addCity("CityC");
        board.addCity("CityD");
        board.addCity("CityE");

        board.addConnection("CityA", "CityB");
        board.addConnection("CityA", "CityC");
        board.addConnection("CityB", "CityC");
        board.addConnection("CityB", "CityD");
        board.addConnection("CityC", "CityE");
        board.addConnection("CityD", "CityE");


        board.placeLink("CityA", "CityB", player1);
        board.placeLink("CityB", "CityC", player2);
        board.placeLink("CityB", "CityD", player1);
    }

    void placeLinks(const std::vector<std::tuple<std::string, std::string, std::shared_ptr<Player>>>& links) {
        for (const auto& [city1, city2, player] : links) {
            ASSERT_TRUE(board.placeLink(city1, city2, player));
        }
    }

    void addSlots(const std::vector<std::tuple<std::string, TileType>>& slots) {
        for (const auto& [city, tileType] : slots) {
            board.addSlot(city, {{tileType}, nullptr});
        }
    }

    void placeMerchantTiles(const std::vector<std::tuple<std::string, int, MerchantType>>& merchantTiles) {
        for (const auto& [city, slotIndex, merchantType] : merchantTiles) {
            MerchantTile merchantTile(merchantType);
            ASSERT_TRUE(board.placeTile(city, slotIndex, merchantTile));
        }
    }
};

TEST_F(GameBoardTest, TileProperties) {
    Tile testTile = createTestTile(TileType::Coal, 1, player1);
    // Verify tile properties
    EXPECT_EQ(testTile.type, TileType::Coal);
    EXPECT_EQ(testTile.owner, player1);
    EXPECT_EQ(testTile.level, 1);
    EXPECT_FALSE(testTile.flipped);
    EXPECT_EQ(testTile.income, 4);
    EXPECT_EQ(testTile.victory_points, 1);
    EXPECT_EQ(testTile.link_points, 2);
    EXPECT_EQ(testTile.cost_money, 5);
    EXPECT_EQ(testTile.cost_coal, 0);
    EXPECT_EQ(testTile.cost_iron, 0);
    EXPECT_EQ(testTile.resource_amount, 2);
    EXPECT_EQ(testTile.beer_demand, 0);

}

TEST_F(GameBoardTest, TileProperties2) {
    Tile testTile = createTestTile(TileType::Pottery, 4, player1);
    // Verify tile properties
    EXPECT_EQ(testTile.type, TileType::Pottery);
    EXPECT_EQ(testTile.owner, player1);
    EXPECT_EQ(testTile.level, 4);
    EXPECT_FALSE(testTile.flipped);
    EXPECT_EQ(testTile.income, 1);
    EXPECT_EQ(testTile.victory_points, 1);
    EXPECT_EQ(testTile.link_points, 1);
    EXPECT_EQ(testTile.cost_money, 0);
    EXPECT_EQ(testTile.cost_coal, 1);

}

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
    ASSERT_EQ(birminghamConnections.size(), 6);

    std::vector<std::string> expectedConnections = {"Dudley", "Walsall", "Coventry","Oxford", "Worcester", "Tamworth"};
    
    for (const auto& expectedCity : expectedConnections) {
        ASSERT_TRUE(std::find(birminghamConnections.begin(), birminghamConnections.end(), expectedCity) != birminghamConnections.end())
            << "Expected connection to " << expectedCity << " not found";
    }
}

TEST_F(GameBoardTest, GetConnectedCities) {
    setupSimpleGameBoard();
    board.addSlot("CityA", {{TileType::Coal, TileType::Iron}, nullptr});
    // Place links

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
    setupSimpleGameBoard();
    board.addSlot("CityA", {{TileType::Coal}, nullptr});
    board.addSlot("CityB", {{TileType::Coal}, nullptr});
    board.addSlot("CityC", {{TileType::Coal}, nullptr});

    Tile Coal_a = createTestTile(TileType::Coal, 1, player1);
    Tile Coal_b = createTestTile(TileType::Coal, 1, player2);

    ASSERT_TRUE(board.placeTile("CityA", 0, Coal_a));
    ASSERT_TRUE(board.placeTile("CityC", 0, Coal_b));

    int totalCoal = board.getTotalResourceCoal("CityA");
    ASSERT_EQ(totalCoal, Coal_a.resource_amount + Coal_b.resource_amount);

    totalCoal = board.getTotalResourceCoal("CityC");
    ASSERT_EQ(totalCoal, Coal_a.resource_amount + Coal_b.resource_amount);
    totalCoal = board.getTotalResourceCoal("CityE");
    ASSERT_EQ(totalCoal, 0);
}

TEST_F(GameBoardTest, PlaceMerchantTile) {
    // Add a new city
    board.addMerchantCity("CityA", MerchantBonus::Points4);
    board.addSlot("CityA", {{TileType::Merchant}, nullptr});
    board.addSlot("CityA", {{TileType::Merchant}, nullptr});

    // Create a MerchantTile
    MerchantTile merchantTile(MerchantType::Cotton);

    // Place the merchant tile in CityA
    ASSERT_TRUE(board.placeTile("CityA", 0, merchantTile));

    // Verify that the tile was placed correctly
    const auto& cities = board.getCities();
    auto cityIt = cities.find("CityA");
    ASSERT_NE(cityIt, cities.end());
    const auto& city = cityIt->second;
    
    const auto& placedTile = city->slots[0].placedTile;
    ASSERT_NE(placedTile, nullptr);
    ASSERT_EQ(placedTile->type, TileType::Merchant);

    // Cast the placedTile to MerchantTile and check its properties
    const MerchantTile* merchantTilePtr = dynamic_cast<const MerchantTile*>(placedTile.get());
    ASSERT_NE(merchantTilePtr, nullptr);
    ASSERT_EQ(merchantTilePtr->merchantType, MerchantType::Cotton);
}

TEST_F(GameBoardTest, GetConnectedMerchantCities) {
    // Set up a simple game board
    board.addCity("CityA");
    board.addMerchantCity("CityB", MerchantBonus::Points4);
    board.addCity("CityC");
    board.addMerchantCity("CityD", MerchantBonus::Income2);
    board.addCity("CityE");

    // Add connections
    board.addConnection("CityA", "CityB");
    board.addConnection("CityB", "CityC");
    board.addConnection("CityC", "CityD");
    board.addConnection("CityD", "CityE");

    // Place links
    ASSERT_TRUE(board.placeLink("CityA", "CityB", player1));
    ASSERT_TRUE(board.placeLink("CityB", "CityC", player2));
    ASSERT_TRUE(board.placeLink("CityC", "CityD", player1));

    std::vector<const MerchantCity*> connectedMerchantCities = board.getConnectedMerchantCities("CityA");
    ASSERT_EQ(connectedMerchantCities.size(), 2);
    EXPECT_EQ(connectedMerchantCities[0]->name, "CityB");
    EXPECT_EQ(connectedMerchantCities[0]->merchant_bonus, MerchantBonus::Points4);
    EXPECT_EQ(connectedMerchantCities[1]->name, "CityD");
    EXPECT_EQ(connectedMerchantCities[1]->merchant_bonus, MerchantBonus::Income2);

    connectedMerchantCities = board.getConnectedMerchantCities("CityE");
    ASSERT_EQ(connectedMerchantCities.size(), 0);

    ASSERT_TRUE(board.placeLink("CityD", "CityE", player2));
    connectedMerchantCities = board.getConnectedMerchantCities("CityE");
    ASSERT_EQ(connectedMerchantCities.size(), 2);
    EXPECT_EQ(connectedMerchantCities[0]->name, "CityD");
    EXPECT_EQ(connectedMerchantCities[1]->name, "CityB");
}

TEST_F(GameBoardTest, IsConnectedToMerchantCity) {
    // Set up a simple game board
    board.addCity("CityA");
    board.addMerchantCity("CityB", MerchantBonus::Points4);
    board.addCity("CityC");
    board.addMerchantCity("CityD", MerchantBonus::Income2);
    board.addCity("CityE");

    // Add connections
    board.addConnection("CityA", "CityB");
    board.addConnection("CityB", "CityC");
    board.addConnection("CityC", "CityD");
    board.addConnection("CityD", "CityE");

    // Place links
    ASSERT_TRUE(board.placeLink("CityA", "CityB", player1));
    ASSERT_TRUE(board.placeLink("CityB", "CityC", player2));
    ASSERT_TRUE(board.placeLink("CityC", "CityD", player1));

    // Test city directly connected to a merchant city
    EXPECT_TRUE(board.isConnectedToMerchantCity("CityA"));

    // Test city indirectly connected to a merchant city
    EXPECT_TRUE(board.isConnectedToMerchantCity("CityC"));

    // Test city not connected to any merchant city
    EXPECT_FALSE(board.isConnectedToMerchantCity("CityE"));

    // Connect CityE and test again
    ASSERT_TRUE(board.placeLink("CityD", "CityE", player2));
    EXPECT_TRUE(board.isConnectedToMerchantCity("CityE"));

    // Test a merchant city itself
    EXPECT_TRUE(board.isConnectedToMerchantCity("CityB"));

    // Test with a non-existent city (should return false)
    EXPECT_FALSE(board.isConnectedToMerchantCity("NonExistentCity"));
}

TEST_F(GameBoardTest, GetConnectedMerchantTypes) {
    // Set up a simple game board
    board.addCity("CityA");
    board.addMerchantCity("CityB", MerchantBonus::Points4);
    board.addCity("CityC");
    board.addMerchantCity("CityD", MerchantBonus::Income2);
    board.addCity("CityE");

    // Add connections
    board.addConnection("CityA", "CityB");
    board.addConnection("CityB", "CityC");
    board.addConnection("CityC", "CityD");
    board.addConnection("CityD", "CityE");

    // Place links
    ASSERT_TRUE(board.placeLink("CityA", "CityB", player1));
    ASSERT_TRUE(board.placeLink("CityB", "CityC", player2));
    ASSERT_TRUE(board.placeLink("CityC", "CityD", player1));

    // Add slots to merchant cities
    board.addSlot("CityB", {{TileType::Merchant}, nullptr});
    board.addSlot("CityD", {{TileType::Merchant}, nullptr});

    // Create merchant tiles
    MerchantTile cottonMerchant(MerchantType::Cotton);
    MerchantTile potteryMerchant(MerchantType::Pottery);

    // Place merchant tiles
    ASSERT_TRUE(board.placeTile("CityB", 0, cottonMerchant));
    ASSERT_TRUE(board.placeTile("CityD", 0, potteryMerchant));

    // Test connected merchant types from CityA
    std::set<MerchantType> connectedMerchantTypes = board.getConnectedMerchantTypes("CityA");
    ASSERT_EQ(connectedMerchantTypes.size(), 2);
    EXPECT_TRUE(connectedMerchantTypes.find(MerchantType::Cotton) != connectedMerchantTypes.end());
    EXPECT_TRUE(connectedMerchantTypes.find(MerchantType::Pottery) != connectedMerchantTypes.end());

    // Test connected merchant types from CityE (should be empty)
    connectedMerchantTypes = board.getConnectedMerchantTypes("CityE");
    ASSERT_EQ(connectedMerchantTypes.size(), 0);

    // Connect CityE and test again
    ASSERT_TRUE(board.placeLink("CityD", "CityE", player2));
    connectedMerchantTypes = board.getConnectedMerchantTypes("CityE");
    ASSERT_EQ(connectedMerchantTypes.size(), 2);
    EXPECT_TRUE(connectedMerchantTypes.find(MerchantType::Cotton) != connectedMerchantTypes.end());
    EXPECT_TRUE(connectedMerchantTypes.find(MerchantType::Pottery) != connectedMerchantTypes.end());
}

TEST_F(GameBoardTest, GetTotalResourceIron) {
    // Set up a simple game board
    board.addCity("CityA");
    board.addCity("CityB");
    board.addCity("CityC");

    // Add slots that allow Iron tiles
    board.addSlot("CityA", {{TileType::Iron}, nullptr});
    board.addSlot("CityB", {{TileType::Coal}, nullptr});
    board.addSlot("CityB", {{TileType::Iron}, nullptr});
    board.addSlot("CityC", {{TileType::Iron}, nullptr});
    board.addSlot("CityC", {{TileType::Iron}, nullptr});

    // Create Iron tiles with different resource amounts
    Tile ironTile1 = createTestTile(TileType::Iron, 1, player1);
    Tile ironTile2 = createTestTile(TileType::Iron, 2, player2);
    Tile ironTile3 = createTestTile(TileType::Iron, 3, player1);

    // Place the tiles
    ASSERT_TRUE(board.placeTile("CityA", 0, ironTile1));
    ASSERT_TRUE(board.placeTile("CityB", 1, ironTile2));
    ASSERT_TRUE(board.placeTile("CityC", 0, ironTile3));

    // Calculate expected total iron
    int expectedTotalIron = ironTile1.resource_amount + ironTile2.resource_amount + ironTile3.resource_amount;

    // Test the getTotalResourceIron function
    int totalIron = board.getTotalResourceIron();
    ASSERT_EQ(totalIron, expectedTotalIron);

    // Add another iron tile and test again
    Tile ironTile4 = createTestTile(TileType::Iron, 2, player2);
    ASSERT_TRUE(board.placeTile("CityC", 1, ironTile4));

    expectedTotalIron += ironTile4.resource_amount;
    totalIron = board.getTotalResourceIron();
    ASSERT_EQ(totalIron, expectedTotalIron);
}

TEST_F(GameBoardTest, IsCityInPlayerNetwork) {
    // Setup
    setupSimpleGameBoard();
    // Place links and tiles for players
    // Place tiles (assuming a placeTile method exists)
    board.addSlot("CityE",{{TileType::Coal}, nullptr}); 
    board.addSlot("CityE",{{TileType::Coal}, nullptr}); 
    Tile testTile = createTestTile(TileType::Coal, 1, player1);
    ASSERT_TRUE(board.placeTile("CityE", 0, testTile));
    // Test cases
    EXPECT_TRUE(board.isCityInPlayerNetwork(*player1, "CityA"));
    EXPECT_TRUE(board.isCityInPlayerNetwork(*player1, "CityB"));
    EXPECT_FALSE(board.isCityInPlayerNetwork(*player1, "CityC"));
    EXPECT_TRUE(board.isCityInPlayerNetwork(*player1, "CityE"));
    EXPECT_TRUE(board.isCityInPlayerNetwork(*player1, "CityD"));
    
    EXPECT_TRUE(board.isCityInPlayerNetwork(*player2, "CityC"));
    EXPECT_FALSE(board.isCityInPlayerNetwork(*player2, "CityD"));
    EXPECT_FALSE(board.isCityInPlayerNetwork(*player2, "CityA"));
    EXPECT_TRUE(board.isCityInPlayerNetwork(*player2, "CityB"));
    EXPECT_FALSE(board.isCityInPlayerNetwork(*player2, "CityE"));

    testTile = createTestTile(TileType::Coal, 1, player2);
    ASSERT_TRUE(board.placeTile("CityE", 1, testTile));
    EXPECT_TRUE(board.isCityInPlayerNetwork(*player2, "CityE"));


}

