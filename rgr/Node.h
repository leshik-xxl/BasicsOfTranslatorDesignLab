//
// Created by oleks on 30.04.2021.
//

#ifndef LAB1_NODE_H
#define LAB1_NODE_H
#include <vector>
#include <iterator>
#include "PropertyLocation.h"

class Node {
private:
    static const int OFFSET = 2;

    bool empty;
    void print(const Table& tab, ostream& stream, int offset);
public:
    Node() : Name("<empty>"), terminal(false), empty(true) {};
    Node(const string& name, const PropertyLocation& position);

    string Name;
    PropertyLocation lexeme;
    vector<Node> nodes;
    bool terminal;

    bool isEmpty() const;
    void print(const Table& tab, ostream& stream);
    void markEmpty();
};


#endif //LAB1_NODE_H
