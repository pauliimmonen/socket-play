#include "Market.hpp"
#include <iostream>

Market::Market(int maxPrice, int slotsPerPrice) : maxPrice(maxPrice) {
    priceTrack.resize(maxPrice);
    cubesAtPrice.resize(maxPrice, slotsPerPrice);
    for (int i = 0; i < maxPrice; ++i) {
        priceTrack[i] = i + 1;
    }
}

int Market::getPrice(int quantity) const {
    int totalCost = 0;
    int remainingQuantity = quantity;
    std::vector<int> tempCubesAtPrice = cubesAtPrice;  // Create a temporary copy

    for (int i = 0; i < maxPrice && remainingQuantity > 0; ++i) {
        while (tempCubesAtPrice[i] > 0 && remainingQuantity > 0) {
            totalCost += priceTrack[i];
            tempCubesAtPrice[i]--;
            remainingQuantity--;
        }
    }

    // If there are not enough cubes in the market, use the highest price for the remaining quantity
    if (remainingQuantity > 0) {
        totalCost += remainingQuantity * (maxPrice + 1);
    }

    return totalCost;
}

int Market::buy(int quantity) {
    int totalCost = 0;
    for (int i = 0; i < quantity; ++i) {
        int price = getCurrentPrice();
        totalCost += price;
        removeCube(price);
    }
    return totalCost;
}

std::pair<int, int> Market::sell(int quantity) {
    int itemsSold = 0;
    int totalRevenue = 0;
    while (itemsSold < quantity) {
        int price = getHighestEmptySlot();
        if (price == 0) {
            break;  // Market is full, stop selling
        }
        totalRevenue += price;
        addCube(price);
        itemsSold++;
    }
    return std::make_pair(itemsSold, totalRevenue);
}

void Market::printMarketState() const {
    for (int i = maxPrice - 1; i >= 0; --i) {
        std::cout << "Price " << priceTrack[i] << ": " << cubesAtPrice[i] << " cubes" << std::endl;
    }
}

int Market::getCurrentPrice() const {
    for (int i = 0; i < maxPrice; ++i) {
        if (cubesAtPrice[i] > 0) {
            return priceTrack[i];
        }
    }
    return maxPrice+1; // Market is empty
}

int Market::getHighestEmptySlot() const {
    for (int i = maxPrice - 1; i >= 0; --i) {
        if (cubesAtPrice[i] < 2) {
            return priceTrack[i];
        }
    }
    return 0; // Market is full
}

void Market::removeCube(int price) {
    int index = price - 1;
    if (index >= 0 && index < maxPrice && cubesAtPrice[index] > 0) {
        cubesAtPrice[index]--;
    }
}

void Market::addCube(int price) {
    int index = price - 1;
    if (index >= 0 && index < maxPrice && cubesAtPrice[index] < 2) {
        cubesAtPrice[index]++;
    }
}