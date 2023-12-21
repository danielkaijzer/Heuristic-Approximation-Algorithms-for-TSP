/*
1) Calculate weights of all edges 
2) Sort all edges by weight
3) Repeatedly add minimum weight edges that won't make cycle
*/

#ifndef GREEDY_TSP_HPP
#define GREEDY_TSP_HPP

#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <chrono>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <utility>
#include <queue>
#include <unordered_map>
#include <unordered_set>


class Node {
public:
    int id; // unique identifier for city
    double x, y; // coordinates
    Node* neighbor1;
    Node* neighbor2;
    int neighbor_count;

    Node() = default;

    // parameterized constructor
    Node(int _id, double _x, double _y) : id(_id), x(_x), y(_y), neighbor1(nullptr), neighbor2(nullptr), neighbor_count(0) {}

    // used to calculate Euclidian distance (root(x^2 + y^2))
    // between this node and another node
    double distance(const Node& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::round(std::sqrt(dx * dx + dy * dy)); // euclidean distance
    }

    void addNeighbor(Node* &other){
        if(neighbor1 == nullptr){
            neighbor1 = other;
            neighbor_count++;
        }
        else if(neighbor2 == nullptr){
            neighbor2 = other;
            neighbor_count++;
        }
        else{
            // throw exception, can't add 3rd neighbor
            throw std::runtime_error("Can't add 3rd neighbor");
        }
    }

    void removeNeighbor(Node* &other){
        if(neighbor1 == other){
            neighbor1 = nullptr;
        }
        else if(neighbor2 == other){
            neighbor2 = nullptr;
        }
        else{
            // throw exception, can't add 3rd neighbor
            std::cout << "Error: Tried to remove neighbor that isn't there.\n";
            throw std::runtime_error("Error: Tried to remove neighbor that isn't there.");
        }
    }

    int neighborCount(){
        return neighbor_count;
    }
};

class Edge {
    public:
    // unordered pair of nodes
    std::pair<Node*, Node*> edge;
    int edge_weight;

    Edge(Node* _n1, Node* _n2): edge(std::make_pair(_n1,_n2)), edge_weight(_n1->distance(*_n2)){}

    bool operator<(const Edge& other) const {
        return edge_weight > other.edge_weight;
    }

};


class GreedyTSP {
private:
    std::priority_queue<Edge> edges;
    // std::list<Node> tour;
    double totalDistance;
    int tour_edge_count;
    std::unordered_map<int, Node*> table;
    


public:
    GreedyTSP() = default;

    // Constructor
    GreedyTSP(const std::vector<Node>& nodes){
        // add nodes to direct address table
        for (auto n : nodes){
            Node * newNode = new Node(n.id,n.x,n.y);
            table.insert(std::make_pair(n.id,newNode));
            // std::cout<< table.at(n.id).x;
        }
        totalDistance = 0.0;
        tour_edge_count = table.size();

    }

    ~GreedyTSP(){
        for (auto& pair : table){
            delete pair.second;
            pair.second = nullptr;
        }
    }

    // print outputs (ids visited in order, distance and duration)
    void printResult(long long duration) const {

        // print tour from node 1 back to node 1
        Node *tmp = table.at(1);
        Node *start = tmp;  // Store the starting node
        std::unordered_set<int> visited;

        do{
            std::cout << tmp->id << " ";
            visited.insert(tmp->id);
            // std::cout << visited.count(tmp->neighbor1->id) << " ";
            if (visited.count(tmp->neighbor1->id) < 1){
                tmp = tmp->neighbor1;
            }
            else if(visited.count(tmp->neighbor2->id) < 1) {
                tmp = tmp->neighbor2;
            }
            else{
                // std::cout << "CHECK\n";
                break;
            }
        }while(visited.size() != table.size());

        std::cout << start->id;


        std::cout << "\nTotal Distance: " << totalDistance << "\n";
        std::cout << "Time in ms: " << duration << "\n";
    }

