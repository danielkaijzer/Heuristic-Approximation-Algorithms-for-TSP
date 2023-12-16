/**
 * Nearest neighbor is most efficiently
 * implemented by starting with a collection of unvisited nodes
 * and calculating distances as you go,
 * only calculating distances from a node once you visit it.
 * The collection should have efficient deletion and iteration 
 * - something linked-list based would probably be a good choice.
*/

#ifndef NEAREST_NEIGHBOR_HPP
#define NEAREST_NEIGHBOR_HPP

#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <chrono>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

class Node {
public:
    int id; // unique identifier for city
    double x, y; // coordinates

    // parameterized constructor
    Node(int _id, double _x, double _y) : id(_id), x(_x), y(_y) {}

    // used to calculate Euclidian distance (root(x^2 + y^2))
    // between this node and another node
    double distance(const Node& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::round(std::sqrt(dx * dx + dy * dy)); // euclidean distance
    }
};


class NearestNeighbor {
private:
    std::list<Node> unvisitedNodes;
    std::list<Node> tour;
    double totalDistance;

public:
    NearestNeighbor() = default;

    // Constructor
        // initializes unvisitedNodes list 
            // (so it contains all Nodes from input file)
        // initializes total distance to 0.0
        // doesn't need to initialize tour since that should start empty.
    NearestNeighbor(const std::vector<Node>& nodes) : unvisitedNodes(nodes.begin(), nodes.end()), totalDistance(0.0) {}


    // Checks all nodes in unvisitedNodes for nearest node. 
    // Returns the nearest node as iterator (points to index in list)
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


    // print outputs (ids visited in order, distance and duration)
    void printResult(long long duration) const {
        // prints ids in order visited
        for (const auto& node : tour) {
            std::cout << node.id << " ";
        }

        std::cout << "\nTotal Distance: " << totalDistance << "\n";
        std::cout << "Time in ms: " << duration << "\n";
        }


    // main function
    void run() {
        auto start = std::chrono::high_resolution_clock::now();

        // Start by visiting node 1 (from list of unvisitedNodes)
        Node current = unvisitedNodes.front();
        tour.push_back(current);
        unvisitedNodes.pop_front();

        // Visit the nearest neighbor until all nodes are visited
        while (!unvisitedNodes.empty()) {
            auto nearest = findNearest(current);
            totalDistance += current.distance(*nearest); // update sum
            tour.push_back(*nearest); // add to tour list

            // visit nearest
            current = *nearest;
            unvisitedNodes.erase(nearest); // remove nearest from unvisited
        }

        // Return to node 1 to complete tour/cycle
        totalDistance += current.distance(tour.front()); // update sum
        tour.push_back(tour.front());

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        printResult(duration); // prints ids visited, distance and duration
    }

};

// Function to trim whitespaces from the beginning and end of a string
// helper for nodesFromInput
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return (first != std::string::npos && last != std::string::npos) ? str.substr(first, last - first + 1) : "";
}

// creates a vector of Node objects from input text file
std::vector<Node> nodesFromInput(std::string filename){
    std::ifstream input(filename);

    if (!input.is_open()) {
        std::cerr << "Error opening file.\n";
        return {};
    }

    std::vector<Node> nodes;

    std::string line;
    bool readingNodes = false;

    while (std::getline(input, line)) {
        // Trim whitespaces from the line
        line = trim(line);

        if (line == "NODE_COORD_SECTION") {
            // std::cout << "found ncs\n";
            readingNodes = true;
            continue;
        }

        if (readingNodes && line == "EOF") {
            // std::cout << line << std::endl;
            break;  // Stop reading after reaching "EOF"
        }

        if (readingNodes && !line.empty()) {
            // std::cout << "hello\n";
            // Parse node data
            int id;
            double x, y;
            std::istringstream iss(line);
            iss >> id >> x >> y;

            if (iss.fail()) {
                std::cerr << "Error parsing line: " << line << "\n";
                return {};
            }

            nodes.emplace_back(id, x, y);
        }
    }

    return nodes;
}


void nearestNeighbor(std::string filename){
    std::vector<Node> nodes = nodesFromInput(filename);

    if (nodes.empty()){
        std::runtime_error("Didn't read data correctly\n");
        return;
    }

    NearestNeighbor nn(nodes);
    nn.run();
    return;
}

#endif