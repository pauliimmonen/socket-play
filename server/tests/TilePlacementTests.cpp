#include <gtest/gtest.h>
#include "GameState.hpp"
#include "GameBoard.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include "TileFactory.hpp"
#include <iostream>

class TilePlacementTest : public ::testing::Test {
protected:
    GameState gameState;
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;

    void SetUp() override {
        player1 = gameState.addPlayer();
        player2 = gameState.addPlayer();
        player1->money=30;
        player2->money=30;
        setupTestBoard();
    }

    void setupTestBoard() {
        GameBoard testBoard;
        
        // Add cities
        testBoard.addCity("CityA");
        testBoard.addCity("CityB");
        testBoard.addCity("CityC");
        testBoard.addCity("CityD");
        testBoard.addCity("CityE");

        // Add connections
        testBoard.addConnection("CityA", "CityB");
        testBoard.addConnection("CityB", "CityC");

        // Add slots to cities
        testBoard.addSlot("CityA", {{TileType::Coal}, nullptr});
        testBoard.addSlot("CityA", {{TileType::Manufacturer}, nullptr});
        testBoard.addSlot("CityA", {{TileType::Brewery}, nullptr});
        testBoard.addSlot("CityB", {{TileType::Manufacturer}, nullptr});
        testBoard.addSlot("CityB", {{TileType::Manufacturer}, nullptr});
        testBoard.addSlot("CityC", {{TileType::Coal}, nullptr});
        testBoard.addSlot("CityD", {{TileType::Coal}, nullptr});
        testBoard.addSlot("CityD", {{TileType::Iron}, nullptr});
        testBoard.addSlot("CityE", {{TileType::Brewery}, nullptr});

        // Place resource tiles
        auto coalTileA = TileFactory::createTile(TileType::Coal, 1, player1);
        testBoard.placeTile("CityA", 0, coalTileA);

        auto ironTileB = TileFactory::createTile(TileType::Iron, 1, player2);
        testBoard.placeTile("CityD", 1, ironTileB);

        auto coalTileC = TileFactory::createTile(TileType::Coal, 1, player2);
        testBoard.placeTile("CityD", 0, coalTileC);

        // Place link tiles
        testBoard.placeLink("CityA", "CityB", player1);
        testBoard.placeLink("CityB", "CityC", player2);

        gameState.setupBoardForTesting(testBoard);
    }
};

TEST_F(TilePlacementTest, PlaceTileTestResourceConsumption) {
    GameAction placeAction;
    placeAction.type = GameAction::Type::PlaceTile;
    placeAction.cityName = "CityB";
    placeAction.slotIndex = 0;
    placeAction.tileType = TileType::Manufacturer;

    // Get initial resource amounts

    // Handle the action

    auto state = gameState.getState();
    //std::cout << state << std::endl;
    EXPECT_EQ(state["board"]["cities"]["CityA"]["slots"][0]["placedTile"]["resource_amount"], 2);
    bool result = gameState.handleAction(player1->id, placeAction);
    ASSERT_TRUE(result);

    state = gameState.getState();
    EXPECT_EQ(state["board"]["cities"]["CityA"]["slots"][0]["placedTile"]["resource_amount"], 1);
    EXPECT_EQ(state["board"]["cities"]["CityB"]["slots"][0]["placedTile"]["type"], TileType::Manufacturer);
    placeAction.slotIndex=1;
    result = gameState.handleAction(player2->id, placeAction);
    
    ASSERT_TRUE(result);
    
    state = gameState.getState();
    EXPECT_EQ(state["board"]["cities"]["CityA"]["slots"][0]["placedTile"]["resource_amount"], 0);
    EXPECT_EQ(state["board"]["cities"]["CityB"]["slots"][1]["placedTile"]["type"], TileType::Manufacturer);

    placeAction.cityName = "CityA";
    result = gameState.handleAction(player1->id, placeAction);
    ASSERT_FALSE(result); //cant access to coal in slot D

    placeAction.cityName = "CityE";
    placeAction.slotIndex = 0;
    placeAction.tileType=TileType::Brewery;
    result = gameState.handleAction(player1->id, placeAction);
    ASSERT_TRUE(result);

    state = gameState.getState();
    EXPECT_EQ(state["board"]["cities"]["CityE"]["slots"][0]["placedTile"]["type"], TileType::Brewery);
    EXPECT_EQ(state["board"]["cities"]["CityD"]["slots"][1]["placedTile"]["resource_amount"], 3);//resource consumed


}

// Add more tests here for different scenarios