# C++ Based Text Editor
A Vi Style Text Editor built in C++, based on my professors code.

#### Features
- File I/O
- Command/Normal Mode
- Text Coloring
- Undo/Redo

#### TODO
- Finish Word Wrapping
- Fix Enter(Prepends insteads of Appends)

#### To Run:

1) Run `make`
2) Run `make clean` in order to get rid of all the object files
3) Run `./editor <filename>` and start typing!

#### Alternative Option:
1) If you are on a Unix based system, there is a bash script that will compile the program for you. Simply run `./helper.sh` and this will compile and clean the object files for you.


#### Note:
- As this uses `termios.h`, you need to run this on a Unix based system.
  - If you are on windows, you can use wsl to compile and run this program.