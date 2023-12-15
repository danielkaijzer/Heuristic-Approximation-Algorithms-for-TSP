/**
 * @file GreedyTSP.hpp
 * @author Daniel Kaijzer
 * @brief 
 * The basic idea of the greedy TSP algorithm is simple: 
 * calculate the weights of all the edges, 
 * sort the edges by weight, 
 * then repeatedly add the minimum weight edge 
 * that won't violate the definition of a tour.
 * @version 0.1
 * @date 2023-12-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef GREEDY_TSP_HPP
#define GREEDY_TSP_HPP

class Node {
public:
    int id; // unique id for city
    double x, y; // coordinates

    Node(int _id, double _x, double _y) : id(_id), x(_x), y(_y) {}

    double distance(const Node& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::round(std::sqrt(dx * dx + dy * dy)); // euclidean distance
    }
};

class GreedyTSP{

};

#endif