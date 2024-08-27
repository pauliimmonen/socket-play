#include <gtest/gtest.h>
#include "../src/Market.hpp"
#include <iostream>

class MarketTest : public ::testing::Test {
protected:
    void SetUp() override {
        // This will be called before each test
        market = new Market(7, 2);
    }

    void TearDown() override {
        // This will be called after each test
        delete market;
    }

    Market* market;
};

TEST_F(MarketTest, InitialState) {
    EXPECT_EQ(market->getPrice(1), 1);
}

TEST_F(MarketTest, BuyOneItem) {
    int cost = market->buy(1);
    EXPECT_EQ(cost, 1);
    EXPECT_EQ(market->getPrice(1), 1);
}

TEST_F(MarketTest, BuyMultipleItems) {
    int cost = market->buy(5);
    EXPECT_EQ(cost, 9);  // 1 + 2 + 3
    EXPECT_EQ(market->getPrice(1), 3);
}

TEST_F(MarketTest, SellOneItem) {
    market->buy(7);  // Move the current price up
    auto result = market->sell(1);
    EXPECT_EQ(result.first, 1);  // 1 item sold
    EXPECT_EQ(result.second, 4); // Revenue of 6
}

TEST_F(MarketTest, SellMultipleItems) {
    market->buy(7);  // Move the current price up
    auto result = market->sell(3);
    EXPECT_EQ(result.first, 3);  // 3 items sold
    EXPECT_EQ(result.second, 4+3+3); // Revenue of 6 + 7 + 5
}

TEST_F(MarketTest, SellOverMarketCapacity) {
    market->buy(3);  // Move the current price up
    auto result = market->sell(5);
    EXPECT_EQ(result.first, 3);  // 3 items sold
    EXPECT_EQ(result.second, 4); // Revenue of 6 + 7 + 5
}


TEST_F(MarketTest, BuyWhenEmpty) {
    market->buy(13);
    EXPECT_EQ(market->getPrice(1), 7);  // Market almost empty
    int cost = market->buy(3);
    EXPECT_EQ(cost, 7+8+8);  // Sum of 1 to 8
    EXPECT_EQ(market->getPrice(1), 8);  // Market is empty
}

TEST_F(MarketTest, SellPartially) {
    market->buy(20);  // Empty the market first
    auto result = market->sell(30);  // Try to sell more than capacity
    EXPECT_EQ(result.first, 14);  // Only 14 items should be sold (2 per price)
    EXPECT_EQ(result.second, 2*(1+2+3+4+5+6+7));
}

TEST_F(MarketTest, GetPriceSmallQuantity) {
    int price = market->getPrice(3);
    EXPECT_EQ(price, 4);
    EXPECT_EQ(market->getPrice(1), 1);
}

TEST_F(MarketTest, GetPriceLargeQuantity) {
    market->buy(4);
    int price = market->getPrice(8);
    EXPECT_EQ(price, 3+3+4+4+5+5+6+6);
    EXPECT_EQ(market->getPrice(1), 3);
}

TEST_F(MarketTest, GetPriceExceedingCapacity) {
    market->buy(11);
    int price = market->getPrice(4);
    EXPECT_EQ(price, 6+7+7+8);
    
    // Verify that the market state hasn't changed
    EXPECT_EQ(market->getPrice(1), 6);
}