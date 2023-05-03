CC = g++

TARGET = editor

final: main.o ModelView.o ECTextViewImp.o TextCommand.o TextEditor.o
	$(CC) -o $(TARGET) main.o ModelView.o ECTextViewImp.o TextCommand.o TextEditor.o -std=c++11

main.o: main.cpp TextEditor.h
	$(CC) -c main.cpp -std=c++11

ModelView.o: ModelView.cpp ModelView.h
	$(CC) -c ModelView.cpp -std=c++11

ECTextViewImp.o: ECTextViewImp.cpp ECTextViewImp.h
	$(CC) -c ECTextViewImp.cpp -std=c++11

TextCommand.o: TextCommand.cpp TextCommand.h
	$(CC) -c TextCommand.cpp -std=c++11

TextEditor.o: TextEditor.cpp TextEditor.h
	$(CC) -c TextEditor.cpp -std=c++11


clean:
	rm -f *.o