    bool hasCycleUtil(Node* current, Node* parent, std::unordered_set<Node*>& visited, Node* target) {
        if (visited.count(current)) {
            return true;  // Cycle detected
        }

        visited.insert(current);

        if (current == target) {
            return false;  // Target node reached without a cycle
        }

        // Recursively explore neighbors:
        if (current->neighbor1 != nullptr && current->neighbor1 != parent) {
            if (hasCycleUtil(current->neighbor1, current, visited, target)) {
                return true;
            }
        }

        if (current->neighbor2 != nullptr && current->neighbor2 != parent) {
            if (hasCycleUtil(current->neighbor2, current, visited, target)) {
                return true;
            }
        }

        return false;  // No cycle found from this node
    }

    bool hasCycle(Node* np1, Node* np2) {
        std::unordered_set<Node*> visited;
        return hasCycleUtil(np1, nullptr, visited, np2) || hasCycleUtil(np2, nullptr, visited, np1);
    }


    // function checks if an edge can be added between two nodes
    // based on two conditions:
    // 1) if either node already has 2 edges we can't add an edge
    // 2) if adding an edge between these nodes creates a cycle, we can't add edge
    bool checkValidEdge(Node* np1, Node* np2) {
        if (np1 == nullptr || np2 == nullptr) {
            std::cout << "ERROR\n";
            throw std::runtime_error("Error");
        }

        if (np1->neighbor_count == 2 || np2->neighbor_count == 2) {
            return false;
        }

        // Check if np1 already has np2 as a neighbor
        if (np1->neighbor1 == np2 || np1->neighbor2 == np2) {
            return false;
        }

        // Check if np2 already has np1 as a neighbor
        if (np2->neighbor1 == np1 || np2->neighbor2 == np1) {
            return false;
        }

        // Check if adding nodes as neighbors creates a cycle
        if (hasCycle(np1, np2)) {
        return false;  // Edge would create a cycle
    }

        return true;
    }


    

    // main function
    void run() {
        auto start = std::chrono::high_resolution_clock::now();

        // Create an ordered list of all edges sorted by weight
         // Create edges between all unvisited nodes:
        for (auto it1 = table.begin(); it1 != table.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != table.end(); ++it2) {
                // Create an edge between the two nodes:
                Edge edge(it1->second, it2->second);  // Note: Using Node objects directly in Edge constructor
                edges.push(edge);
            }
        }
        int current_edges = 0;

        // add minimum edges until you reach maximum number of edges possible |E| = |V| for tour
        while (current_edges < tour_edge_count && !edges.empty()) {
            Node* n1 = edges.top().edge.first;
            Node* n2 = edges.top().edge.second;

            if (checkValidEdge(n1, n2)) {
                n1->addNeighbor(n2);
                n2->addNeighbor(n1);
                current_edges++;
                std::cout << "Edge from " << n1->id << " to " << n2->id << " of weight " << edges.top().edge_weight << "\n";
                totalDistance += edges.top().edge_weight;
            }

            edges.pop();
        }

        // finish tour by connecting last two nodes without 2 neighbors
        Node* tourStart = nullptr;
        Node* tourEnd = nullptr;
        for (auto it : table){
            if (it.second->neighbor_count == 1){
                // std::cout << it.second->id << std::endl;
                if(tourStart == nullptr){
                    tourStart = it.second;
                    // std::cout << "HELLO" << tourStart->id << std::endl;
                }
                else if (tourEnd == nullptr){
                    tourEnd = it.second;
                    break;
                }
            }
        }
        tourStart->addNeighbor(tourEnd);
        tourEnd->addNeighbor(tourStart);
        totalDistance += tourStart->distance(*tourEnd);
        current_edges++;

        // std::cout << current_edges << std::endl;

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


void greedyTSP(std::string filename){
    std::vector<Node> nodes = nodesFromInput(filename);

    if (nodes.empty()){
        std::runtime_error("Didn't read data correctly\n");
        return;
    }

    GreedyTSP nn(nodes);
    nn.run();
    return;
}

#endif