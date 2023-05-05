#include "TextEditor.h"
#include <algorithm>
#include <string>

// controller

TextDocumentController :: TextDocumentController() {};
TextDocumentController :: ~TextDocumentController() {};

void TextDocumentController :: RemoveRow(int x, int y, TextEditor &edi)
{
    FullRowBackSpaceCommand *rowDeleteCommand = new FullRowBackSpaceCommand(edi, x, y);
    histCommand.ExecuteCom(rowDeleteCommand);
}

void TextDocumentController :: InsertText(int x, int y, int chr, TextEditor &edi)
{
    InsertionCommand *insertCommand = new InsertionCommand(edi, x, y, chr);
    histCommand.ExecuteCom(insertCommand);
}

void TextDocumentController :: InsertRow(int x, int y, TextEditor &edi)
{
    EnterCommand *enterCommand = new EnterCommand(edi, x, y);
    histCommand.ExecuteCom(enterCommand);
}

void TextDocumentController :: RemoveText(int x, int y, TextEditor &edi)
{
    RemoveCommand *removeCommand = new RemoveCommand(edi, x, y);
    histCommand.ExecuteCom(removeCommand);
}

void TextDocumentController :: Undo()
{
    histCommand.Undo();
}

void TextDocumentController :: Redo()
{
    histCommand.Redo();
}

// observer

TextEditor :: TextEditor(string fileName)
{
    filename = fileName;
    numRows = 0;
    window.ClearStatusRows();
    window.AddStatusRow("Command Mode", "Command", false);
    commandMode = true;

    ifstream fileIn(fileName.c_str());
    if (!(fileIn.good()))
    {
        ofstream file(filename);
        this->InsertRow("");
    }

    else
    {
        string temp;
        while (getline(fileIn, temp))
        {
            this->InsertRow(temp);
        }

        fileIn.close();
    }

    // keyword handling

    string keywordFile = "keywords.txt";

    ifstream inFile(keywordFile.c_str());

    if (inFile.good())
    {
        keywordEnabled = true;
        string temp;
        while (getline(inFile, temp))
        {
            keyWords.push_back(temp);
        }

        for (auto x : keyWords)
        {
            x.erase(remove(x.begin(), x.end(), '\n'), x.cend());
        }

        inFile.close();
    }

    this->SetCursor(0, 0);
    this->InsertRow("");
    this->Compose();

    window.Attach(this);
    window.Show();
}

void TextEditor :: Update()
{
    keyPressed = window.GetPressedKey();
    char representation = static_cast<char>(keyPressed);
    if (keyPressed == 17)
    {
        SaveHandler();

        for (int i = 0; i < 25; i++)
        {
            cout << endl;
        }
        window.Quit();
    }

    else if (keyPressed == 25 && commandMode)
    {
        controller.Redo();
    }

    else if (keyPressed == 26 && commandMode)
    {
        controller.Undo();
    }

    else if ((keyPressed == 13 || representation == '\n') && !commandMode)
    {
        EnterHandler();
    }

    else if (representation == ' ' && keywordEnabled && !commandMode)
    {
        KeyHandler(keyPressed);
        ColorKeywords();
    }

    else if (keyPressed == 19 && commandMode)
    {
        SaveHandler();
    }

    else if (keyPressed == 127 && !commandMode)
    {
        BackSpaceHandler();
    }

    else if (keyPressed <= 1003 && keyPressed >= 1000)
    {
        ArrowKeyHandler(keyPressed);
    }

    else if (keyPressed == 27 || keyPressed == 1)
    {
        EscapeHandler();
    }

    else if (representation == 'i' && commandMode)
    {
        NormalModeHandler();
    }

    else if (!commandMode)
    {
        KeyHandler(keyPressed);
    }

    this->Compose();
}

void TextEditor :: NormalModeHandler()
{
    commandMode = !commandMode;
    if (!commandMode)
    {
        window.ClearStatusRows();
        window.AddStatusRow("Insert Mode", "Edit Mode", false);
    }
}

void TextEditor :: EscapeHandler()
{
    commandMode = !commandMode;
    if (commandMode)
    {
        window.ClearStatusRows();
        window.AddStatusRow("Normal Mode", "Command Mode", false);
    }
}

void TextEditor :: InsertRow(string row)
{
    text.insert(text.begin() + numRows, row);
    numRows++;
}

void TextEditor :: InsertRowSpecific(int xIn, int yIn, string row)
{
    text[yIn - 1] = text[yIn - 1].substr(0, (int)text[yIn - 1].size() - xIn);
    text.insert(text.begin() + yIn, row);
    numRows++;
}

