#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include "ECTextViewImp.h"
#include "ModelView.h"
#include "TextCommand.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

class TextDocumentController
{
public:
    TextDocumentController();
    virtual ~TextDocumentController();

    void InsertText(int x, int y, int chr, TextEditor &edi);
    void RemoveText(int x, int y, TextEditor &edi);
    void InsertRow(int x, int y, TextEditor &edi);
    void RemoveRow(int x, int y, TextEditor &edi);
    void Undo();
    void Redo();


private:
    TextCommandHistory histCommand;

};

class TextEditor : public ECObserver
{
public:
    TextEditor(string fileName);
    ~TextEditor() {};

    void Update();
    void EnterHandler();
    void ArrowKeyHandler(int direction);
    void KeyHandler(int key);
    void BackSpaceHandler();
    void SaveHandler();
    void EscapeHandler();
    void NormalModeHandler();

    void InsertText(int x, int y, int chr);
    void RemoveText(int x, int y);
    void RemoveRow(int y);
    void SetCursor(int x, int y);
    void InsertRow(string row);
    void InsertRowSpecific(int x, int y, string row);

    vector<string> GetText();
    vector<string> GetViewsText();
    void Compose();
    void MoveCursor(int length);
    int GetColumns();
    void ColorKeywords();
    vector<size_t> stringFinder(string strIn, string match);

private:
    string filename;
    ECTextViewImp window;
    TextDocumentController controller;
    vector<string> text;
    int cursorX, cursorY, numRows, keyPressed;
    int windowRows = window.GetRowNumInView();
    int windowColumns = window.GetColNumInView();
    ModelView modView = ModelView(text, window, windowRows, windowColumns);
    vector<string> viewText;
    int rowsInView = (int)viewText.size();
    bool commandMode = false;
    vector<string> keyWords;
    bool keywordEnabled = false;

};


#endif