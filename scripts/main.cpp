#include "TextEditor.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    string filename;

    if (argc == 2)
    {
        filename = argv[1];
        TextEditor edit(filename);
        return 0;
    }
    else
    {
        cout << "Usage: ./editor <filename>" << endl;
        return 1;
    }

    return 0;
}