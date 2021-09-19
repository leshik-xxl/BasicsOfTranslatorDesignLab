#include "Table.h"
#include "LexicalAnalyzer.h"
#include "SyntacticalAnalyzer.h"

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "No arguments passed" << endl;
        exit(1);
    }
//    for (int i = 1; i <= 10; i++) {
        string filename = argv[1];
//        if (i > 9)
//            filename.append("\\test");
//        else
//            filename.append("\\test0");
//        filename.append(to_string(i));
        cout << filename << endl;
        string directory = filename;
        string directoryLex = filename;

        filename.append("\\input.sig");
        directory.append("\\generated.txt");
        //directoryLex.append("\\generatedLex.txt");
        //directory.append("\\expected.txt");

        ifstream f(filename);
        ofstream out(directory);
        //ofstream outLex(directoryLex);

        if (f.is_open()) {
            Table table;
            LexicalAnalyzer lexicalAnalyzer(out, f, table);

            if (lexicalAnalyzer.start()) {
                //lexicalAnalyzer.printNodes();
                cout << "LexicalAnalyzer has been passed" << endl;
                SyntacticalAnalyzer syntacticalAnalyzer(out, lexicalAnalyzer.lexemes, table);
                if (syntacticalAnalyzer.startSyntacticalAnalyzer()) {
                    syntacticalAnalyzer.printNodes();
                    cout << "SyntacticalAnalyzer has been passed" << endl;
                } else cout << "SyntacticalAnalyzer has been failed" << endl;
            } else {
                cout << "LexicalAnalyzer has been failed" << endl;
            }
        } else {
            cout << "Unable to open file " << (const char *) argv[1];
        }

        f.close();
        out.close();
        //outLex.close();
    //}
    return 0;
}
