#ifndef MARKET_HPP
#define MARKET_HPP

#include <vector>
#include <utility>

class Market {
private:
    std::vector<int> priceTrack;
    std::vector<int> cubesAtPrice;
    int maxPrice;

    void removeCube(int price);
    void addCube(int price);

public:
    Market(int maxPrice, int slotsPerPrice);
    int getCurrentPrice() const;
    int getHighestEmptySlot() const;
    int getPrice(int quantity) const;
    int buy(int quantity);
    std::pair<int, int> sell(int quantity);
    void printMarketState() const;
};

#endif // MARKET_HPP