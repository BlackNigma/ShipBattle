#pragma once
#include <iostream>
#include <ctime>
#include <Windows.h>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

class GameBoard {
private:
	int width, height;
	int shipBlocksDestroyed;
	string board;
	string dummyBoard;
	vector<int> unavailablePositions;
	queue<int> searchBuffer;

	bool bot;
	void initEmptyBoard(string &board) {
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				if (i == 0 || j == 0 || i == height - 1 || j == width - 1) {
					board += BOARD_SYMBOL_SIDE;
				}
				else {
					board += BOARD_SYMBOL_EMPTY;
				}
			}
			board += "\n";
		}
	};

	bool checkShipPosValidity(int x, int y) {
		bool isValid{ true };
		int i{ y * (width + 1) + x };
		if (x < 0 && x > width && y < 0 && y > height) {
			return false;
		}
		if (board[i] == BOARD_SYMBOL_SHIP || board[i] == BOARD_SYMBOL_SIDE ||
			board[i - 1] == BOARD_SYMBOL_SHIP || board[i - 1] == BOARD_SYMBOL_SIDE ||
			board[i + 1] == BOARD_SYMBOL_SHIP || board[i + 1] == BOARD_SYMBOL_SIDE ||
			board[i - (width + 1)] == BOARD_SYMBOL_SHIP || board[i - (width + 1)] == BOARD_SYMBOL_SIDE ||
			board[i - (width + 1) - 1] == BOARD_SYMBOL_SHIP || board[i - (width + 1) - 1] == BOARD_SYMBOL_SIDE ||
			board[i - (width + 1) + 1] == BOARD_SYMBOL_SHIP || board[i - (width + 1) + 1] == BOARD_SYMBOL_SIDE ||
			board[i + (width + 1)] == BOARD_SYMBOL_SHIP || board[i + (width + 1)] == BOARD_SYMBOL_SIDE ||
			board[i + (width + 1) - 1] == BOARD_SYMBOL_SHIP || board[i + (width + 1) - 1] == BOARD_SYMBOL_SIDE ||
			board[i + (width + 1) + 1] == BOARD_SYMBOL_SHIP || board[i + (width + 1) + 1] == BOARD_SYMBOL_SIDE) {
			isValid = false;
		}

		return isValid;
	};
	bool positionIsAvailable(int pos) {
		for (const auto &position : unavailablePositions) {
			if (position == pos) {
				return false;
			}
		}
		return true;
	};