void TextEditor :: InsertText(int x, int y, int chr)
{
    text[y].insert(x, 1, chr);
}

void TextEditor :: RemoveRow(int y)
{
    if (text[y].size() != 0)
    {
        text[y - 1] += text[y];
    }
    text.erase(text.begin() + y);
    numRows -= 1;
}

void TextEditor :: RemoveText(int x, int y)
{
    text[y].erase(x, 1);
}

vector<string> TextEditor :: GetViewsText()
{
    return viewText;
}

vector<string> TextEditor :: GetText()
{
    return text;
}

void TextEditor :: SetCursor(int x, int y)
{
    cursorX = x;
    cursorY = y;

    window.SetCursorX(cursorX);
    window.SetCursorY(cursorY);
}

void TextEditor :: Compose()
{
    modView.Compose(text);
    viewText = modView.GetViewsText();
    rowsInView = viewText.size();
}

int TextEditor :: GetColumns()
{
    return windowColumns;
}

void TextEditor :: MoveCursor(int length)
{
    modView.MoveCursor(length, *this);
}

// handlers

void TextEditor :: KeyHandler(int keyPressed)
{
    controller.InsertText(cursorX, cursorY, keyPressed, *this);
}

void TextEditor :: EnterHandler()
{
    controller.InsertRow(cursorX, cursorY, *this);
}

void TextEditor :: BackSpaceHandler()
{
    if (cursorX > 0 || cursorY > 0)
    {
        if (cursorX != 0)
        {
            controller.RemoveText(cursorX, cursorY, *this);
        }

        else if (cursorX == 0)
        {
            int length = 0, realY = 0, sum = 0;
            for (int i = 0; i < cursorY; i++)
            {
                length += (int)GetViewsText()[i].size();
            }

            while ((sum + GetText()[realY].size()) <= length)
            {
                sum += (int)GetText()[realY].size();
                realY++;
            }

            if (length - sum != 0)
            {
                controller.RemoveText(cursorX, cursorY, *this);
            }

            else
            {
                controller.RemoveRow(cursorX, cursorY, *this);
            }
        }
    }
}

void TextEditor :: SaveHandler()
{
    fstream outFile;
    outFile.open(filename, ios::out);
    if (outFile.is_open())
    {
        for (auto line : text)
        {
            outFile << line << endl;
        }
    }

    outFile.close();
}

void TextEditor :: ArrowKeyHandler(int direction)
{
    cursorX = window.GetCursorX();
    cursorY = window.GetCursorY();

    if (direction == 1000)
    {
        if (cursorX > 0)
        {
            cursorX--;
        }

        else if (cursorY > 0 && cursorX == 0)
        {
            cursorY--;
            cursorX = (int)viewText[cursorY].size();
        }
    }

    else if (direction == 1001)
    {
        if (cursorX < (int)viewText[cursorY].size())
        {
            cursorX++;
        }

        else if (cursorX == (int)viewText[cursorY].size() && cursorY < rowsInView - 1)
        {
            cursorX = 0;
            cursorY++;
        }
    }

    else if (direction == 1002)
    {
        if (cursorY > 0)
        {
            if (cursorX < (int)viewText[cursorY - 1].size())
            {
                cursorY--;
            }

            else
            {
                cursorX = (int)viewText[cursorY - 1].size();
                cursorY--;
            }
        }
    }

    else if (direction == 1003)
    {
        if (cursorY < rowsInView)
        {
            if (cursorX < (int)viewText[cursorY + 1].size())
            {
                cursorY++;
            }

            else
            {
                cursorX = (int)viewText[cursorY + 1].size();
                cursorY++;
            }
        }
    }

    window.SetCursorX(cursorX);
    window.SetCursorY(cursorY);
}

vector<size_t> TextEditor :: stringFinder(string strIn, string match)
{
    vector<size_t> results;

    size_t pos = strIn.find(match, 0);
    while(pos != string::npos)  
    {
        results.push_back(pos);
        pos = strIn.find(match,pos+1);
    }

    return results;
}

void TextEditor :: ColorKeywords()
{
    for (int i = 0; i < viewText.size(); i++)
    {
        for (auto word : keyWords)
        {
            vector<size_t> posArray = stringFinder(viewText[i], word);
            for (auto pos : posArray)
            {
                if (viewText[i][pos + word.size()] == ' ' || viewText[i][pos + word.size()] == '\n')
                {
                    window.SetColor(i, pos, pos + word.size() - 1, TEXT_COLOR_BLUE);
                }
            }
        }
    }
}