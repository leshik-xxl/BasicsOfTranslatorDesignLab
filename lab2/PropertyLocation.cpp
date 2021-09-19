//
// Created by oleks on 21.03.2021.
//

#include"PropertyLocation.h"


PropertyLocation::PropertyLocation(int id, int line, int column)
{
   this->id = id;
   this->line = line;
   this->column = column;
}

void PropertyLocation::print(ostream* stream, Table* tab) const
{
   string val;
   switch (tab->classifyIndex(id))
   {
      case IdType::DM:
         val = (char) id;
         break;
      case IdType::Keyword:
         val = tab->getKeyword(id);
         break;
      case IdType::Id:
         val = tab->getId(id);
         break;
      default:
         val = "ERR";
   }
   *stream << setw(3) << line << " | "
           << setw(3) << column << " | "
           << setw(7) << id << " | "
           << val << endl;
}

