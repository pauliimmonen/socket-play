#ifndef INCOME_FUNCTIONS_HPP
#define INCOME_FUNCTIONS_HPP

#include <vector>
#include <algorithm>

// Global income table declaration

// Function to get income for a given level
unsigned int getIncome(unsigned int income_level);

// Function to take a loan and return the new income level
unsigned int takeLoan(unsigned int income_level);

#endif // INCOME_FUNCTIONS_HPP