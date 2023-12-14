#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "NearestNeighbor.hpp"

// Function to trim whitespaces from the beginning and end of a string
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return (first != std::string::npos && last != std::string::npos) ? str.substr(first, last - first + 1) : "";
}

int main() {
    // Read input data from file (replace "wi29.tsp" with the actual file name)
    std::ifstream input("dj38.tsp");

    if (!input.is_open()) {
        std::cerr << "Error opening file.\n";
        return 1;
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
                return 1;
            }

            nodes.emplace_back(id, x, y);
        }
    }

    // for (auto n : nodes) {
    //     std::cout << n.id << std::endl;
    // }

    if (!nodes.empty()) {
        NearestNeighbor nn(nodes);
        nn.run();
    } else {
        std::cout << "Didn't read data correctly\n";
        return -1;
    }

    return 0;
}
