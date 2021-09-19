#include "Table.h"
#include "LexicalAnalyzer.h"

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "No arguments passed" << endl;
        exit(1);
    }

    string filename = argv[1];
    string directory = argv[1];

    filename.append("\\input.sig");
    directory.append("\\generated.txt");
    //directory.append("\\expected.txt");

    ifstream f(filename);
    ofstream out(directory);

    if (f.is_open()) {
        Table table;
        LexicalAnalyzer lexicalAnalyzer(out, f, table);

        if (!lexicalAnalyzer.start()) {
            cout << "LexicalAnalyzer has been failed" << endl;
        }
    } else {
        cout << "Unable to open file " << (const char *) argv[1];
    }

    f.close();
    out.close();

    return 0;
}
