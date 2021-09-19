//
// Created by oleks on 26.05.2021.
//

#ifndef LAB1_CODEGENERATOR_H
#define LAB1_CODEGENERATOR_H


#include <utility>
#include <stack>

#include "Table.h"
#include "SyntacticalAnalyzer.h"
#include "LexicalAnalyzer.h"

class CodeGenerator
{
private:
   static const int BODY_INDENT = 2;
   static const int DECL_INDENT = 0;

   const Node& root;
   const Table table;
   ostream& output;

   string code;
   string data;

   int indent = DECL_INDENT;

   struct variable
   {
      string name;
      PropertyLocation lex;
      vector<string> attributes;
      string base_type;

      variable(string name, PropertyLocation lex) : name(std::move(name)), lex(lex)
      {}
   };

   vector<variable> variables;

   void printError(const Node& node, string format, ...);

   string makeCode();

   void addLine(const string& line, int newIndent = -1);

   bool collectVariables();
   bool makeMainProcedure();

   string getProgramName();

public:
   CodeGenerator(ostream& stream, const Node& root, const Table& tab) : table(tab), output(stream), root(root)
   {}

   bool startCode();
   void printCode() const;
};


#endif //LAB1_CODEGENERATOR_H
