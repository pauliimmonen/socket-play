#include <gtest/gtest.h>
#include "GameState.hpp"
#include "GameBoard.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include "TileFactory.hpp"
#include <iostream>

class TilePlacementTest : public ::testing::Test
{
protected:
    GameState gameState;
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;

    void SetUp() override
    {
        player1 = gameState.addPlayer();
        player2 = gameState.addPlayer();
        player1->money = 30;
        player2->money = 30;
    }

    void setupTestBoard()
    {
        auto coalTileA = TileFactory::createTile(TileType::Coal, 1, player1->id);
        gameState.m_board.placeTile("Coventry", 1, coalTileA);

        auto ironTileB = TileFactory::createTile(TileType::Iron, 1, player2->id);
        gameState.m_board.placeTile("Coalbrookdale", 1, ironTileB);

        auto coalTileC = TileFactory::createTile(TileType::Coal, 1, player2->id);
        gameState.m_board.placeTile("Dudley", 0, coalTileC);

        // Place link tiles
        gameState.m_board.placeLink("Coventry", "Birmingham", player1);
        gameState.m_board.placeLink("Birmingham", "Walsall", player2);
    }
};

TEST_F(TilePlacementTest, PlaceTileTestResourceConsumption)
{
    setupTestBoard();
    GameAction placeAction;
    placeAction.type = GameAction::Type::PlaceTile;
    placeAction.cityName = "Birmingham";
    placeAction.slotIndex = 1;
    placeAction.tileType = TileType::Manufacturer;

    // Get initial resource amounts

    // Handle the action

    auto state = gameState.getState();
    // std::cout << state << std::endl;
    EXPECT_EQ(state["board"]["cities"]["Coventry"]["slots"][1]["placedTile"]["resource_amount"], 2);

    placeAction.tileType = TileType::Manufacturer;
    bool result = gameState.handleAction(player1->id, placeAction);
    ASSERT_TRUE(result);

    state = gameState.getState();
    EXPECT_EQ(state["board"]["cities"]["Coventry"]["slots"][1]["placedTile"]["resource_amount"], 1);
    EXPECT_EQ(state["board"]["cities"]["Birmingham"]["slots"][1]["placedTile"]["type"], TileType::Manufacturer);

    placeAction.slotIndex = 3;
    placeAction.tileType = TileType::Manufacturer;
    result = gameState.handleAction(player2->id, placeAction);

    ASSERT_TRUE(result);

    state = gameState.getState();
    EXPECT_EQ(state["board"]["cities"]["Coventry"]["slots"][1]["placedTile"]["resource_amount"], 0);
    EXPECT_EQ(state["board"]["cities"]["Birmingham"]["slots"][3]["placedTile"]["type"], TileType::Manufacturer);
    EXPECT_EQ(state["players"][0]["income_level"], 14);

    placeAction.cityName = "Coventry";
    placeAction.tileType = TileType::Iron;
    placeAction.slotIndex = 2;
    result = gameState.handleAction(player1->id, placeAction);
    ASSERT_FALSE(result); // cant access dudley coal

    placeAction.cityName = "Walsall";
    placeAction.slotIndex = 1;
    placeAction.tileType = TileType::Brewery;
    result = gameState.handleAction(player1->id, placeAction);
    ASSERT_TRUE(result);

    state = gameState.getState();
    EXPECT_EQ(state["board"]["cities"]["Walsall"]["slots"][1]["placedTile"]["type"], TileType::Brewery);
    EXPECT_EQ(state["board"]["cities"]["Coalbrookdale"]["slots"][1]["placedTile"]["resource_amount"], 3); // resource consumed
}

TEST_F(TilePlacementTest, TileNotConsumedFromOnFailedAction)
{
    GameAction placeAction;
    placeAction.type = GameAction::Type::PlaceTile;
    placeAction.cityName = "Leek";
    placeAction.slotIndex = 0;
    placeAction.tileType = TileType::Manufacturer;
    bool result = gameState.handleAction(player1->id, placeAction);
    ASSERT_FALSE(result);
    result = gameState.handleAction(player1->id, placeAction);
    ASSERT_FALSE(result);
}
