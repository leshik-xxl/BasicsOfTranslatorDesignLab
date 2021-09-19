//
// Created by oleks on 30.04.2021.
//

#ifndef LAB1_NODE_H
#define LAB1_NODE_H

#include <vector>
#include <iterator>
#include "PropertyLocation.h"

class Node
{
private:
   static const int OFFSET = 2;

   bool empty;
   void print(const Table& tab, ostream& stream, int offset);
   static Node search(const Node& node, const string& name, bool terminal, bool fail);
   static void extractList(const Node& node, const string& elementName, const string& listName, vector<Node>& v);
public:
   Node() : Name("<empty>"), terminal(false), empty(true)
   {};
   Node(const string& name, const PropertyLocation& position);

   string Name;
   PropertyLocation lexeme;
   vector<Node> nodes;
   bool terminal;

   bool isEmpty() const;
   void print(const Table& tab, ostream& stream);
   void markEmpty();

   string getId(const Table& tab) const;
   string getKw(const Table& tab) const;
   int getConst(const Table& tab) const;

   Node search(const string& name) const;
   Node searchAnyTerminal() const;
   vector<Node> extractList(const string& elementName, const string& listName) const;
};


#endif //LAB1_NODE_H
