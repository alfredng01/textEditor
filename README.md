# C++ Based Text Editor
A Vi Style Text Editor built in C++, based on my professors code.

#### Features
- File I/O
  - Reads in files from command line, will display all contents in the file to edit
- Command/Insert Mode
  - Command Mode for Undo/Redo, Save and Quit
  - Insert Mode for typing
- Text Coloring
  - Will highlight keywords if there is a file called `keywords.txt` located in the same directory as the executable
- Undo/Redo
  - Will Undo/Redo the actions from the previous session
  - A session being the time while you were in insert mode, and then left insert mode

#### TODO
- Finish Word Wrapping
- Fix Enter(Prepends insteads of Appends)

#### To Run:

1) Run `make`
2) Run `make clean` in order to get rid of all the object files
3) Run `./editor <filename>` and start typing!

#### Alternative Option:
1) If you are on a Unix based system, there is a bash script that will compile the program for you. Simply run `./helper.sh` and this will compile and clean the object files for you.
   1) Note that you might need to change the permissions, if so just run `chmod +x helper.sh`


#### Note:
- As this uses `termios.h`, you need to run this on a Unix based system.
  - If you are on windows, you can use wsl to compile and run this program.