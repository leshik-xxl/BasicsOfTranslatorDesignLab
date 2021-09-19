//
// Created by oleks on 21.03.2021.
//

#ifndef LAB1_LEXICALANALYZER_H
#define LAB1_LEXICALANALYZER_H

#include "CharType.h"
#include "IdType.h"
#include "PropertyLocation.h"
#include "Table.h"

#include <istream>
#include <ostream>
#include <cstdarg>

using namespace std;

class LexicalAnalyzer {
private:
    istream *stream;
    ostream *output;
    Table *tab;
    string buffer;
    CharType type;
    char current;
    int position;
    int col;
    int lines;

    int prevLines;
    int prevCol;

    void setNext();

    void setBuffer();

    void reset();

    int makeId();

    int makeDm();
    int makeTask();

    PropertyLocation getPropLoc(int id = 0);

    void printError(string format, ...);

public:
    vector<PropertyLocation> lexemes;

    explicit LexicalAnalyzer(ostream &output, istream &stream, Table &table);

    bool start();
};


#endif //LAB1_LEXICALANALYZER_H
