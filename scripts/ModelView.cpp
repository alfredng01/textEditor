#include "ModelView.h"
#include "TextEditor.h"

using namespace std;

ModelView :: ModelView(vector<string> text, ECTextViewImp &windo, int rows, int columns) : window(windo), height(rows), width(columns), paragraphs(text)
{
}

void ModelView :: Compose(vector<string> text)
{
    paragraphs = text;
    wordWraps.clear();

    for (auto paragraph : paragraphs)
    {
        if (!(paragraph.length() < width - 1))
        {
            vector<string> wrapParagraph = this->createParagraph(paragraph);
            for (auto line : wrapParagraph)
            {
                wordWraps.push_back(line);
            }
        }

        else
        {
            wordWraps.push_back(paragraph);
        }
    }

    window.InitRows();
    for (auto line : wordWraps)
    {
        window.AddRow(line);
    }
}

vector<string> ModelView :: GetViewsText()
{
    return wordWraps;
}

void ModelView :: MoveCursor(int length, TextEditor &edi)
{
    TextEditor &edit = edi;
    int x = 0, y = 0, counter = 0;

    while (length > (int)wordWraps[counter].size())
    {
        length -= (int)wordWraps[counter].size();
        counter ++;
        y++;
    }

    x = length;
    
    edit.SetCursor(x, y);
}

vector<string> ModelView :: createParagraph(string paragraphIn)
{
    vector<string> splitWord = this->splitWords(paragraphIn);
    vector<string> created;
    created.push_back("");

    int currentLength = 0, currentLine = 0;

    for (int i = 0; i < splitWord.size(); i++)
    {
        string word = splitWord[i];
        if ((currentLength + word.size()) >= width)
        {
            created.push_back("");
            currentLength = 0;
            currentLine += 1;
        }

        created[currentLine] += word;
        currentLength += word.size();
    }

    return created;
}

vector<string> ModelView :: splitWords(string paragraphIn)
{
    vector<string> splitWord;
    int counter = 0;

    while (true)
    {
        int index = (int)paragraphIn.find(' ', counter);

        if (index == -1)
        {
            splitWord.push_back(paragraphIn.substr(counter));
            return splitWord;
        }

        string word = paragraphIn.substr(counter, index - counter);
        char nextChar = paragraphIn.substr(index, 1)[0];


        if (nextChar == ' ' || nextChar == '\t')
        {
            splitWord.push_back(word);
            string temp(1, nextChar);
            splitWord.push_back(temp);
        }

        counter = index + 1;
    }
}