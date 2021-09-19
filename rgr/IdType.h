//
// Created by oleks on 29.03.2021.
//

#ifndef LAB1_IDTYPE_H
#define LAB1_IDTYPE_H

#include <string>

enum class IdType {
    DM,
    Keyword,
    Id
};

std::string idTypeToString(IdType type);

#endif //LAB1_IDTYPE_H
