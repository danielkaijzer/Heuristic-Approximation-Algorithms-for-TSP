#include <iostream>
// #include "NearestNeighbor.hpp"
#include "GreedyTSP.hpp"



int main() {
    // nearestNeighbor("wi29.tsp"); // 36,388
    // std::cout << std::endl;

    greedyTSP("wi29.tsp"); // 28,863
    std::cout << std::endl;

    // greedyTSP("qa194.tsp"); // 28,230
    // std::cout << std::endl;

    // nearestNeighbor("qa194.tsp");
    // std::cout << std::endl;

    // nearestNeighbor("zi929.tsp");
    // std::cout << std::endl;

    // nearestNeighbor("ja9847.tsp");
    // std::cout << std::endl;

    // nearestNeighbor("");

    return 0;
}
