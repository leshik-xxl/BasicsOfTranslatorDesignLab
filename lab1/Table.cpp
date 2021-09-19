//
// Created by oleks on 21.03.2021.
//
#include "Table.h"

using namespace std;

void Table::setupChars() {
    for (int i = 0; i < 255; i++)
        chars[i] = CharType::ERR;

    for (int i = 8; i < 15; i++)
        chars[i] = CharType::WS; //tab \r \t etc.

    for (int i = 48; i < 58; i++)
        chars[i] = CharType::DIG; // 0 1 2 3 4 ...


    for (int i = 65; i < 91; i++)
        chars[i] = CharType::LET; // A B C D ...

    chars[32] = CharType::WS;  // space
    chars[40] = CharType::COM;  // (
    chars[41] = CharType::DM; // )
    chars[58] = CharType::DM; // :
    chars[46] = CharType::DM; // .
    chars[59] = CharType::DM; // ;
    chars[44] = CharType::DM; // ,
    chars[37] = CharType::TASK; //%

    chars[EOF] = CharType::Eof;
}

void Table::setupKeywords() {
    keywords.emplace_back("PROGRAM");
    keywords.emplace_back("PROCEDURE");
    keywords.emplace_back("BEGIN");
    keywords.emplace_back("END");
    keywords.emplace_back("SIGNAL");
    keywords.emplace_back("COMPLEX");
    keywords.emplace_back("INTEGER");
    keywords.emplace_back("FLOAT");
    keywords.emplace_back("BLOCKFLOAT");
    keywords.emplace_back("EXT");
}


Table::Table() {
    setupChars();
    setupKeywords();
}


int Table::makeId(string &buffer) {
    auto iter = find(keywords.begin(), keywords.end(), buffer);
    if (iter != keywords.end()) {
        return (int) distance(keywords.begin(), iter) + offsetKeyword;
    } else {
        auto iter = find(ids.begin(), ids.end(), buffer);
        if (iter != ids.end()) {
            return (int) distance(ids.begin(), iter) + offsetId;
        } else {
            ids.push_back(buffer);
            return (int) ids.size() - 1 + offsetId;
        }

    }
}

int Table::makeTask(string &buffer){
    auto iter = find(task.begin(), task.end(), buffer);
    if (iter != task.end()) {
        return (int) distance(task.begin(), iter) + offsetTask;
    } else {
        task.push_back(buffer);
        return (int) task.size() - 1 + offsetTask;
    }
}

int Table::makeDm(char chr) {
    return chr;
}


CharType Table::getChar(char chr) const {
    return chars.at(chr);
}

string Table::getKeyword(int id) const {
    return keywords.at(id - offsetKeyword);
}

string Table::getId(int id) const {
    return ids.at(id - offsetId);
}

string Table::getTask(int id) const {
    return task.at(id - offsetTask);
}

IdType Table::classifyIndex(int id) const {
    if (id > offsetChar && id < offsetDM) {
        if (chars.at(id) == CharType::DM || chars.at(id) == CharType::COM)
            return IdType::DM;
        else
            abort();
    } else if (id < offsetId) return IdType::Keyword;
    else if (id < offsetTask) return IdType::Id;
    else return IdType::TASK;
}
