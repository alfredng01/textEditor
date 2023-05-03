#ifndef TEXTCOMMAND_H
#define TEXTCOMMAND_H

#include <vector>
#include <string>

using namespace std;

class TextEditor;

class TextCommand
{
public:
    virtual ~TextCommand() {};
    virtual void Execute() = 0;
    virtual void UnExecute() = 0;
};

class RemoveCommand : public TextCommand
{
public:
    RemoveCommand(TextEditor &edi, int x, int y);
    ~RemoveCommand();
    void Execute();
    void UnExecute();


private:
    int X, Y;
    char deletedCharacter;
    TextEditor &editor;
    int realX, realY;
};

class InsertionCommand : public TextCommand
{
public:
    InsertionCommand(TextEditor &edi, int x, int y, int keyPressed);
    ~InsertionCommand();
    void Execute();
    void UnExecute();


private:
    int X, Y;
    TextEditor &editor;
    int realX, realY;
    int keyVal;
};

class EnterCommand : public TextCommand
{
public:
    EnterCommand(TextEditor &edi, int x, int y);
    ~EnterCommand();
    void Execute();
    void UnExecute();


private:
    string row;
    int X, Y, rowLength, realX, realY;
    TextEditor &editor;
};

class FullRowBackSpaceCommand : public TextCommand
{
public:
    FullRowBackSpaceCommand(TextEditor &edi, int x, int y);
    ~FullRowBackSpaceCommand();
    void Execute();
    void UnExecute();


private:
    string rowD;
    int X, Y, rowLength, realY;
    TextEditor &editor;
};


class TextCommandHistory
{
public:
    TextCommandHistory();
    virtual ~TextCommandHistory();

    bool Undo();
    bool Redo();
    void ExecuteCom(TextCommand *com);

private:
    vector<TextCommand *> undoStack;
    vector<TextCommand *> redoStack;

};


#endif