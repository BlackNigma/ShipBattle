#include <iostream>
#include <ctime>
#include <Windows.h>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include "GameBoard.h"
using namespace std;



GameBoard::GameBoard(int width, int height)
	:width{ width }, height{ height }, shipBlocksDestroyed{ 0 }, bot{ false } {
	initEmptyBoard(board);
	initEmptyBoard(dummyBoard);
}

const int TOTAL_SHIP_BLOCKS{ 20 };

string getBoardForDrawing(GameBoard &gBoard);
void drawGameBoard(const string &gameBoardForDrawing, int rimColor, COORD consolePOS);
void drawGameBoards(GameBoard &gameBoard, GameBoard &enemyBoard, int offSet, bool &resetThisTurn);
bool moveHandler(GameBoard &playerBoard, GameBoard &botBoard, bool playersTurn, int &offset);

int main(int argc, char *argv[]) {
	bool playersTurn{ true };
	bool run{ true };
	bool resetThisTurn{ false };

	GameBoard playerBoard(14, 14);
	GameBoard botBoard(14, 14);
	botBoard.setBot(true);

	playerBoard.placeRNGShips();
	botBoard.placeBotRNGShips();

	botBoard.setInitialUnvPostionsForBot(playerBoard.getBoard());
	int offset = 2;
	drawGameBoards(playerBoard, botBoard, offset++, resetThisTurn);
	cout << endl;

	while (run) {
		playersTurn = moveHandler(playerBoard, botBoard, playersTurn, offset);
		drawGameBoards(playerBoard, botBoard, offset++, resetThisTurn);

		if (botBoard.getShipBlocksDestroyedCounter() == TOTAL_SHIP_BLOCKS) {
			run = false;
			cout << "[You have won!]" << endl;
		}
		else if (playerBoard.getShipBlocksDestroyedCounter() == TOTAL_SHIP_BLOCKS) {
			run = false;
			cout << "[You have lost.]" << endl;
		}

		if (offset >= 16) {
			offset = 2;
			resetThisTurn = true;
		}
		else {
			resetThisTurn = false;
		}
	}

	return 0;
}

void drawGameBoards(GameBoard &playerBoard, GameBoard &enemyBoard, int offSet, bool &resetThisTurn) {

	if (resetThisTurn) {
		system("cls");
	}

	string pBoard = getBoardForDrawing(playerBoard);
	string eBoard = getBoardForDrawing(enemyBoard);

	COORD pos = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

	drawGameBoard(pBoard, 2, pos);

	pos.X = 50;
	pos.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	drawGameBoard(eBoard, 4, pos);

	pos.X = 0;
	pos.Y = playerBoard.getHeight() + offSet;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

bool moveHandler(GameBoard &playerBoard, GameBoard &botBoard, bool playersTurn, int &offset) {
	int x, y;
	bool hit{ false };

	if (playersTurn) {
		cout << "It's Your turn, choose strike coordinates(x, y): ";
		bool inputIsValid{ true };
		do {
			if (!inputIsValid) {
				++offset;
				cout << "Bad input, try again: (x, y): ";
			}
			string input;
			getline(cin, input);
			stringstream inputValidator(input);
			if (!(inputValidator >> x >> y && x >= 0 && x < botBoard.getWidth() && y >= 0 && y < botBoard.getHeight())) {
				inputIsValid = false;
			}
			else {
				inputIsValid = true;
			}
		} while (!inputIsValid);

		hit = botBoard.handleEvent(playerBoard.makeMove(x, y, botBoard.getBoard()), playerBoard);
		if (hit) {
			playersTurn = true;
			cout << "Direct hit at: " << x << " " << y << endl;
		}
		else {
			cout << "Miss at: " << x << " " << y << endl;
			playersTurn = false;
		}
	}
	else {
		cout << "Bot's turn: ";
		hit = playerBoard.handleEvent(botBoard.botMakeMove(playerBoard.getBoard()), botBoard);
		if (hit) {
			playersTurn = false;
			cout << "Direct hit" << endl;
		}
		else {
			playersTurn = true;
			cout << "Miss" << endl;
		}
	}

	return playersTurn;
}

void drawGameBoard(const string &gameBoardForDrawing, int rimColor, COORD consolePOS) {
	int j{ 0 };
	COORD originalPos = consolePOS;

	for (size_t i = 0; i < gameBoardForDrawing.size(); ++i) {
		if (gameBoardForDrawing.at(i) == GameBoard::BOARD_SYMBOL_SIDE) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), rimColor);
		}
		else if (gameBoardForDrawing.at(i) == GameBoard::BOARD_SYMBOL_EMPTY) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
		}
		else if (gameBoardForDrawing.at(i) == GameBoard::BOARD_SYMBOL_SHIP_HIT) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
		}
		else if (gameBoardForDrawing.at(i) == GameBoard::BOARD_SYMBOL_SHIP) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
		}
		else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}

		cout << gameBoardForDrawing.at(i);
		if (gameBoardForDrawing.at(i) == '\n') {
			++j;
			consolePOS.Y = j;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), consolePOS);
		}
	}
}

string getBoardForDrawing(GameBoard &gBoard) {

	string gameBoard;
	stringstream ss;
	int position{ 0 };

	while (gBoard.getBoard().at(position) != '\n') {
		if (position == 0) {
			ss << "     ";
		}
		if (position < 10) {
			ss << position << "  ";
		}
		else {
			ss << position << " ";
		}
		++position;
	}
	gameBoard = ss.str();
	gameBoard.push_back('\n');

	position = 0;
	for (size_t i = 0; i < gBoard.getBoard().size(); ++i) {
		if (i == 0) {
			gameBoard += "  ";
		}

		if (i % (gBoard.getWidth() + 1) == 0) {
			gameBoard += to_string(position++);
			if (position <= 10) {
				gameBoard += "  ";
			}
			else {
				gameBoard += " ";
			}
		}

		gameBoard.push_back(gBoard.getBoard().at(i));
		gameBoard += "  ";
	}

	return gameBoard;
}