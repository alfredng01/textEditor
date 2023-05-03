#include "TextCommand.h"
#include "TextEditor.h"

#include <vector>
#include <string>

using namespace std;

// command hist

TextCommandHistory :: TextCommandHistory() {};
TextCommandHistory :: ~TextCommandHistory() {};

void TextCommandHistory :: ExecuteCom(TextCommand *cmd)
{
    while (redoStack.size() != 0)
    {
        redoStack.pop_back();
    }

    cmd->Execute();
    undoStack.push_back(cmd);

}

bool TextCommandHistory :: Undo()
{
    if (undoStack.size() == 0)
    {
        return false;
    }

    TextCommand *cmd = undoStack.back();
    cmd->UnExecute();
    undoStack.pop_back();
    redoStack.push_back(cmd);
    return true;
}

bool TextCommandHistory :: Redo()
{
    if (redoStack.size() == 0)
    {
        return false;
    }

    TextCommand *cmd = redoStack.back();
    cmd->Execute();
    redoStack.pop_back();
    undoStack.push_back(cmd);
    return true;
}

// Enter command

EnterCommand :: EnterCommand(TextEditor &edi, int x, int y) : X(x), Y(y), editor(edi) {};

EnterCommand :: ~EnterCommand() 
{ 
    delete this; 
};

void EnterCommand :: UnExecute()
{
    editor.RemoveRow(realY + 1);
    editor.SetCursor(X, Y);
}

void EnterCommand :: Execute()
{
    int rowLengthE = X;

    for (int i = 0; i < Y; i++)
    {
        rowLengthE += (int)editor.GetViewsText()[i].size();
    }

    realY = 0;
    int temp = 0;

    if (X == 0)
    {
        while ((temp + editor.GetText()[realY].size() <= rowLengthE) && realY < Y)
        {
            temp += (int)editor.GetText()[realY].size();
            realY++;
        }
    }

    else
    {
        while ((temp + editor.GetText()[realY].size() < rowLengthE) && realY < Y)
        {
            temp += (int)editor.GetText()[realY].size();
            realY++;
        }
    }

    if (realX == (editor.GetText()[realY].size() - 1))
    {
        row = " ";
        rowLength = 0;
    }

    else
    {
        row = editor.GetText()[realY].substr(realX, (int)editor.GetText()[realY].size());
        rowLength = (int)row.size();
    }

    editor.InsertRowSpecific(rowLength, realY + 1, row);
    editor.Compose();

    if (realX == editor.GetText()[realY].size())
    {
        editor.SetCursor(0, Y + 1);
    }

    else
    {
        editor.MoveCursor(rowLengthE);
    }
}

// insert command

InsertionCommand :: InsertionCommand(TextEditor &edi, int x, int y, int keyPressed) : X(x), Y(y), editor(edi), keyVal(keyPressed) {};

InsertionCommand :: ~InsertionCommand() 
{ 
    delete this; 
};

void InsertionCommand :: UnExecute()
{
    editor.SetCursor(X, Y);
    editor.RemoveText(realX, realY);
}

void InsertionCommand :: Execute()
{
    int rowLengthE = X;

    for (int i = 0; i < Y; i++)
    {
        rowLengthE += (int)editor.GetViewsText()[i].size();
    }

    int temp = 0;
    realY = 0;

    if (X != 0)
    {
        while ((temp + editor.GetViewsText()[realY].size()) <= rowLengthE && realY < Y)
        {
            temp += (int)editor.GetViewsText()[realY].size();
            realY++;
        }

        realX = rowLengthE - temp;
    }

    else
    {
        while ((temp + editor.GetViewsText()[realY].size()) < rowLengthE && realY < Y)
        {
            temp += (int)editor.GetViewsText()[realY].size();
            realY++;
        }

        realX = rowLengthE - temp;
    }

    editor.InsertText(realX, realY, static_cast<char>(keyVal));
    editor.Compose();
    editor.MoveCursor(rowLengthE + 1);
}

// backspace

FullRowBackSpaceCommand :: FullRowBackSpaceCommand(TextEditor &edi, int x, int y) : Y(y), editor(edi) {};

FullRowBackSpaceCommand :: ~FullRowBackSpaceCommand() 
{ 
    delete this; 
};

void FullRowBackSpaceCommand :: UnExecute()
{
    editor.InsertRowSpecific(rowLength, realY, rowD);
}

void FullRowBackSpaceCommand :: Execute()
{
    int rowLengthE = X;

    for (int i = 0; i < Y; i++)
    {
        rowLengthE += (int)editor.GetViewsText()[i].size();
    }

    int temp = 0;
    realY = 0;

    while ((temp + editor.GetViewsText()[realY].size()) <= rowLengthE && realY < editor.GetText().size())
    {
        temp += (int)editor.GetViewsText()[realY].size();
        realY++;
    }


    try
    {
        rowD = editor.GetText()[realY];
    }
    catch(const std::exception& e)
    {
        rowD = " ";
    }

    rowLength = (int)rowD.size();

    editor.RemoveRow(realY);
    editor.Compose();
    editor.MoveCursor(rowLengthE);
}

// remove

RemoveCommand :: RemoveCommand(TextEditor &edi, int x, int y) : X(x), Y(y), editor(edi) {};

RemoveCommand :: ~RemoveCommand() 
{ 
    delete this; 
};

void RemoveCommand :: UnExecute()
{
    editor.InsertText(realX - 1, realY, deletedCharacter);
}

void RemoveCommand :: Execute()
{
    int rowLengthE = X;
    for (int i = 0; i < Y; i++)
    {
        rowLengthE += (int)editor.GetViewsText()[i].size();
    }
    
    realY = 0;
    int temp = 0;

    if (X != 0) 
    {
        while (temp + editor.GetText()[realY].size() < rowLengthE && realY < Y) {
            temp += (int)editor.GetText()[realY].size();
            realY++;
        }
        realX = rowLengthE - temp;
    } 
    else 
    {
        while (temp + editor.GetText()[realY].size() <= rowLengthE && realY < Y) {
            temp += (int)editor.GetText()[realY].size();
            realY++;
        }
        realX = rowLengthE - temp;
    }

    deletedCharacter = editor.GetText()[realY][realX - 1];

    editor.RemoveText(realX - 1, realY);
    editor.Compose();

    if (realX != (editor.GetText()[realY].size() + 1))
    {
        editor.MoveCursor(rowLengthE - 1);
    }

    else
    {
        editor.SetCursor(X - 1, Y);
    }
}
