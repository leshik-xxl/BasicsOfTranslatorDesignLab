//
// Created by oleks on 30.04.2021.
//

#include "Node.h"

bool Node::isEmpty() const
{
   return empty;
}

Node::Node(const string& name, const PropertyLocation& position)
{
   empty = false;
   for (auto& c: name) Name += (char) (c);
   lexeme = position;
   nodes = vector<Node>();
   terminal = false;
}

void Node::print(const Table& tab, ostream& stream)
{
   print(tab, stream, 0);
}

void Node::print(const Table& tab, ostream& stream, int offset)
{
   for (int i = 0; i < offset; i++)
      stream << '.';
   if (empty)
   {
      stream << "<" << Name << ">" << endl;
      for (int i = 0; i < offset + OFFSET; i++)
         stream << '.';
      stream << "<empty>" << endl;
   }
   else
   {
      if (terminal)
      {
         string data;
         switch (tab.classifyIndex(lexeme.id))
         {
            case IdType::DM:
               data = to_string(lexeme.id) + " " + string(1, (char) lexeme.id);
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
   for (auto node : nodes)
   {
      node.print(tab, stream, offset + OFFSET);
   }
}

void Node::markEmpty()
{
   empty = true;
}

Node Node::search(const Node& node, const string& name, bool terminal, bool fail)
{
   if (!fail)
   {
      if (!terminal && node.Name == name)
         return node;
      if (terminal && node.terminal)
         return node;
   }

   for (const auto& i : node.nodes)
   {
      Node r = search(i, name, terminal, false);
      if (!r.isEmpty()) return r;
   }
   return Node();
}

Node Node::search(const string& name) const
{
   if (empty) return Node();
   return search(*this, name, false, false);
}

Node Node::searchAnyTerminal() const
{
   if (empty) return Node();
   return search(*this, string(), true, false);
}

void Node::extractList(const Node& node, const string& elementName, const string& listName, vector<Node>& v)
{
   const Node elem = search(node, elementName, false, false);
   const Node tail = search(node, listName, false, true);
   if (!elem.isEmpty()) v.push_back(elem);
   if (!tail.isEmpty()) extractList(tail, elementName, listName, v);
}

vector<Node> Node::extractList(const string& elementName, const string& listName) const
{
   vector<Node> res;
   extractList(*this, elementName, listName, res);
   return res;
}

string Node::getId(const Table& tab) const
{
   return tab.getId(this->search("identifier").lexeme.id);
}

string Node::getKw(const Table& tab) const
{
   return tab.getKeyword(this->search("keywords").lexeme.id);
}

