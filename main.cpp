#include <iostream>
#include "NearestNeighbor.hpp"
#include "GreedyTSP.hpp"



int main() {

    nearestNeighbor("wi29.tsp"); 
    std::cout << std::endl;

    greedyTSP("wi29.tsp"); 
    std::cout << std::endl;

    nearestNeighbor("dj38.tsp"); 
    std::cout << std::endl;

    greedyTSP("dj38.tsp"); 
    std::cout << std::endl;

    nearestNeighbor("qa194.tsp");
    std::cout << std::endl;

    greedyTSP("qa194.tsp"); 
    std::cout << std::endl;

    nearestNeighbor("zi929.tsp");
    std::cout << std::endl;

    greedyTSP("zi929.tsp"); 
    std::cout << std::endl;

    nearestNeighbor("ja9847.tsp");
    std::cout << std::endl;

    greedyTSP("ja9847.tsp"); 
    std::cout << std::endl;

    return 0;
}
