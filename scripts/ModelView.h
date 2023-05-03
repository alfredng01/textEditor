#ifndef MODELVIEW_H
#define MODELVIEW_H

#include "ECTextViewImp.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class TextEditor;

class ModelView
{
public:
    ModelView(vector<string> text, ECTextViewImp &windo, int rows, int columns);
    ~ModelView() {};

    void Compose(vector<string> text);
    vector<string> createParagraph(string paragraphIn);
    vector<string> splitWords(string paragraphIn);
    void MoveCursor(int length, TextEditor &edi);
    vector<string> GetViewsText();

private:
    int height, width;
    vector<string> paragraphs;
    vector<string> wordWraps;
    ECTextViewImp &window;
};


#endif