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
    EXPECT_TRUE(state["board"]["cities"]["Birmingham"].contains("slots"));
}

TEST_F(GameStateTest, PlaceSimpleTile) {
    auto player = gameState.addPlayer();
    int initialMoney = player->money;

    Tile testTile = createTestTile(TileType::Cotton, player);

    GameAction action;
    action.type = GameAction::Type::PlaceTile;
    action.cityName = "Birmingham";
    action.slotIndex = 0;
    action.tileType = TileType::Cotton;

    bool result = gameState.handleAction(player->id, action);
    EXPECT_TRUE(result);

    auto state = gameState.getState();
    EXPECT_EQ(state["board"]["cities"]["Birmingham"]["slots"][0]["placedTile"]["type"], TileType::Cotton);
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
    action.tileType = TileType::Cotton;

    EXPECT_TRUE(gameState.handleAction(player->id, action));
    EXPECT_FALSE(gameState.handleAction(player->id, action));
}

TEST_F(GameStateTest, PlaceTileInInvalidSlot) {
    auto player = gameState.addPlayer();
    
    GameAction action;
    action.type = GameAction::Type::PlaceTile;
    action.cityName = "Birmingham";
    action.slotIndex = 1;  // coal not allowed
    action.tileType = TileType::Coal;


    EXPECT_FALSE(gameState.handleAction(player->id, action));
    action.slotIndex = 11;  // Invalid slot index
    EXPECT_FALSE(gameState.handleAction(player->id, action));
}

TEST_F(GameStateTest, placeLinkTile) {
    auto player = gameState.addPlayer();
    
    GameAction action;
    action.type = GameAction::Type::PlaceLink;
    action.cityName = "Birmingham";
    action.cityName2 = "Coventry";
    bool first_placed_link = gameState.handleAction(player->id, action);
    EXPECT_TRUE(first_placed_link);

    action.cityName = "Derby";
    action.cityName2 = "Conventry";
    bool impossible_link = gameState.handleAction(player->id, action);
    EXPECT_FALSE(impossible_link);
    action.cityName = "Stone";
    action.cityName2 = "Stafford";
    bool not_in_player_network = gameState.handleAction(player->id, action);
    EXPECT_FALSE(not_in_player_network);
    action.cityName = "Dudley";
    action.cityName2 = "Birmingham";
    bool in_player_network_by_connection = gameState.handleAction(player->id, action);
    EXPECT_TRUE(in_player_network_by_connection);

    action.type = GameAction::Type::PlaceTile;
    action.cityName = "Stone";
    action.tileType = TileType::Coal;
    action.slotIndex = 1;

    bool placed_tile=gameState.handleAction(player->id, action);
    EXPECT_TRUE(placed_tile);
    action.type = GameAction::Type::PlaceLink;
    action.cityName = "Stone";
    action.cityName2 = "Stafford";
    bool in_player_network_by_tile=gameState.handleAction(player->id, action);
    EXPECT_TRUE(in_player_network_by_tile);
}

TEST_F(GameStateTest, TakeLoan) {
    auto player = gameState.addPlayer();
    
    GameAction action;
    action.type = GameAction::Type::TakeLoan;

    bool loan_action_result = gameState.handleAction(player->id, action);
    EXPECT_TRUE(loan_action_result);
    auto state = gameState.getState();
    EXPECT_EQ(state["players"][0]["income_level"], 7);
}

TEST_F(GameStateTest, RemovePlayer) {
    auto player1 = gameState.addPlayer();
    auto player2 = gameState.addPlayer();

    gameState.removePlayer(player1->id);

    auto state = gameState.getState();
    EXPECT_EQ(state["players"].size(), 1);
    EXPECT_EQ(state["players"][0]["id"], player2->id);
}

