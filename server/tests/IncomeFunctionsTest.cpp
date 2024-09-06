#include <gtest/gtest.h>
#include "IncomeFunctions.hpp"

class IncomeFunctionsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // SetUp is called before each test
    }

    void TearDown() override {
        // TearDown is called after each test
    }
};

TEST_F(IncomeFunctionsTest, InitialIncome) {
    EXPECT_EQ(getIncome(10), 0);
    EXPECT_EQ(getIncome(19), 5);
    EXPECT_EQ(getIncome(20), 5);
    EXPECT_EQ(getIncome(21), 6);
    EXPECT_EQ(getIncome(30), 10);
}


TEST_F(IncomeFunctionsTest, TakeLoan) {
    int oldLevel = 30;
    int newLevel = takeLoan(oldLevel);
    EXPECT_EQ(getIncome(newLevel), getIncome(oldLevel) - 3);
    EXPECT_EQ(newLevel,24);
}

TEST_F(IncomeFunctionsTest, TakeLoanAtHighLevel) {
    int oldLevel = 90;
    int newLevel = takeLoan(oldLevel);
    EXPECT_EQ(getIncome(newLevel), getIncome(oldLevel) - 3);
    EXPECT_EQ(newLevel, 80);
}

TEST_F(IncomeFunctionsTest, TakeLoanAtLowLevel) {
    int oldLevel = 5;
    int newLevel = takeLoan(oldLevel);
    EXPECT_EQ(newLevel, 2);
}

TEST_F(IncomeFunctionsTest, GetIncomeOutOfRange) {
    EXPECT_THROW(getIncome(-1), std::out_of_range);
    EXPECT_THROW(getIncome(100), std::out_of_range);
}

TEST_F(IncomeFunctionsTest, TakeLoanOutOfRange) {
    EXPECT_THROW(takeLoan(-1), std::out_of_range);
    EXPECT_THROW(takeLoan(100), std::out_of_range);
}
