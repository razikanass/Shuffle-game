#include <iostream>
#include <string>
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include <random>


//TODO : grab from file or word generation algorithm
std::vector<std::string> words = { "pack","steriotyped","present","loaf","kindly",
										"skate","festive","irritating","spare","gaudy","rail","selfish",
										"rare","familiar","wreck","scattered","root","fill","nonstop",
										"crush","thumb","worry","empty","fool","reflective",
										"jail","wrap","shade","strap","wiry","describe","drag",
										"squirrel","grip","dazzling","tax","subsequent","parched",
										"rock","depressed","full","dust","likeable","alluring" };

static const int YELLOW = 14;
static const int STRONG_BLUE = 1;
static const int RED = 12;

static const HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);

struct Word {
	std::string word;
	float x, y;
	int cursor;
};

struct Game {
	std::vector<std::string> words;
	std::string keyboard;
	std::string greetting;
	std::string footer;

	Word currentWord;

	int score;
	int lives;
	float speed;
	bool gameover;

	std::string generateWord();
	void reInitGameState();
};

Game game = { words ,"AZERTYUIOPQSDFGHJKLMWXCVBN" , "if ready press any key !" ,"score = ",{} ,0 ,3 ,0.25f ,false };

int generateRandomInt(int min, int max) {
	std::random_device random_device;
	std::mt19937 engine{ random_device() };
	std::uniform_int_distribution<int> dist(min, max);
	return dist(engine);
}

std::string Game::generateWord() {
	return game.words[generateRandomInt(0, game.words.size() - 1)];
}

void Game::reInitGameState() {
	game.currentWord.word = game.generateWord();
	game.currentWord.x = 1;
	game.currentWord.y = generateRandomInt(0, 20);
	game.currentWord.cursor = 0;
	game.speed += 0.5f;
}

void gotoxy(short x, short y) {
	SetConsoleCursorPosition(H, { x,y });
}

void color(int textColorIndex, int backgroundColorIndex) {
	SetConsoleTextAttribute(H, backgroundColorIndex * 16 + textColorIndex);
}

COORD getDim() {
	COORD coord;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(H, &csbi);
	return { csbi.srWindow.Right - csbi.srWindow.Left + 1 , csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };
}

int main(int argc, char** argv) {
	system("cls");
	color(YELLOW, STRONG_BLUE);
	COORD dim = getDim();
	gotoxy((dim.X / 2) - (game.greetting.size() / 2), dim.Y / 2);
	std::cout << game.greetting << std::endl;
	getc(stdin);

	game.currentWord = { game.generateWord(), 0.0f, float(dim.Y / 2), 0 };

	auto tp1 = std::chrono::system_clock().now();

	while (!game.gameover) {

		auto tp2 = std::chrono::system_clock().now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		system("cls");
		gotoxy(game.currentWord.x, game.currentWord.y);
		color(YELLOW, STRONG_BLUE);
		std::cout << game.currentWord.word << std::endl;
		if (game.lives == 0) {
			game.gameover = true;
		}
		if (game.currentWord.word.length() == game.currentWord.cursor) {
			game.reInitGameState();
			game.score++;
		}
		if (game.currentWord.x + game.currentWord.word.size() >= dim.X) {
			game.reInitGameState();
			game.lives--;
		}
		for (int i = 0; i < 26; i++) {
			if (GetAsyncKeyState((unsigned int)(game.keyboard[i])) & 0x8000) {
				if (game.currentWord.word[game.currentWord.cursor] == tolower(game.keyboard[i])) {
					game.currentWord.cursor++;
				}
				gotoxy(20, 25);
				color(12, 1);
			}
		}

		game.currentWord.x += game.speed * fElapsedTime;
		gotoxy(0, 25);
		color(RED, STRONG_BLUE);
		std::cout << game.footer << game.score;
		gotoxy(50, 25);
		color(RED, STRONG_BLUE);
		std::cout << "lives = " << game.lives;
	}
	system("cls");
	color(14, 1);
	gotoxy((dim.X / 2) - (strlen("!!! Game over !!!") / 2), dim.Y / 2);
	std::cout << "!!! Game over !!!" << std::endl;
	system("pause");
}