#include <gtest/gtest.h>
#include "PlayerBoard.hpp"
#include "TileFactory.hpp"

class PlayerBoardTest : public ::testing::Test {
protected:
    PlayerBoard board;
};

TEST_F(PlayerBoardTest, InitializationTest) {
    // Check if all tile types are initialized
    EXPECT_TRUE(board.hasTiles(TileType::Coal));
    EXPECT_TRUE(board.hasTiles(TileType::Iron));
    EXPECT_TRUE(board.hasTiles(TileType::Cotton));
    EXPECT_TRUE(board.hasTiles(TileType::Manufacturer));
    EXPECT_TRUE(board.hasTiles(TileType::Pottery));
    EXPECT_TRUE(board.hasTiles(TileType::Brewery));
}

TEST_F(PlayerBoardTest, PeekTileTest) {
    // Check if peeking returns the correct tile type and level
    auto coalTile = board.peekTile(TileType::Coal);
    EXPECT_EQ(coalTile->type, TileType::Coal);
    EXPECT_EQ(coalTile->level, 1);

    auto ironTile = board.peekTile(TileType::Iron);
    EXPECT_EQ(ironTile->type, TileType::Iron);
    EXPECT_EQ(ironTile->level, 1);
}

TEST_F(PlayerBoardTest, TakeTileTest) {
    // Take a tile and check if it's removed from the board
    auto coalTile = board.takeTile(TileType::Coal);
    EXPECT_EQ(coalTile->type, TileType::Coal);
    EXPECT_EQ(coalTile->level, 1);

    // The next coal tile should be level 2
    auto nextCoalTile = board.peekTile(TileType::Coal);
    EXPECT_EQ(nextCoalTile->type, TileType::Coal);
    EXPECT_EQ(nextCoalTile->level, 2);
}

TEST_F(PlayerBoardTest, HasTilesTest) {
    // Check if hasTiles returns true for existing tiles
    EXPECT_TRUE(board.hasTiles(TileType::Coal));
    int coal_tile_count=0;
    // Remove all coal tiles
    while (board.hasTiles(TileType::Coal)) {
        board.takeTile(TileType::Coal);
        coal_tile_count++;
    }
    // Check if hasTiles returns false for depleted tiles
    EXPECT_EQ(coal_tile_count, 7);
    EXPECT_FALSE(board.hasTiles(TileType::Coal));
}

