#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iostream>
#include "IncomeFunctions.hpp"

// Initialize the income table
std::vector<unsigned int> initializeIncomeTable() {
    std::vector<unsigned int> incomeTable;
    // Levels 0 to 10: Income increases by 1 per level
    for (int i = 0; i <= 10; ++i) {
        incomeTable.push_back(i - 10);
    }
    // Levels 11 to 30: Income increases by 1 for every 2 levels
    for (int i = 11; i <= 30; ++i) {
        incomeTable.push_back(ceil((i - 10) / 2.f));
    }
    // Levels 31 to 60: Income increases by 1 for every 3 levels
    for (int i = 31; i <= 60; ++i) {
        incomeTable.push_back(ceil(10 + (i-30) / 3.f));
    }
    // Levels 61 to 99: Income increases by 1 for every 4 levels
    for (int i = 61; i <= 99; ++i) {
        incomeTable.push_back(ceil(20 + (i-60) / 4.f));
    }
    return incomeTable;
}

// Global income table
const std::vector<unsigned int> INCOME_TABLE = initializeIncomeTable();

// Function to get income for a given level
int getIncome(unsigned int income_level) {
    if (income_level >= INCOME_TABLE.size()) {
        throw std::out_of_range("Income level out of valid range");
    }
    return INCOME_TABLE[income_level];
}

// Function to take a loan and return the new income level
unsigned int takeLoan(unsigned int income_level) {
    int currentIncome = getIncome(income_level);
    // Find the highest level with -3
    int newLevel=income_level;
    for (unsigned int i = income_level; getIncome(i) >= currentIncome-2; i--){
        newLevel = i-1;
    }
    return newLevel;
}
