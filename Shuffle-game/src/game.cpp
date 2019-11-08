#include <iostream>
#include <string>
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

class Word {
public:
	string word;
	float x, y;

	Word(string, float, float);
};

Word::Word(string w, float x, float y) :word(w), x(x), y(y) {}

class Game {
public:
	static const string words[44];
	static const string keyboard;
	static const string greetting;
	static const string footer;
	static int score;
	static int lives;

	static string generateWord();
};

const string Game::greetting = "if ready press any key !";
const string Game::footer = "score = ";
const string Game::words[44] = { "pack","steriotyped","present","loaf","kindly",
"skate","festive","irritating","spare","gaudy","rail","selfish",
"rare","familiar","wreck","scattered","root","fill","nonstop",
"crush","thumb","worry","empty","fool","reflective",
"jail","wrap","shade","strap","wiry","describe","drag",
"squirrel","grip","dazzling","tax","subsequent","parched",
"rock","depressed","full","dust","likeable","alluring" };
const string Game::keyboard = "AZERTYUIOPQSDFGHJKLMWXCVBN";
int Game::score = 0;
int Game::lives = 3;

string Game::generateWord() {
	srand(time(NULL));
	return words[rand() % 44];
}

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void color(int t, int f) {
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, f * 16 + t);
}

COORD getDim() {
	COORD coord;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	coord.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	coord.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	return coord;
}

int main(int argc, char** argv) {
	system("cls");
	color(14, 1);
	char key;
	string word;
	COORD dim = getDim();
	gotoxy((dim.X / 2) - (Game::greetting.size() / 2), dim.Y / 2);
	cout << Game::greetting << endl;
	key = getc(stdin);

	Word genw(Game::generateWord(), 0, dim.Y / 2);

	unsigned int cursor = 0;
	float speed = 0.25f;
	bool gameover = false;

	auto tp1 = chrono::system_clock().now();
	auto tp2 = chrono::system_clock().now();

	while (!gameover) {

		tp2 = chrono::system_clock().now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		system("cls");
		gotoxy(genw.x, genw.y);
		color(14, 1);
		cout << genw.word << endl;
		if (Game::lives == 0) {
			gameover = true;
		}
		if (genw.word.length() == cursor) {
			srand(time(NULL));
			genw.word = Game::generateWord();
			genw.x = 1;
			genw.y = rand() % 20;
			cursor = 0;
			Game::score++;
			speed += 0.5f;
		}
		if (genw.x + genw.word.size() >= dim.X) {
			srand(time(NULL));
			genw.word = Game::generateWord();
			genw.x = 1;
			genw.y = rand() % 20;
			cursor = 0;
			Game::lives--;
		}
		for (int i = 0; i < 26; i++) {
			if (GetAsyncKeyState((unsigned int)(Game::keyboard[i])) & 0x8000) {
				if (genw.word[cursor] == tolower(Game::keyboard[i])) {
					cursor++;
				}
				gotoxy(20, 25);
				color(12, 1);
			}
		}

		genw.x += speed * fElapsedTime;
		gotoxy(0, 25);
		color(12, 1);
		cout << Game::footer << Game::score;
		gotoxy(50, 25);
		color(12, 1);
		cout << "lives = " << Game::lives;
	}
	system("cls");
	color(14, 1);
	gotoxy((dim.X / 2) - (strlen("!!! Game over !!!") / 2), dim.Y / 2);
	cout << "!!! Game over !!!" << endl;
	system("pause");
}