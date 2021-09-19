#include "Table.h"
#include "LexicalAnalyzer.h"
#include "SyntacticalAnalyzer.h"
#include "CodeGenerator.h"

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

void doFile(const string& fname)
{
   cout << fname << endl;

   string filename = fname;
   string directory = filename;

   filename.append("/input.sig");
   directory.append("/generated.txt");

   ifstream f(filename);
   ofstream out(directory);

   if (f.is_open())
   {
      Table table;
      LexicalAnalyzer lexicalAnalyzer(out, f, table);
      if (lexicalAnalyzer.start())
      {
         // lexicalAnalyzer.printNodes();
         cout << "LexicalAnalyzer has been passed" << endl;
         SyntacticalAnalyzer syntacticalAnalyzer(out, lexicalAnalyzer.lexemes, table);
         if (syntacticalAnalyzer.startSyntacticalAnalyzer())
         {
            // syntacticalAnalyzer.printNodes();
            cout << "SyntacticalAnalyzer has been passed" << endl;
            CodeGenerator codeGenerator(out, syntacticalAnalyzer.rootNode, table);
            if (codeGenerator.startCode())
            {
               codeGenerator.printCode();
               cout << "CodeGenerator has been passed" << endl;
            }
            else
            {
               cout << "CodeGenerator has been failed" << endl;
            }
         }
         else cout << "SyntacticalAnalyzer has been failed" << endl;
      }
      else
      {
         cout << "LexicalAnalyzer has been failed" << endl;
      }
   }
   else
   {
      cout << "Unable to open file " << filename;
   }

   f.close();
   out.close();
}

// #define RUN_ALL_TESTS

int main(int argc, char** argv)
{
   if (argc != 2)
   {
      cout << "No arguments passed" << endl;
      exit(1);
   }

#ifdef RUN_ALL_TESTS
   for (int i = 1; i <= 8; i++)
   {
      string filename = argv[1];
      if (i > 9)
         filename.append("/test");
      else
         filename.append("/test0");
      filename.append(to_string(i));
      doFile(filename);
   }
#else
   doFile((const char*)argv[1]);
#endif

   return 0;
}