public:
	static const char BOARD_SYMBOL_EMPTY{ ' ' };
	static const char BOARD_SYMBOL_SIDE{ '*' };
	static const char BOARD_SYMBOL_SHIP{ '#' };
	static const char BOARD_SYMBOL_SHIP_HIT{ 'X' };
	static const char BOARD_SYMBOL_SHIP_MISS{ '^' };

	static const int SHIP_POS_VERTICAL{ 0 };
	static const int SHIP_POS_HORIZONTAL{ 1 };
	static const int TOTAL_SHIP_COUNT{ 10 };

	GameBoard(int width = 15, int height = 30);

	void placeBotRNGShips() {
		srand(time(NULL));
		int shipOrientation{ 0 };
		int shipStartX{ 0 };
		int shipStartY{ 0 };
		int shipSize{ 4 };
		bool generateShips{ true };
		bool shipPlacementSuccessfull{ false };

		while (generateShips) {

			int i{ shipSize };
			while (i != 5) {
				shipOrientation = rand() % 2;
				shipStartX = rand() % (width - 1) + 1;
				shipStartY = rand() % (height - 1) + 1;

				int tempShipStartX = shipStartX + (rand() % 2);
				int tempShipStartY = shipStartY + (rand() % 2);

				for (int j = 0; j < shipSize; ++j) {
					if (shipOrientation == SHIP_POS_VERTICAL) {
						if (checkShipPosValidity(tempShipStartX, tempShipStartY++)) {
							shipPlacementSuccessfull = true;
						}
						else {
							shipPlacementSuccessfull = false;
							break;
						}
					}
					else {
						if (checkShipPosValidity(tempShipStartX++, tempShipStartY)) {
							shipPlacementSuccessfull = true;
						}
						else {
							shipPlacementSuccessfull = false;
							break;
						}
					}
				}

				if (shipPlacementSuccessfull) {

					int pos{ 0 };
					for (int j = 0; j < shipSize; ++j) {
						pos = shipStartY * (width + 1) + shipStartX;
						board[pos] = BOARD_SYMBOL_SHIP;

						if (shipOrientation == SHIP_POS_VERTICAL) {
							++shipStartY;
						}
						else {
							++shipStartX;
						}
					}
					++i;
				}
			}

			if (shipPlacementSuccessfull) {
				--shipSize;
				shipPlacementSuccessfull = false;
			}

			if (shipSize == 0) {
				generateShips = false;
			}
		}
	};

	void placeRNGShips() {
		srand(time(NULL));
		int shipOrientation{ 0 };
		int shipStartX{ 0 };
		int shipStartY{ 0 };
		int shipSize{ 4 };
		bool generateShips{ true };
		bool shipPlacementSuccessfull{ false };

		while (generateShips) {

			int i{ shipSize };
			while (i != 5) {
				shipOrientation = rand() % 2;
				shipStartX = rand() % (width - 1) + 1;
				shipStartY = rand() % (height - 1) + 1;

				int tempShipStartX = shipStartX;
				int tempShipStartY = shipStartY;

				for (int j = 0; j < shipSize; ++j) {
					if (shipOrientation == SHIP_POS_VERTICAL) {
						if (checkShipPosValidity(tempShipStartX, tempShipStartY++)) {
							shipPlacementSuccessfull = true;
						}
						else {
							shipPlacementSuccessfull = false;
							break;
						}
					}
					else {
						if (checkShipPosValidity(tempShipStartX++, tempShipStartY)) {
							shipPlacementSuccessfull = true;
						}
						else {
							shipPlacementSuccessfull = false;
							break;
						}
					}
				}

				if (shipPlacementSuccessfull) {

					int pos{ 0 };
					for (int j = 0; j < shipSize; ++j) {
						pos = shipStartY * (width + 1) + shipStartX;
						board[pos] = BOARD_SYMBOL_SHIP;

						if (shipOrientation == SHIP_POS_VERTICAL) {
							++shipStartY;
						}
						else {
							++shipStartX;
						}
					}
					++i;
				}
			}

			if (shipPlacementSuccessfull) {
				--shipSize;
				shipPlacementSuccessfull = false;
			}

			if (shipSize == 0) {
				generateShips = false;
			}
		}
	};

	int makeMove(int x, int y, string enemyBoard) {
		int pos = y * (width + 1) + x;
		return pos;
	};

	bool handleEvent(int hitPos, GameBoard &enemyBoard) {
		bool hit{ false };
		if (hitPos != -1) {
			if (board.at(hitPos) == BOARD_SYMBOL_EMPTY) {
				if (bot) {
					dummyBoard.at(hitPos) = BOARD_SYMBOL_SHIP_MISS;
				}
				else {
					board.at(hitPos) = BOARD_SYMBOL_SHIP_MISS;
				}
			}
			else if (board.at(hitPos) == BOARD_SYMBOL_SHIP) {
				++shipBlocksDestroyed;
				if (bot) {
					dummyBoard.at(hitPos) = BOARD_SYMBOL_SHIP_HIT;
					int x{ 0 }, y{ 0 };
					x = hitPos % width + 1;
					y = hitPos / width + 1;
					if (checkShipPosValidity(x, y) && positionIsAvailable(hitPos)) {
						enemyBoard.addNewUnvPos(hitPos);
						enemyBoard.addNewUnvPos(hitPos - (width + 1) - 1);
						enemyBoard.addNewUnvPos(hitPos - (width + 1) + 1);
						enemyBoard.addNewUnvPos(hitPos + (width + 1) - 1);
						enemyBoard.addNewUnvPos(hitPos + (width + 1) + 1);
					}
				}
				else {
					board.at(hitPos) = BOARD_SYMBOL_SHIP_HIT;

					enemyBoard.addNewUnvPos(hitPos - (width + 1) - 1);
					enemyBoard.addNewUnvPos(hitPos - (width + 1) + 1);
					enemyBoard.addNewUnvPos(hitPos + (width + 1) - 1);
					enemyBoard.addNewUnvPos(hitPos + (width + 1) + 1);

					enemyBoard.addToSearchBuffer(hitPos - 1);
					enemyBoard.addToSearchBuffer(hitPos + 1);
					enemyBoard.addToSearchBuffer(hitPos - (width + 1));
					enemyBoard.addToSearchBuffer(hitPos + (width + 1));
				}

				hit = true;
			}
		}

		return hit;
	};

	int botMakeMove(const string &enemyBoard) {
		int pos{ 0 };
		if (!searchBuffer.empty()) {
			if (positionIsAvailable(searchBuffer.front())) {
				addNewUnvPos(searchBuffer.front());
				pos = searchBuffer.front();
			}
			searchBuffer.pop();
		}
		else {
			srand(time(0));
			//int tempPos{ 0 };

			while (!positionIsAvailable(pos)) {
				pos = rand() % enemyBoard.length();
			}

			addNewUnvPos(pos);
		}

		return pos;
	};

	void setInitialUnvPostionsForBot(const string &enemyBoard) {
		for (size_t i = 0; i < enemyBoard.length(); ++i) {
			if (enemyBoard.at(i) == BOARD_SYMBOL_SIDE) {
				unavailablePositions.push_back(i);
			}
		}
	};
	const string &getBoard() const {
		if (bot) {
			return dummyBoard;
		}
		else {
			return board;
		}
	};
	int getWidth() { return width; };

	int getHeight() { return height; };

	int getShipBlocksDestroyedCounter() { return shipBlocksDestroyed; };

	void setBot(bool value) { bot = value; };

	bool isBot() { return bot; };

	void addNewUnvPos(int pos) {
		if (positionIsAvailable(pos)) {
			unavailablePositions.push_back(pos);
		}
	};

	void addToSearchBuffer(int pos) {
		if (positionIsAvailable(pos)) {
			searchBuffer.push(pos);
		}
	};
};