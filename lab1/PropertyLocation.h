//
// Created by oleks on 21.03.2021.
//

#ifndef LAB1_PROPERTYLOCATION_H
#define LAB1_PROPERTYLOCATION_H

#include "Table.h"
#include <ostream>
#include <iomanip>
#include <string>

using namespace std;

class PropertyLocation {
public:
    int id;
    int line;
    int column;

    PropertyLocation(int id, int line, int column);

    void print(ostream *stream, Table *tab) const;
};


#endif //LAB1_PROPERTYLOCATION_H
