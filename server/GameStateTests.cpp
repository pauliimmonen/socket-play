#include "GameState.hpp"
#include <gtest/gtest.h>

class GameStateTest : public ::testing::Test {
protected:
    GameState gameState;
};

TEST_F(GameStateTest, AddPlayer) {
    auto player = gameState.addPlayer();
    ASSERT_NE(player, nullptr);
    EXPECT_EQ(player->id, 1);
    EXPECT_EQ(player->score, 0);
    EXPECT_EQ(player->money, 30);
}

TEST_F(GameStateTest, PlaceTile) {
    auto player = gameState.addPlayer();
    int initialMoney = player->money;
    int initialScore = player->score;

    GameAction action;
    action.type = GameAction::Type::PlaceTile;
    action.cityName = "Birmingham";
    action.slotIndex = 0;
    action.tileType = "Coal";

    bool result = gameState.handleAction(player->id, action);
    EXPECT_TRUE(result);

    auto state = gameState.getState();
    EXPECT_EQ(state["cities"]["Birmingham"]["slots"][0]["placedTile"], "Coal");
    EXPECT_EQ(player->score, initialScore + 1);
    EXPECT_EQ(player->money, initialMoney - 5);
}

TEST_F(GameStateTest, PlaceTileInOccupiedSlot) {
    auto player = gameState.addPlayer();
    
    GameAction action;
    action.type = GameAction::Type::PlaceTile;
    action.cityName = "Birmingham";
    action.slotIndex = 0;
    action.tileType = "Coal";

    EXPECT_TRUE(gameState.handleAction(player->id, action));
    EXPECT_FALSE(gameState.handleAction(player->id, action));
}

TEST_F(GameStateTest, PlaceNonExistentTile) {
    auto player = gameState.addPlayer();
    
    GameAction action;
    action.type = GameAction::Type::PlaceTile;
    action.cityName = "Birmingham";
    action.slotIndex = 0;
    action.tileType = "NonExistentTile";

    EXPECT_FALSE(gameState.handleAction(player->id, action));
}

TEST_F(GameStateTest, PlaceTileInInvalidSlot) {
    auto player = gameState.addPlayer();
    
    GameAction action;
    action.type = GameAction::Type::PlaceTile;
    action.cityName = "Birmingham";
    action.slotIndex = 10;  // Invalid slot index
    action.tileType = "Coal";

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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}