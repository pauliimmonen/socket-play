#include "GameState.hpp"
#include "Tile.hpp"
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
    int initialScore = player->score;

    GameAction action;
    action.type = GameAction::Type::PlaceTile;
    action.cityName = "Birmingham";
    action.slotIndex = 0;
    action.tileType = "Coal";

    bool result = gameState.handleAction(player->id, action);
    EXPECT_TRUE(result);

    auto state = gameState.getState();
    EXPECT_EQ(state["board"]["cities"]["Birmingham"]["slots"][0]["placedTile"], "Coal");
    EXPECT_EQ(state["board"]["cities"]["Birmingham"]["slots"][0]["owner"], player->id);
    EXPECT_EQ(player->score, initialScore + 1);  // Assuming Coal tile gives 1 victory point
    EXPECT_EQ(player->money, initialMoney - 5);  // Assuming Coal tile costs 5 money
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

TEST_F(GameStateTest, TileProperties) {
    auto player = gameState.addPlayer();
    
    GameAction action;
    action.type = GameAction::Type::PlaceTile;
    action.cityName = "Birmingham";
    action.slotIndex = 0;
    action.tileType = "Coal";

    EXPECT_TRUE(gameState.handleAction(player->id, action));

    auto state = gameState.getState();
    auto placedTile = state["board"]["cities"]["Birmingham"]["slots"][0];
    
    EXPECT_EQ(placedTile["placedTile"], "Coal");
    EXPECT_EQ(placedTile["owner"], player->id);
    EXPECT_EQ(placedTile["level"], 1);
    EXPECT_FALSE(placedTile["flipped"]);
    EXPECT_EQ(placedTile["income"], 1);
    EXPECT_EQ(placedTile["victory_points"], 1);
    EXPECT_EQ(placedTile["link_points"], 0);
    EXPECT_EQ(placedTile["cost_money"], 5);
    EXPECT_EQ(placedTile["cost_coal"], 0);
    EXPECT_EQ(placedTile["cost_iron"], 0);
    EXPECT_EQ(placedTile["resource_coal"], 2);
    EXPECT_EQ(placedTile["resource_iron"], 0);
    EXPECT_EQ(placedTile["initial_resource_amount"], 2);
}

TEST_F(GameStateTest, InitialResourceAmount) {
    auto player = gameState.addPlayer();
    auto state = gameState.getState();

    // Check Coal tile
    auto coalTile = std::find_if(state["availableTiles"].begin(), state["availableTiles"].end(),
                                 [](const auto& tile) { return tile["type"] == "Coal"; });
    ASSERT_NE(coalTile, state["availableTiles"].end());
    EXPECT_EQ((*coalTile)["initial_resource_amount"], 1);

    // Check Iron tile
    auto ironTile = std::find_if(state["availableTiles"].begin(), state["availableTiles"].end(),
                                 [](const auto& tile) { return tile["type"] == "Iron"; });
    ASSERT_NE(ironTile, state["availableTiles"].end());
    EXPECT_EQ((*ironTile)["initial_resource_amount"], 1);

    // Check Cotton tile
    auto cottonTile = std::find_if(state["availableTiles"].begin(), state["availableTiles"].end(),
                                   [](const auto& tile) { return tile["type"] == "Cotton"; });
    ASSERT_NE(cottonTile, state["availableTiles"].end());
    EXPECT_EQ((*cottonTile)["initial_resource_amount"], 0);

    // Check Manufacturer tile
    auto manufacturerTile = std::find_if(state["availableTiles"].begin(), state["availableTiles"].end(),
                                         [](const auto& tile) { return tile["type"] == "Manufacturer"; });
    ASSERT_NE(manufacturerTile, state["availableTiles"].end());
    EXPECT_EQ((*manufacturerTile)["initial_resource_amount"], 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
