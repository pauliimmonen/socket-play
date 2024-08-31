#include "GameState.hpp"
#include "Tile.hpp"
#include "TileFactory.hpp"
#include <gtest/gtest.h>

class GameStateTest : public ::testing::Test {
protected:
    GameState gameState;

    // Helper function to create a tile for testing
    Tile createTestTile(TileType type, std::shared_ptr<Player> player) {
        return TileFactory::createTile(type, 1, player);
    }
};

TEST_F(GameStateTest, AddPlayer) {
    auto player = gameState.addPlayer();
    ASSERT_NE(player, nullptr);
    EXPECT_EQ(player->id, 1);
    EXPECT_EQ(player->score, 0);
    EXPECT_EQ(player->money, 30);
}

TEST_F(GameStateTest, BoardInitialization) {
    auto state = gameState.getState();

    EXPECT_TRUE(state["board"].contains("cities"));
    EXPECT_TRUE(state["board"]["cities"].contains("Birmingham"));
    EXPECT_TRUE(state["board"]["cities"].contains("Coventry"));
    EXPECT_TRUE(state["board"]["cities"].contains("Worcester"));
    EXPECT_TRUE(state["board"]["cities"].contains("Oxford"));
}

TEST_F(GameStateTest, PlaceTile) {
    auto player = gameState.addPlayer();
    int initialMoney = player->money;

    Tile testTile = createTestTile(TileType::Coal, player);

    GameAction action;
    action.type = GameAction::Type::PlaceTile;
    action.cityName = "Birmingham";
    action.slotIndex = 0;
    action.tileType = TileType::Coal;

    bool result = gameState.handleAction(player->id, action);
    EXPECT_TRUE(result);

    auto state = gameState.getState();
    EXPECT_EQ(state["board"]["cities"]["Birmingham"]["slots"][0]["placedTile"]["type"], TileType::Coal);
    EXPECT_EQ(state["board"]["cities"]["Birmingham"]["slots"][0]["placedTile"]["owner"], player->id);
    EXPECT_EQ(state["board"]["cities"]["Birmingham"]["slots"][1]["placedTile"], nullptr);
    EXPECT_EQ(player->money, initialMoney - testTile.cost_money);
}

TEST_F(GameStateTest, PlaceTileInOccupiedSlot) {
    auto player = gameState.addPlayer();
    
    GameAction action;
    action.type = GameAction::Type::PlaceTile;
    action.cityName = "Birmingham";
    action.slotIndex = 0;
    action.tileType = TileType::Coal;

    EXPECT_TRUE(gameState.handleAction(player->id, action));
    EXPECT_FALSE(gameState.handleAction(player->id, action));
}

TEST_F(GameStateTest, PlaceTileInInvalidSlot) {
    auto player = gameState.addPlayer();
    
    GameAction action;
    action.type = GameAction::Type::PlaceTile;
    action.cityName = "Birmingham";
    action.slotIndex = 10;  // Invalid slot index
    action.tileType = TileType::Coal;


    EXPECT_FALSE(gameState.handleAction(player->id, action));
}

TEST_F(GameStateTest, RemovePlayer) {
    auto player1 = gameState.addPlayer();
    auto player2 = gameState.addPlayer();

    gameState.removePlayer(player1->id);

    auto state = gameState.getState();
    EXPECT_EQ(state["players"].size(), 1);
    EXPECT_EQ(state["players"][0]["id"], player2->id);
}

TEST_F(GameStateTest, TileProperties) {
    auto player = gameState.addPlayer();
    
    Tile testTile = createTestTile(TileType::Coal, player);

    // Verify tile properties
    EXPECT_EQ(testTile.type, TileType::Coal);
    EXPECT_EQ(testTile.owner, player);
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

