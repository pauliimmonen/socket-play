#include <gtest/gtest.h>
#include <string>
#include <algorithm>
#include "GameBoard.hpp"
#include "Player.hpp"

class GameBoardTest : public ::testing::Test {
protected:
    GameBoard board;
    Player player1;
    Player player2;

    GameBoardTest() : player1(1), player2(2) {
        board.initializeBrassBirminghamMap();
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
    ASSERT_TRUE(board.placeLink("CityA", "CityB", &player1));
    ASSERT_FALSE(board.placeLink("CityA", "CityB", &player2)); // Already placed
    ASSERT_FALSE(board.placeLink("CityA", "CityC", &player1)); // Non-existent connection
}

TEST_F(GameBoardTest, GetPlacedConnections) {
    board.addConnection("CityA", "CityB");
    board.addConnection("CityB", "CityC");
    board.placeLink("CityA", "CityB", &player1);

    auto placedConnections = board.getPlacedConnections();
    ASSERT_EQ(placedConnections.size(), 1);
    ASSERT_EQ(placedConnections[0].city1, "CityA");
    ASSERT_EQ(placedConnections[0].city2, "CityB");
    ASSERT_EQ(placedConnections[0].linkOwner, &player1);
}

TEST_F(GameBoardTest, InitializedMapConnections) {
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

    // Place links
    ASSERT_TRUE(board.placeLink("CityA", "CityB", &player1));
    ASSERT_TRUE(board.placeLink("CityB", "CityC", &player2));
    ASSERT_TRUE(board.placeLink("CityB", "CityD", &player1));

    // Test connected cities from CityA
    std::vector<std::string> connectedCities = board.getConnectedCities("CityA");
    ASSERT_EQ(connectedCities.size(), 3);
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityB") != connectedCities.end());
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityC") != connectedCities.end());
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityD") != connectedCities.end());

    // CityE should not be in the list as there's no link to it
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityE") == connectedCities.end());

    // Test connected cities from CityD
    connectedCities = board.getConnectedCities("CityD");
    ASSERT_EQ(connectedCities.size(), 3);
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityA") != connectedCities.end());
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityB") != connectedCities.end());
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityC") != connectedCities.end());

    // Place the last link
    ASSERT_TRUE(board.placeLink("CityC", "CityE", &player2));

    // Test connected cities from CityA again
    connectedCities = board.getConnectedCities("CityA");
    ASSERT_EQ(connectedCities.size(), 4);
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityB") != connectedCities.end());
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityC") != connectedCities.end());
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityD") != connectedCities.end());
    EXPECT_TRUE(std::find(connectedCities.begin(), connectedCities.end(), "CityE") != connectedCities.end());
}

