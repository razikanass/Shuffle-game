#include <iostream>
#include <string>
#include "windows.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <thread>

using namespace std;

class Word{
public:
	string word;
	float x,y;

	Word(string, float, float);
};

Word::Word(string w, float x, float y):word(w),x(x),y(y){}

class Game{
public:
	static const string words[44];
	static const string greetting;
	static const string footer;
	static int score;
	static string generateWord();
};
 
const string Game::greetting = "if ready press any key !";
const string Game::footer = "score = ";
const string Game::words[44] = {"pack","steriotyped","present","loaf","kindly",
								"skate","festive","irritating","spare","gaudy","rail","selfish",
								"rare","familiar","wreck","scattered","root","fill","nonstop",
								"crush","thumb","worry","empty","fool","reflective",
								"jail","wrap","shade","strap","wiry","describe","drag",
								"squirrel","grip","dazzling","tax","subsequent","parched",
								"rock","depressed","full","dust","likeable","alluring"};
int Game::score = 0;

string Game::generateWord(){
	srand(time(NULL));
	return words[rand()%44];
}

void gotoxy(int x, int y) { 
    COORD coord;
    coord.X = x; 
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void color(int t,int f){
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H,f*16+t);
}

COORD getDim(){
	COORD coord;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    coord.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    coord.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;	
    return coord;
}

int main(int argc, char** argv){
	system("cls");
	color(14,1);
	char key;
	string word;
	COORD dim = getDim();
	gotoxy((dim.X/2)-(Game::greetting.size()/2),dim.Y/2);
	cout<<Game::greetting<<endl;
	key = getc(stdin);

	Word genw(Game::generateWord(),0,dim.Y/2);
	
	while(true){
		system("cls");
		gotoxy(genw.x,genw.y);
		color(14,1);
		cout<<genw.word<<endl;
		if(genw.x+genw.word.size() >= dim.X){
			srand(time(NULL));
			genw.word = Game::generateWord();
			genw.x = 1;
			genw.y = rand()%20;
			Game::score++;
		}
		genw.x+=0.5f;
		gotoxy(0,25);
		color(12,1);
		cout<<Game::footer<<Game::score;
	}
}