#include <iostream>
// #include "NearestNeighbor.hpp"
#include "GreedyTSP.hpp"



int main() {
    greedyTSP("wi29.tsp"); // 30,052
    std::cout << std::endl;

    // greedyTSP("dj38.tsp"); // 7019, time 1ms
    // std::cout << std::endl;

    // greedyTSP("qa194.tsp"); // 11,521, time: 29ms
    // std::cout << std::endl;

    // nearestNeighbor("qa194.tsp");
    // std::cout << std::endl;

    // nearestNeighbor("zi929.tsp");
    // std::cout << std::endl;

    // greedyTSP("zi929.tsp"); // 102,456, time 414ms
    // std::cout << std::endl;

    // greedyTSP("ja9847.tsp"); // 557,018, time 73818ms
    // std::cout << std::endl;

    // nearestNeighbor("ja9847.tsp");
    // std::cout << std::endl;

    // nearestNeighbor("");

    return 0;
}
