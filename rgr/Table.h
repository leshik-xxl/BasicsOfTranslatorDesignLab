//
// Created by oleks on 21.03.2021.
//

#ifndef LAB1_TABLE_H
#define LAB1_TABLE_H

#include "CharType.h"
#include "IdType.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class Table {
private:
    map<char, CharType> chars;
    vector<string> keywords;
    vector<string> ids;

    const int offsetChar = 0;
    const int offsetDM = 256;
    const int offsetKeyword = 400;
    const int offsetId = 1000;

    void setupChars();
    void setupKeywords();

public:
    Table();
    int makeId(string &buffer);
    int makeDm(char chr);
    CharType getChar(char chr) const;
    string getKeyword(int id) const;
    string getId(int id) const;
    IdType classifyIndex(int id) const;
};

#endif //LAB1_TABLE_H
