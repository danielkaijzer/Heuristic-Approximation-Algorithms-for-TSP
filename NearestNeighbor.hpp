#ifndef NEAREST_NEIGHBOR_HPP
#define NEAREST_NEIGHBOR_HPP

#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <chrono>

class Node {
public:
    int id; // unique id for city
    double x, y; // coordinates

    Node(int _id, double _x, double _y) : id(_id), x(_x), y(_y) {}

    double distance(const Node& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy); // euclidean distance
    }
};


class NearestNeighbor {
private:
    std::list<Node> unvisitedNodes;
    std::list<Node> tour;
    double totalDistance;

public:
   NearestNeighbor(const std::vector<Node>& nodes) : unvisitedNodes(nodes.begin(), nodes.end()), totalDistance(0.0) {}

    // helper function for run()
    std::list<Node>::iterator findNearest(const Node& current)
    {
        auto nearest = unvisitedNodes.begin(); // 
        double minDistance = current.distance(*nearest);

        for (auto it = unvisitedNodes.begin(); it != unvisitedNodes.end(); ++it) {
            double dist = current.distance(*it);
            // if you find closer city, choose that city instead
            if (dist < minDistance) {
                minDistance = dist;
                nearest = it;
            }
        }

        // return closest unvisited city
        return nearest;
    }

    // print outputs
        // id's in order visited
        // total distance
        // time in milliseconds
    void printResult(long long duration) const {
        for (const auto& node : tour) {
            std::cout << node.id << " ";
        }

        std::cout << "\nTotal Distance: " << totalDistance << "\n";
        std::cout << "Time in ms: " << duration << "\n";
    }

    // main function
    void run() {
        auto start = std::chrono::high_resolution_clock::now();

        // Start with node 1
        Node current = unvisitedNodes.front();
        tour.push_back(current);
        unvisitedNodes.pop_front();

        // Visit the nearest neighbor until all nodes are visited
        while (!unvisitedNodes.empty()) {
            auto nearest = findNearest(current);
            totalDistance += current.distance(*nearest);
            tour.push_back(*nearest);
            current = *nearest;
            unvisitedNodes.erase(nearest);
        }

        // Return to node 1 to complete tour/cycle
        totalDistance += current.distance(tour.front());
        tour.push_back(tour.front());

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        printResult(duration);
    }
};

#endif