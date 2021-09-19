//
// Created by oleks on 30.04.2021.
//

#include "Node.h"

bool Node::isEmpty() const{
    return empty;
}

Node::Node(const string& name, const PropertyLocation& position){
    empty = false;
    for (auto & c: name) Name += (char)(c);
    lexeme = position;
    nodes = vector<Node>();
    terminal = false;
}

void Node::print(const Table& tab, ostream& stream){
    print(tab, stream, 0);
}

void Node::print(const Table& tab, ostream &stream, int offset){
    for(int i = 0; i < offset; i++)
        stream << '.';
    if(empty) {
        stream << "<" << Name << ">" << endl;
        for(int i = 0; i < offset + OFFSET; i++)
            stream << '.';
        stream << "<empty>" << endl;
    } else {
        if(terminal) {
            string data;
            switch(tab.classifyIndex(lexeme.id)) {
                case IdType::DM:
                    data = to_string(lexeme.id) + " " + string(1, (char)lexeme.id);
                    break;
                case IdType::Keyword:
                    data = to_string(lexeme.id) + " " + tab.getKeyword(lexeme.id);
                    break;
                case IdType::Id:
                    data = to_string(lexeme.id) + " " + tab.getId(lexeme.id);
                    break;
                default:
                    data = "INVALID";
            }
            stream << data << endl;
        }
        else stream << "<" << Name << ">" << endl;
    }
    for(auto node : nodes) {
        node.print(tab, stream, offset + OFFSET);
    }
}

void Node::markEmpty(){
    empty = true;
}