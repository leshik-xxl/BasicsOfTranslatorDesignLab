//
// Created by oleks on 27.04.2021.
//

#ifndef LAB1_SYNTACTICALANALYZER_H
#define LAB1_SYNTACTICALANALYZER_H

#include <vector>
#include <iterator>
#include <cstdarg>
#include <algorithm>
#include <iostream>
#include "PropertyLocation.h"
#include "Node.h"

using namespace std;

class SyntacticalAnalyzer {
private:
    struct read_res {
        bool ok;
        Node data;
        string error;
    };

    const vector<PropertyLocation> lexemes;
    const Table table;
    ostream &output;
    int lex_counter;

    string createError(string format, ...);
    void appendTerminal(Node &n, const string &name, PropertyLocation lex);
    read_res readKeyword(Node &n, const string &keyword);
    read_res readDm(Node &n, char delimiter);
    read_res readIdentifier(Node &n);
    void reset();

#define DECL(name) read_res name##_func();
    DECL(root)
    DECL(signal_program)
    DECL(program)
    DECL(block)

    DECL(statement_list)
    DECL(statement)
    DECL(condition_statement)
    DECL(condition_expression)
    DECL(alternative_part)

    DECL(parametrs_list)
    DECL(declarations_list)
    DECL(declaration)
    DECL(identifiers_list)
    DECL(attributes_list)
    DECL(attribute)
    DECL(procedure_identifier)
    DECL(variable_identifier)
    DECL(identifier)
#undef DECL

public:
    Node rootNode;

    SyntacticalAnalyzer(ostream &output, const vector<PropertyLocation> &lexemes, const Table &table) : output(output),
                                                                                                        lexemes(lexemes),
                                                                                                        table(table) {
        reset();
    }

    bool startSyntacticalAnalyzer();
    void printNodes();
};

#endif //LAB1_SYNTACTICALANALYZER_H
