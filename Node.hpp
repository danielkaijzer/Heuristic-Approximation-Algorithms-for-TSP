#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>

class Node{
    private:
        int val;
        Node* edge1;
        Node* edge2;
        int distance;

    public: 
        Node() = default;

        void setVal(int &x);
        const int getVal();

        void setEdge1(Node* edge);
        const Node* getEdge1();

        void setEdge2(Node* edge);
        const Node* getEdge2();

        void setDistance(Node &a, Node &b);
        const int getDistance(Node &a, Node &b);
};

#endif