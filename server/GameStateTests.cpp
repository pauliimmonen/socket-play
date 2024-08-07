#include "GameState.hpp"
#include "Tile.hpp"
#include "TileFactory.hpp"
#include <gtest/gtest.h>

class GameStateTest : public ::testing::Test {
protected:
    GameState gameState;

    // Helper function to create a tile for testing
    Tile createTestTile(const std::string& type, std::shared_ptr<Player> player) {
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
    int initialScore = player->score;

    Tile testTile = createTestTile("Coal", player);

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
    EXPECT_EQ(player->score, initialScore + testTile.victory_points);
    EXPECT_EQ(player->money, initialMoney - testTile.cost_money);
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
    
    Tile testTile = createTestTile("Coal", player);

    GameAction action;
    action.type = GameAction::Type::PlaceTile;
    action.cityName = "Birmingham";
    action.slotIndex = 0;
    action.tileType = "Coal";

    EXPECT_TRUE(gameState.handleAction(player->id, action));

    // Verify tile properties
    EXPECT_EQ(testTile.type, "Coal");
    EXPECT_EQ(testTile.owner, player);
    EXPECT_EQ(testTile.level, 1);
    EXPECT_FALSE(testTile.flipped);
    EXPECT_EQ(testTile.income, 4);
    EXPECT_EQ(testTile.victory_points, 1);
    EXPECT_EQ(testTile.link_points, 2);
    EXPECT_EQ(testTile.cost_money, 5);
    EXPECT_EQ(testTile.cost_coal, 0);
    EXPECT_EQ(testTile.cost_iron, 0);
    EXPECT_EQ(testTile.resource_coal, 0);
    EXPECT_EQ(testTile.resource_iron, 0);
    EXPECT_EQ(testTile.initial_resource_amount, 2);
    EXPECT_EQ(testTile.sell_beer_cost, 0);

    // Verify essential game state information
    auto state = gameState.getState();
    auto placedTile = state["board"]["cities"]["Birmingham"]["slots"][0];
    EXPECT_EQ(placedTile["placedTile"], "Coal");
    EXPECT_EQ(placedTile["owner"], player->id);
}

TEST_F(GameStateTest, InitialResourceAmount) {
    auto player = gameState.addPlayer();
    auto state = gameState.getState();

    // Check Coal tile
    auto coalTile = std::find_if(state["availableTiles"].begin(), state["availableTiles"].end(),
                                 [](const auto& tile) { return tile["type"] == "Coal"; });
    ASSERT_NE(coalTile, state["availableTiles"].end());
    EXPECT_EQ((*coalTile)["initial_resource_amount"], 2);

    // Check Iron tile
    auto ironTile = std::find_if(state["availableTiles"].begin(), state["availableTiles"].end(),
                                 [](const auto& tile) { return tile["type"] == "Iron"; });
    ASSERT_NE(ironTile, state["availableTiles"].end());
    EXPECT_EQ((*ironTile)["initial_resource_amount"], 2);

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
