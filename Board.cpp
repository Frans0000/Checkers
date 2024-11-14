#include "Board.h"
#include "Game.h"

Board::Board() {}

void Board::initPawns() {
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 8; x++) {
			if ((x + y) % 2 == 0) {
				Pawn pawn = Pawn(PawnColor::WHITE);
				pawn.setPosition(x, y);
				pawnsOnBoard.emplace_back(pawn);
			}
		}
	}

	for (int y = 5; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if ((x + y) % 2 == 0) {
				Pawn pawn = Pawn(PawnColor::BLACK);
				pawn.setPosition(x, y);
				pawnsOnBoard.emplace_back(pawn);
			}
		}
	}
}

void Board::drawPawns(RenderWindow& window) {
	for (int x = 0; x < pawnsOnBoard.size(); x++) {
		pawnsOnBoard[x].draw(window);
	}
	//pawnsOnBoard.pop_back();
}





void Board::removePawns(Vector2i positionToRemove) {
	for (int i = 0; i < pawnsOnBoard.size(); i++) {
		if (pawnsOnBoard[i].getPosition() == positionToRemove) {
			pawnsOnBoard.erase(pawnsOnBoard.begin() + i);
		}
	}
}

