//
// Created by oleks on 21.03.2021.
//
#include "LexicalAnalyzer.h"

void LexicalAnalyzer::setNext()
{
   position++;
   char chr;

   prevLines = lines;
   prevCol = col;

   if ((chr = stream->get()) == '\n')
   {
      col = position;
      lines++;
   }

   current = chr;
   type = tab->getChar(chr);
}

void LexicalAnalyzer::setBuffer()
{
   buffer.push_back(current);
}

void LexicalAnalyzer::reset()
{
   buffer.clear();
   type = CharType::ERR;
   lexemes.clear();

   current = 0;
   position = 0;
   col = 0;
   lines = 0;
}


int LexicalAnalyzer::makeId()
{
   return tab->makeId(buffer);
}

int LexicalAnalyzer::makeDm()
{
   return tab->makeDm(current);
}


PropertyLocation LexicalAnalyzer::getPosInfo(int id)
{
   return PropertyLocation{id,
                           prevLines + 1,
                           position - prevCol - (int) buffer.size()};
}

void LexicalAnalyzer::printError(string format, ...)
{
   char buff[300];

   va_list argp;
   va_start(argp, format);
   vsnprintf(buff, sizeof(buff), format.c_str(), argp);
   va_end(argp);

   *output << "LexicalAnalyzer: Error ( line: " << prevLines + 1 << ", column " << position - prevCol << " ): ";
   *output << (const char*) buff << endl;
}

LexicalAnalyzer::LexicalAnalyzer(ostream& output, istream& stream, Table& table)
{
   this->stream = &stream;
   this->output = &output;
   this->tab = &table;
   reset();
}

bool LexicalAnalyzer::start()
{
   reset();
   setNext();
   bool exit = false;
   bool abort = false;
   while (!exit)
   {
      buffer.clear();
      switch (type)
      {
         case CharType::WS:
            while (type == CharType::WS)
               setNext();
            break;
         case CharType::LET:
            while (type == CharType::DIG || type == CharType::LET)
            {
               setBuffer();
               setNext();
            }
            lexemes.push_back(getPosInfo(makeId()));
            break;
         case CharType::DM:
            lexemes.push_back(getPosInfo(makeDm()));
            setNext();
            break;
         case CharType::COM:
         {
            setNext();
            int startLine = prevLines;
            int startPosition = position - 1;
            int startCol = prevCol;
            if (current == '*')
            {
               bool comment = true;
               setNext();
               while (comment)
               {
                  while (current != '*')
                  {
                     if (type == CharType::Eof)
                     {
                        prevLines = startLine;
                        position = startPosition;
                        prevCol = startCol;
                        printError("File ended before comment was closed", current);
                        return false;
                     }
                     setNext();
                  }
                  setNext();
                  if (current == ')')
                     comment = false;
               }
               setNext();
            }
            else
            {
               lexemes.push_back(getPosInfo('('));
               setNext();
            }
            break;
         }
         case CharType::Eof:
            exit = true;
            break;
         default:
            printError("Illegal character `%c` detected", current);
            setNext();
            abort = true;
            break;
      }
   }

//    if(!abort) {
//        for (auto iter : lexemes) {
//            iter.print(output, tab);
//        }
//    }
   return !abort;
}

void LexicalAnalyzer::printNodes()
{
   for (auto iter : lexemes)
   {
      iter.print(output, tab);
   }

}

std::string idTypeToString(IdType type)
{
   switch (type)
   {
      case IdType::DM:
         return "DELIMITER";
      case IdType::Keyword:
         return "KEYWORD";
      case IdType::Id:
         return "IDENTIFIER";
      default:
         return nullptr;
   }
}

