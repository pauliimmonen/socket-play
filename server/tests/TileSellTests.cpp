#include <gtest/gtest.h>
#include "GameState.hpp"
#include "GameBoard.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include "TileFactory.hpp"
#include <iostream>

class TileSellTest : public ::testing::Test
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
        auto manufacturerTileA = TileFactory::createTile(TileType::Manufacturer, 1, player1->id);
        gameState.m_board.placeTile("Coventry", 1, manufacturerTileA);

        auto manufacturerTileB = TileFactory::createTile(TileType::Manufacturer, 2, player2->id);
        gameState.m_board.placeTile("Stafford", 0, manufacturerTileB);

        auto breweryTile = TileFactory::createTile(TileType::Brewery, 1, player1->id);
        gameState.m_board.placeTile("Walsall", 1, breweryTile);
        // Place link tiles
        gameState.m_board.placeLink("Coventry", "Birmingham", player1);
        gameState.m_board.placeLink("Birmingham", "Walsall", player2);
        gameState.m_board.placeLink("Birmingham", "Oxford", player1);

        // Place Merchant tiles
        gameState.m_board.placeTile("Oxford", 0, MerchantTile(MerchantType::Manufacturer));
        gameState.m_board.placeTile("Oxford", 1, MerchantTile(MerchantType::Empty));
    }
};

TEST_F(TileSellTest, SellTile)
{
    bool result;
    setupTestBoard();
    GameAction sellAction;
    sellAction.type = GameAction::Type::Sell;
    sellAction.cityName = "Coventry";
    sellAction.slotIndex = 1;

    auto merchan_city = gameState.m_board.getMerchantCity("Oxford");
    // merchan_city->slots[0].placedTile->resource_amount = 1;
    //  Get initial resource amounts

    // Handle the action

    auto state = gameState.getState();
    EXPECT_EQ(state["board"]["cities"]["Coventry"]["slots"][1]["placedTile"]["flipped"], false);
    EXPECT_EQ(state["board"]["cities"]["Walsall"]["slots"][1]["placedTile"]["owner"], 1);
    result = gameState.handleAction(player1->id, sellAction);
    ASSERT_TRUE(result);
    state = gameState.getState();
    EXPECT_EQ(state["board"]["cities"]["Coventry"]["slots"][1]["placedTile"]["flipped"], true);
    EXPECT_EQ(state["board"]["cities"]["Walsall"]["slots"][1]["placedTile"]["resource_amount"], 0);
}
