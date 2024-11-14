#include "Game.h"

using namespace sf;

//Initialization
Game::Game() {}

Game::~Game() {
	delete this->window;
}

void Game::mainMenu()
{
	alreadyPressed = false;
	backgroundTexture.loadFromFile("resources/menu.png");
	backgroundTexture.setSmooth(true);
	backgroundImage = Sprite(backgroundTexture);
	backgroundImage.setScale(WIDTH / (float)backgroundTexture.getSize().x, HEIGHT / (float)backgroundTexture.getSize().y);
}

void Game::checkGameOver()
{
	bool black = false;
	bool white = false;
	for (int i = 0; i < gameBoard.pawnsOnBoard.size(); i++) {
		if (gameBoard.pawnsOnBoard[i].color == BLACK) {
			black = true;
		}
		else if (gameBoard.pawnsOnBoard[i].color == WHITE) {
			white = true;
		}
	}
	if (black == false) {
		whiteWon = true;
		backgroundTexture.loadFromFile("resources/white_won.png");
		backgroundTexture.setSmooth(true);
		backgroundImage = Sprite(backgroundTexture);
		backgroundImage.setScale(WIDTH / (float)backgroundTexture.getSize().x, HEIGHT / (float)backgroundTexture.getSize().y);
	}
	else if (white == false) {
		blackWon = true;
		backgroundTexture.loadFromFile("resources/black_won.png");
		backgroundTexture.setSmooth(true);
		backgroundImage = Sprite(backgroundTexture);
		backgroundImage.setScale(WIDTH / (float)backgroundTexture.getSize().x, HEIGHT / (float)backgroundTexture.getSize().y);
	}
}

void Game::initWindow() {
	WIDTH = (int)(sf::VideoMode::getDesktopMode().width);
	HEIGHT = (int)(sf::VideoMode::getDesktopMode().height);
	window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Checkers", sf::Style::Fullscreen, sf::ContextSettings::ContextSettings(0, 0, 10, 2, 0));
	window->setFramerateLimit(20);
}

void Game::initPlansza() {
	backgroundTexture.loadFromFile("resources/board.png");
	backgroundTexture.setSmooth(true);
	backgroundImage = Sprite(backgroundTexture);
	backgroundImage.setScale(WIDTH / (float)backgroundTexture.getSize().x, HEIGHT / (float)backgroundTexture.getSize().y);
	alreadyPressed = false;
	gameBoard = Board();
	gameBoard.initPawns();
}

//Checking if there is a window
const bool Game::isWindowOpen() {
	return this->window->isOpen();
}

void Game::frameUpdate() {
	pollEvents();
	checkButtonClick();
}

void Game::pollEvents() {
	while (this->window->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape) {
				window->close();
				break;
			}
		}
	}
}

void Game::getMousePosition()
{
	mousePos = Mouse::getPosition(*window);
}

void Game::checkPlayButtonClick() {
	if (Mouse::isButtonPressed(Mouse::Left) && !alreadyPressed) {
		getMousePosition();
		alreadyPressed = true;
	}
	else if (!Mouse::isButtonPressed(Mouse::Left) && alreadyPressed) {
		alreadyPressed = false;
	}
}

bool Game::waitPlayButton() {
	checkPlayButtonClick();
	if (mousePos.x > 723 and mousePos.x < 1194 and mousePos.y > 459 and mousePos.y < 561) {
		play = true;
		bot = false;
		initPlansza();
		return true;
	}
	else if (mousePos.x > 696 and mousePos.x < 1224 and mousePos.y > 637 and mousePos.y < 739) {
		play = true;
		bot = true;
		initPlansza();
		return true;
	}
	return false;
}

void Game::checkButtonClick()
{
	if (Mouse::isButtonPressed(Mouse::Left) && !alreadyPressed) {
		getMousePosition();
		movePawn();
		alreadyPressed = true;
	}
	else if (!Mouse::isButtonPressed(Mouse::Left) && alreadyPressed) {
		alreadyPressed = false;
	}

}

bool Game::whatPositionClicked()
{
	if (mousePos.x > 494 && mousePos.x < 1450 && mousePos.y > 63 && mousePos.y < 1015) {
		clickedArea.x = std::ceil((mousePos.x - 494) / 121);
		clickedArea.y = std::ceil((mousePos.y - 63) / 120);
		return true;
	}
	return false;
}

PawnColor Game::whatIsThere(int x, int y) {
	Vector2i place(x, y);
	for (int i = 0; i < gameBoard.pawnsOnBoard.size(); i++) {
		if (gameBoard.pawnsOnBoard[i].getPosition() == place) {
			return gameBoard.pawnsOnBoard[i].color;
			break;
		}
	}
	return EMPTY;
}

//search moves for queen or for normal pawn
void Game::searchAvaiableMoves(Pawn* clickedPawn) {
	Vector2i actualPosition = clickedPawn->getPosition();
	Vector2i attacking;
	clickedPawn->normalMoves.clear();
	clickedPawn->attackingMoves.clear();

	bool pawnHopped = false;

	if (clickedPawn->color == WHITE) {
		if (clickedPawn->getQueen() == false) {
			//forward right move without attack for white pawns
			if (actualPosition.x + 1 <= 7 and actualPosition.y + 1 <= 7) {
				if (whatIsThere(actualPosition.x + 1, actualPosition.y + 1) == EMPTY) {
					clickedPawn->normalMoves.push_back(Vector2i(actualPosition.x + 1, actualPosition.y + 1));
				}
				//right attack move for white pawns
				else if (whatIsThere(actualPosition.x + 1, actualPosition.y + 1) == BLACK) {
					if (actualPosition.x + 2 <= 7 and actualPosition.y + 2 <= 7) {
						if (whatIsThere(actualPosition.x + 2, actualPosition.y + 2) == EMPTY) {
							//tutaj bedziesz musial zrobic przymus bicia
							clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x + 2, actualPosition.y + 2));
						}
					}
				}
			}
			//forward left move without attack for white pawns
			if (actualPosition.x - 1 >= 0 and actualPosition.y + 1 <= 7) {
				if (whatIsThere(actualPosition.x - 1, actualPosition.y + 1) == EMPTY) {
					clickedPawn->normalMoves.push_back(Vector2i(actualPosition.x - 1, actualPosition.y + 1));
				}
				//left attack move for white pawns
				if (whatIsThere(actualPosition.x - 1, actualPosition.y + 1) == BLACK) {
					if (actualPosition.x - 2 >= 0 and actualPosition.y + 2 <= 7) {
						if (whatIsThere(actualPosition.x - 2, actualPosition.y + 2) == EMPTY) {
							clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x - 2, actualPosition.y + 2));
							//you have to attack
						}
					}
				}
			}

			//attacking backwards left
			if (actualPosition.x - 1 >= 0 and actualPosition.y - 1 >= 0) {
				//left attack move for white pawns
				if (whatIsThere(actualPosition.x - 1, actualPosition.y - 1) == BLACK) {
					if (actualPosition.x - 2 >= 0 and actualPosition.y - 2 >= 0) {
						if (whatIsThere(actualPosition.x - 2, actualPosition.y - 2) == EMPTY) {
							clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x - 2, actualPosition.y - 2));
							//you have to attack
						}
					}
				}
			}

			//attacking backwards right
			if (actualPosition.x + 1 <= 7 and actualPosition.y - 1 >= 0) {
				//left attack move for white pawns
				if (whatIsThere(actualPosition.x + 1, actualPosition.y - 1) == BLACK) {
					if (actualPosition.x + 2 <= 7 and actualPosition.y - 2 >= 0) {
						if (whatIsThere(actualPosition.x + 2, actualPosition.y - 2) == EMPTY) {
							clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x + 2, actualPosition.y - 2));
							//you have to attack
						}
					}
				}
			}
		}

		////extending queen functionality
		else {//extending moves if the pawn is queen
			//forward right move without attack for white pawns
			for (int i = 1; i <= 7; i++) {
				if (actualPosition.x + i <= 7 and actualPosition.y + i <= 7) {
					if (whatIsThere(actualPosition.x + i, actualPosition.y + i) == EMPTY and pawnHopped == false) { //empty space so no attack
						clickedPawn->normalMoves.push_back(Vector2i(actualPosition.x + i, actualPosition.y + i));
					}
					else if (whatIsThere(actualPosition.x + i, actualPosition.y + i) == WHITE) {
						break; //if it met same color then brake
					}
					//right attack move for white pawns
					else if (whatIsThere(actualPosition.x + i, actualPosition.y + i) == BLACK) {
						if (pawnHopped == true) { //there is second black pawn
							break;
						}
						else {
							if (actualPosition.x + i + 1 <= 7 and actualPosition.y + i + 1 <= 7) {
								if (whatIsThere(actualPosition.x + i + 1, actualPosition.y + i + 1) == EMPTY) {
									pawnHopped = true;
									clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x + i + 1, actualPosition.y + i + 1));
									//tu moznaby bylo zapisac wspolrzedne tego pionka do usuniecia
									rightForward = Vector2i(actualPosition.x + i, actualPosition.y + i);
								}
								else {
									break; //met black pawn but there is another pawn after it
								}
							}
							else {
								break; //there was black pawn but there is no board after it
							}
						}
					}

					else if (whatIsThere(actualPosition.x + i, actualPosition.y + i) == EMPTY and pawnHopped == true) {
						clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x + i, actualPosition.y + i));
					}
				}
				else {
					break;
				}
			}

			pawnHopped = false;
			//forward left move without attack for white pawns
			for (int i = 1; i <= 7; i++) {
				if (actualPosition.x - i >= 0 and actualPosition.y + i <= 7) {
					if (whatIsThere(actualPosition.x - i, actualPosition.y + i) == EMPTY and pawnHopped == false) {
						clickedPawn->normalMoves.push_back(Vector2i(actualPosition.x - i, actualPosition.y + i));
					}
					else if (whatIsThere(actualPosition.x - i, actualPosition.y + i) == WHITE) {
						break;
					}
					//right attack move for white pawns
					else if (whatIsThere(actualPosition.x - i, actualPosition.y + i) == BLACK) {
						if (pawnHopped == true) {
							break;
						}
						else {
							if (actualPosition.x - i - 1 >= 0 and actualPosition.y + i + 1 <= 7) {
								if (whatIsThere(actualPosition.x - i - 1, actualPosition.y + i + 1) == EMPTY) {
									pawnHopped = true;
									clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x - i - 1, actualPosition.y + i + 1));
									leftForward = Vector2i(actualPosition.x - i, actualPosition.y + i);
								}
								else {
									break;
								}
							}
							else {
								break;
							}
						}
					}

					else if (whatIsThere(actualPosition.x - i, actualPosition.y + i) == EMPTY and pawnHopped == true) {
						clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x - i, actualPosition.y + i));
					}
				}
				else {
					break;
				}
			}

			pawnHopped = false;
			for (int i = 1; i <= 7; i++) {
				if (actualPosition.x - i >= 0 and actualPosition.y - i >= 0) {
					if (whatIsThere(actualPosition.x - i, actualPosition.y - i) == EMPTY and pawnHopped == false) {
						clickedPawn->normalMoves.push_back(Vector2i(actualPosition.x - i, actualPosition.y - i));
					}
					else if (whatIsThere(actualPosition.x - i, actualPosition.y - i) == WHITE) {
						break;
					}

					else if (whatIsThere(actualPosition.x - i, actualPosition.y - i) == BLACK) {
						if (pawnHopped == true) {
							break;
						}
						else {
							if (actualPosition.x - i - 1 >= 0 and actualPosition.y - i - 1 >= 0) {
								if (whatIsThere(actualPosition.x - i - 1, actualPosition.y - i - 1) == EMPTY) {
									pawnHopped = true;
									clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x - i - 1, actualPosition.y - i - 1));
									leftBackwards = Vector2i(actualPosition.x - i, actualPosition.y - i);
								}
								else {
									break;
								}
							}
							else {
								break;
							}
						}
					}

					else if (whatIsThere(actualPosition.x - i, actualPosition.y - i) == EMPTY and pawnHopped == true) {
						clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x - i, actualPosition.y - i));
					}
				}
				else {
					break;
				}
			}

			pawnHopped = false;
			for (int i = 1; i <= 7; i++) {
				if (actualPosition.x + i <= 7 and actualPosition.y - i >= 0) {
					if (whatIsThere(actualPosition.x + i, actualPosition.y - i) == EMPTY and pawnHopped == false) {
						clickedPawn->normalMoves.push_back(Vector2i(actualPosition.x + i, actualPosition.y - i));
					}
					else if (whatIsThere(actualPosition.x + i, actualPosition.y - i) == WHITE) {
						break;
					}
					//right attack move for white pawns
					else if (whatIsThere(actualPosition.x + i, actualPosition.y - i) == BLACK) {
						if (pawnHopped == true) {
							break;
						}
						else {
							if (actualPosition.x + i + 1 <= 7 and actualPosition.y - i - 1 >= 0) {
								if (whatIsThere(actualPosition.x + i + 1, actualPosition.y - i - 1) == EMPTY) {
									pawnHopped = true;
									clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x + i + 1, actualPosition.y - i - 1));
									rightBackwards = Vector2i(actualPosition.x + i, actualPosition.y - i);
								}
								else {
									break;
								}
							}
							else {
								break;
							}
						}
					}

					else if (whatIsThere(actualPosition.x + i, actualPosition.y - i) == EMPTY and pawnHopped == true) {
						clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x + i, actualPosition.y - i));
					}
				}
				else {
					break;
				}
			}

		}
	}

	else if (clickedPawn->color == BLACK) {
		if (clickedPawn->getQueen() == false) {
			//right forward for black pawn
			if (actualPosition.x + 1 <= 7 and actualPosition.y - 1 >= 0) {
				if (whatIsThere(actualPosition.x + 1, actualPosition.y - 1) == EMPTY) {
					clickedPawn->normalMoves.push_back(Vector2i(actualPosition.x + 1, actualPosition.y - 1));
				}
				//right attack move for black pawns
				if (whatIsThere(actualPosition.x + 1, actualPosition.y - 1) == WHITE) {
					if (actualPosition.x + 2 <= 7 and actualPosition.y - 2 >= 0) {
						if (whatIsThere(actualPosition.x + 2, actualPosition.y - 2) == EMPTY) {
							clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x + 2, actualPosition.y - 2));
						}
					}
				}
			}

			//forward left for black pawn
			if (actualPosition.x - 1 >= 0 and actualPosition.y - 1 >= 0) {
				if (whatIsThere(actualPosition.x - 1, actualPosition.y - 1) == EMPTY) {
					clickedPawn->normalMoves.push_back(Vector2i(actualPosition.x - 1, actualPosition.y - 1));
				}
				//left attack move for black pawns
				if (whatIsThere(actualPosition.x - 1, actualPosition.y - 1) == WHITE) {
					if (actualPosition.x - 2 >= 0 and actualPosition.y - 2 >= 0) {
						if (whatIsThere(actualPosition.x - 2, actualPosition.y - 2) == EMPTY) {
							clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x - 2, actualPosition.y - 2));
						}
					}
				}
			}

			//backwards right attack
			if (actualPosition.x + 1 <= 7 and actualPosition.y + 1 <= 7) {
				//left attack move for black pawns
				if (whatIsThere(actualPosition.x + 1, actualPosition.y + 1) == WHITE) {
					if (actualPosition.x + 2 <= 7 and actualPosition.y + 2 <= 7) {
						if (whatIsThere(actualPosition.x + 2, actualPosition.y + 2) == EMPTY) {
							clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x + 2, actualPosition.y + 2));
						}
					}
				}
			}

			if (actualPosition.x - 1 >= 0 and actualPosition.y + 1 <= 7) {
				//left attack move for black pawns
				if (whatIsThere(actualPosition.x - 1, actualPosition.y + 1) == WHITE) {
					if (actualPosition.x - 2 >= 0 and actualPosition.y + 2 <= 7) {
						if (whatIsThere(actualPosition.x - 2, actualPosition.y + 2) == EMPTY) {
							clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x - 2, actualPosition.y + 2));
						}
					}
				}
			}
		}
		////extending queen functionality
		else {//extending moves if the pawn is queen
			//forward right move without attack for white pawns
			for (int i = 1; i <= 7; i++) {
				if (actualPosition.x + i <= 7 and actualPosition.y + i <= 7) {
					if (whatIsThere(actualPosition.x + i, actualPosition.y + i) == EMPTY and pawnHopped == false) {
						clickedPawn->normalMoves.push_back(Vector2i(actualPosition.x + i, actualPosition.y + i));
					}
					else if (whatIsThere(actualPosition.x + i, actualPosition.y + i) == BLACK) {
						break;
					}
					//right attack move for white pawns
					else if (whatIsThere(actualPosition.x + i, actualPosition.y + i) == WHITE) {
						if (pawnHopped == true) {
							break;
						}
						else {
							if (actualPosition.x + i + 1 <= 7 and actualPosition.y + i + 1 <= 7) {
								if (whatIsThere(actualPosition.x + i + 1, actualPosition.y + i + 1) == EMPTY) {
									pawnHopped = true;
									clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x + i + 1, actualPosition.y + i + 1));
									rightForward = Vector2i(actualPosition.x + i, actualPosition.y + i);
								}
								else {
									break;
								}
							}
							else {
								break;
							}
						}
					}

					else if (whatIsThere(actualPosition.x + i, actualPosition.y + i) == EMPTY and pawnHopped == true) {
						clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x + i, actualPosition.y + i));
					}
				}
				else {
					break;
				}
			}

			pawnHopped = false;
			for (int i = 1; i <= 7; i++) {
				if (actualPosition.x - i >= 0 and actualPosition.y + i <= 7) {
					if (whatIsThere(actualPosition.x - i, actualPosition.y + i) == EMPTY and pawnHopped == false) {
						clickedPawn->normalMoves.push_back(Vector2i(actualPosition.x - i, actualPosition.y + i));
					}
					else if (whatIsThere(actualPosition.x - i, actualPosition.y + i) == BLACK) {
						break;
					}
					//right attack move for white pawns
					else if (whatIsThere(actualPosition.x - i, actualPosition.y + i) == WHITE) {
						if (pawnHopped == true) {
							break;
						}
						else {
							if (actualPosition.x - i - 1 >= 0 and actualPosition.y + i + 1 <= 7) {
								if (whatIsThere(actualPosition.x - i - 1, actualPosition.y + i + 1) == EMPTY) {
									pawnHopped = true;
									clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x - i - 1, actualPosition.y + i + 1));
									leftForward = Vector2i(actualPosition.x - i, actualPosition.y + i);
								}
								else {
									break;
								}
							}
							else {
								break;
							}
						}
					}

					else if (whatIsThere(actualPosition.x - i, actualPosition.y + i) == EMPTY and pawnHopped == true) {
						clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x - i, actualPosition.y + i));
					}
				}
				else {
					break;
				}
			}

			pawnHopped = false;
			for (int i = 1; i <= 7; i++) {
				if (actualPosition.x - i >= 0 and actualPosition.y - i >= 0) {
					if (whatIsThere(actualPosition.x - i, actualPosition.y - i) == EMPTY and pawnHopped == false) {
						clickedPawn->normalMoves.push_back(Vector2i(actualPosition.x - i, actualPosition.y - i));
					}
					else if (whatIsThere(actualPosition.x - i, actualPosition.y - i) == BLACK) {
						break;
					}
					//right attack move for white pawns
					else if (whatIsThere(actualPosition.x - i, actualPosition.y - i) == WHITE) {
						if (pawnHopped == true) {
							break;
						}
						else {
							if (actualPosition.x - i - 1 >= 0 and actualPosition.y - i - 1 >= 0) {
								if (whatIsThere(actualPosition.x - i - 1, actualPosition.y - i - 1) == EMPTY) {
									pawnHopped = true;
									clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x - i - 1, actualPosition.y - i - 1));
									leftBackwards = Vector2i(actualPosition.x - i, actualPosition.y - i);
								}
								else {
									break;
								}
							}
							else {
								break;
							}
						}
					}

					else if (whatIsThere(actualPosition.x - i, actualPosition.y - i) == EMPTY and pawnHopped == true) {
						clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x - i, actualPosition.y - i));
					}
				}
				else {
					break;
				}
			}

			pawnHopped = false;
			for (int i = 1; i <= 7; i++) {
				if (actualPosition.x + i <= 7 and actualPosition.y - i >= 0) {
					if (whatIsThere(actualPosition.x + i, actualPosition.y - i) == EMPTY and pawnHopped == false) {
						clickedPawn->normalMoves.push_back(Vector2i(actualPosition.x + i, actualPosition.y - i));
					}
					else if (whatIsThere(actualPosition.x + i, actualPosition.y - i) == BLACK) {
						break;
					}
					//right attack move for white pawns
					else if (whatIsThere(actualPosition.x + i, actualPosition.y - i) == WHITE) {
						if (pawnHopped == true) {
							break;
						}
						else {
							if (actualPosition.x + i + 1 <= 7 and actualPosition.y - i - 1 >= 0) {
								if (whatIsThere(actualPosition.x + i + 1, actualPosition.y - i - 1) == EMPTY) {
									pawnHopped = true;
									clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x + i + 1, actualPosition.y - i - 1));
									rightBackwards = Vector2i(actualPosition.x + i, actualPosition.y - i);
								}
								else {
									break;
								}
							}
							else {
								break;
							}
						}
					}

					else if (whatIsThere(actualPosition.x + i, actualPosition.y - i) == EMPTY and pawnHopped == true) {
						clickedPawn->attackingMoves.push_back(Vector2i(actualPosition.x + i, actualPosition.y - i));
					}
				}
				else {
					break;
				}
			}

		}

	}

}

bool Game::searchIfMustAttack(PawnColor color) {

	for (int i = 0; i < gameBoard.pawnsOnBoard.size(); i++) {
		searchAvaiableMoves(&gameBoard.pawnsOnBoard[i]);
		if (gameBoard.pawnsOnBoard[i].color == color and gameBoard.pawnsOnBoard[i].attackingMoves.size()) {
			return true;
		}
	}
	return false;
}

void Game::movePawn() {
	if (bot == false) {
		bool rightPositionClicked = whatPositionClicked();
		if (secondMove == false) {
			if (rightPositionClicked and isPawnWaiting == false) {

				for (int i = 0; i < gameBoard.pawnsOnBoard.size(); i++) {
					if (gameBoard.pawnsOnBoard[i].getPosition() == clickedArea) {
						if (((gameBoard.pawnsOnBoard[i].color == WHITE) and whiteTurn) or ((gameBoard.pawnsOnBoard[i].color == BLACK) and (whiteTurn == false))) {
							if (searchIfMustAttack(gameBoard.pawnsOnBoard[i].color)) { //the given color can attack
								searchAvaiableMoves(&gameBoard.pawnsOnBoard[i]);
								if (gameBoard.pawnsOnBoard[i].attackingMoves.size()) { //there is an attacking move and player chose pawn who can attack
									gameBoard.pawnsOnBoard[i].setScaleBigger();

									isPawnWaiting = true;
									clickedPawnNumber = i;
									firstClickedPawn = clickedArea;

								}
								return; //there is other pawn which can attack, player has to click again on the right one
							}
							else { // there is no attacking move
								gameBoard.pawnsOnBoard[i].setScaleBigger();
								searchAvaiableMoves(&gameBoard.pawnsOnBoard[i]);
								isPawnWaiting = true;
								clickedPawnNumber = i;
								firstClickedPawn = clickedArea;
								return;
							}
						}
					}
				}

			}
			else if (rightPositionClicked and isPawnWaiting == true) {
				bool alreadyMoved = false;
				if (clickedArea == firstClickedPawn) {
					isPawnWaiting = false;
					gameBoard.pawnsOnBoard[clickedPawnNumber].setScaleNormal();
				}
				else {
					//you are moving your pawn while attacking
					for (int i = 0; i < gameBoard.pawnsOnBoard[clickedPawnNumber].attackingMoves.size(); i++) {
						if (gameBoard.pawnsOnBoard[clickedPawnNumber].attackingMoves[i] == clickedArea) {
							gameBoard.pawnsOnBoard[clickedPawnNumber].setPosition(clickedArea);

							Vector2i toRemove(0, 0);

							if (gameBoard.pawnsOnBoard[clickedPawnNumber].getQueen()) {
								if (firstClickedPawn.x < clickedArea.x and firstClickedPawn.y < clickedArea.y) { //right forward
									toRemove = rightForward;
								}
								else if (firstClickedPawn.x > clickedArea.x and firstClickedPawn.y < clickedArea.y) { //left forward
									toRemove = leftForward;
								}
								else if (firstClickedPawn.x > clickedArea.x and firstClickedPawn.y > clickedArea.y) { //left backwards
									toRemove = leftBackwards;
								}
								else if (firstClickedPawn.x < clickedArea.x and firstClickedPawn.y > clickedArea.y) { //right backwards
									toRemove = rightBackwards;
								}
							}
							else {
								toRemove.x = (clickedArea.x + firstClickedPawn.x) / 2;
								toRemove.y = (clickedArea.y + firstClickedPawn.y) / 2;
							}

							gameBoard.pawnsOnBoard[clickedPawnNumber].setScaleNormal();
							alreadyMoved = true;
							checkIfQueenUpgrade(clickedPawnNumber);

							gameBoard.removePawns(toRemove); //this will change the number of pawn to remove at the end

							for (int j = 0; j < gameBoard.pawnsOnBoard.size(); j++) {
								if (gameBoard.pawnsOnBoard[j].getPosition() == clickedArea) {
									searchAvaiableMoves(&gameBoard.pawnsOnBoard[j]); //after attacking move you search if other moves are avaiable

									if (gameBoard.pawnsOnBoard[j].attackingMoves.size() == 0) { //if there are next moves the player have to choose the same pawn
										whiteTurn = !whiteTurn;
										secondMove = false;
									}
									else {
										mustClickPawnArea = clickedArea;
										secondMove = true;
									}
									isPawnWaiting = false;
									break;
								}
							}
							isPawnWaiting = false;

							break;
						}
					}

					//normal move without attack
					if (alreadyMoved == false) {
						for (int i = 0; i < gameBoard.pawnsOnBoard[clickedPawnNumber].normalMoves.size(); i++) {
							if (gameBoard.pawnsOnBoard[clickedPawnNumber].normalMoves[i] == clickedArea) {
								gameBoard.pawnsOnBoard[clickedPawnNumber].setPosition(clickedArea);
								alreadyMoved = true;
								gameBoard.pawnsOnBoard[clickedPawnNumber].setScaleNormal();
								isPawnWaiting = false;
								whiteTurn = !whiteTurn;
								checkIfQueenUpgrade(clickedPawnNumber);
								break;
							}
						}
					}

				}

			}
		}
		////////////////////////////////////////////////////////  second attack move
		else {
			if (rightPositionClicked and isPawnWaiting == false) {

				if (clickedArea == mustClickPawnArea) {
					for (int i = 0; i < gameBoard.pawnsOnBoard.size(); i++) {
						if (gameBoard.pawnsOnBoard[i].getPosition() == clickedArea) {
							gameBoard.pawnsOnBoard[i].setScaleBigger();
							searchAvaiableMoves(&gameBoard.pawnsOnBoard[i]);
							isPawnWaiting = true;
							clickedPawnNumber = i;
							firstClickedPawn = clickedArea;
							break;
						}
					}
				}
			}
			else if (rightPositionClicked and isPawnWaiting == true) {
				bool alreadyMoved = false;
				if (clickedArea == firstClickedPawn) {
					isPawnWaiting = false;
					gameBoard.pawnsOnBoard[clickedPawnNumber].setScaleNormal();
				}
				else {
					//you are moving your pawn while attacking
					for (int i = 0; i < gameBoard.pawnsOnBoard[clickedPawnNumber].attackingMoves.size(); i++) {
						if (gameBoard.pawnsOnBoard[clickedPawnNumber].attackingMoves[i] == clickedArea) {
							gameBoard.pawnsOnBoard[clickedPawnNumber].setPosition(clickedArea);
							Vector2i toRemove((clickedArea.x + firstClickedPawn.x) / 2, (clickedArea.y + firstClickedPawn.y) / 2);
							gameBoard.pawnsOnBoard[clickedPawnNumber].setScaleNormal();
							alreadyMoved = true;
							checkIfQueenUpgrade(clickedPawnNumber);

							gameBoard.removePawns(toRemove); //this will change the number of pawn so remove at the end

							for (int j = 0; j < gameBoard.pawnsOnBoard.size(); j++) {
								if (gameBoard.pawnsOnBoard[j].getPosition() == clickedArea) {
									searchAvaiableMoves(&gameBoard.pawnsOnBoard[j]); //after attacking move you search if other moves are avaiable

									if (gameBoard.pawnsOnBoard[j].attackingMoves.size() == 0) { //if there are next moves the player have to choose the same pawn
										whiteTurn = !whiteTurn;
										secondMove = false;
									}
									else {
										mustClickPawnArea = clickedArea;
										secondMove = true;
									}
									isPawnWaiting = false;
									break;
								}
							}
							isPawnWaiting = false;

							break;
						}
					}

					//normal move without attack
					if (alreadyMoved == false) {
						for (int i = 0; i < gameBoard.pawnsOnBoard[clickedPawnNumber].normalMoves.size(); i++) {
							if (gameBoard.pawnsOnBoard[clickedPawnNumber].normalMoves[i] == clickedArea) {
								gameBoard.pawnsOnBoard[clickedPawnNumber].setPosition(clickedArea);
								alreadyMoved = true;
								gameBoard.pawnsOnBoard[clickedPawnNumber].setScaleNormal();
								isPawnWaiting = false;
								whiteTurn = !whiteTurn;
								checkIfQueenUpgrade(clickedPawnNumber);
								break;
							}
						}
					}

				}

			}
		}
	}
	else {
		if (whiteTurn) {
			bool rightPositionClicked = whatPositionClicked();
			if (secondMove == false) {
				if (rightPositionClicked and isPawnWaiting == false) {

					for (int i = 0; i < gameBoard.pawnsOnBoard.size(); i++) {
						if (gameBoard.pawnsOnBoard[i].getPosition() == clickedArea) {
							if (((gameBoard.pawnsOnBoard[i].color == WHITE) and whiteTurn) or ((gameBoard.pawnsOnBoard[i].color == BLACK) and (whiteTurn == false))) {
								if (searchIfMustAttack(gameBoard.pawnsOnBoard[i].color)) { //the given color can attack
									searchAvaiableMoves(&gameBoard.pawnsOnBoard[i]);
									if (gameBoard.pawnsOnBoard[i].attackingMoves.size()) { //there is an attacking move and player chose pawn who can attack
										gameBoard.pawnsOnBoard[i].setScaleBigger();

										isPawnWaiting = true;
										clickedPawnNumber = i;
										firstClickedPawn = clickedArea;

									}
									return; //there is other pawn which can attack, player has to click again on the right one
								}
								else { // there is no attacking move
									gameBoard.pawnsOnBoard[i].setScaleBigger();
									searchAvaiableMoves(&gameBoard.pawnsOnBoard[i]);
									isPawnWaiting = true;
									clickedPawnNumber = i;
									firstClickedPawn = clickedArea;
									return;
								}
							}
						}
					}

				}
				else if (rightPositionClicked and isPawnWaiting == true) {
					bool alreadyMoved = false;
					if (clickedArea == firstClickedPawn) {
						isPawnWaiting = false;
						gameBoard.pawnsOnBoard[clickedPawnNumber].setScaleNormal();
					}
					else {
						//you are moving your pawn while attacking
						for (int i = 0; i < gameBoard.pawnsOnBoard[clickedPawnNumber].attackingMoves.size(); i++) {
							if (gameBoard.pawnsOnBoard[clickedPawnNumber].attackingMoves[i] == clickedArea) {
								gameBoard.pawnsOnBoard[clickedPawnNumber].setPosition(clickedArea);

								Vector2i toRemove(0, 0);

								if (gameBoard.pawnsOnBoard[clickedPawnNumber].getQueen()) {
									if (firstClickedPawn.x < clickedArea.x and firstClickedPawn.y < clickedArea.y) { //right forward
										toRemove = rightForward;
									}
									else if (firstClickedPawn.x > clickedArea.x and firstClickedPawn.y < clickedArea.y) { //left forward
										toRemove = leftForward;
									}
									else if (firstClickedPawn.x > clickedArea.x and firstClickedPawn.y > clickedArea.y) { //left backwards
										toRemove = leftBackwards;
									}
									else if (firstClickedPawn.x < clickedArea.x and firstClickedPawn.y > clickedArea.y) { //right backwards
										toRemove = rightBackwards;
									}
								}
								else {
									toRemove.x = (clickedArea.x + firstClickedPawn.x) / 2;
									toRemove.y = (clickedArea.y + firstClickedPawn.y) / 2;
								}

								gameBoard.pawnsOnBoard[clickedPawnNumber].setScaleNormal();
								alreadyMoved = true;
								checkIfQueenUpgrade(clickedPawnNumber);

								gameBoard.removePawns(toRemove);

								for (int j = 0; j < gameBoard.pawnsOnBoard.size(); j++) {
									if (gameBoard.pawnsOnBoard[j].getPosition() == clickedArea) {
										searchAvaiableMoves(&gameBoard.pawnsOnBoard[j]); //after attacking move you search if other moves are avaiable

										if (gameBoard.pawnsOnBoard[j].attackingMoves.size() == 0) { //if there are next moves the player have to choose the same pawn
											whiteTurn = !whiteTurn;
											secondMove = false;
										}
										else {
											mustClickPawnArea = clickedArea;
											secondMove = true;
										}
										isPawnWaiting = false;
										break;
									}
								}
								isPawnWaiting = false;

								break;
							}
						}

						//normal move without attack
						if (alreadyMoved == false) {
							for (int i = 0; i < gameBoard.pawnsOnBoard[clickedPawnNumber].normalMoves.size(); i++) {
								if (gameBoard.pawnsOnBoard[clickedPawnNumber].normalMoves[i] == clickedArea) {
									gameBoard.pawnsOnBoard[clickedPawnNumber].setPosition(clickedArea);
									alreadyMoved = true;
									gameBoard.pawnsOnBoard[clickedPawnNumber].setScaleNormal();
									isPawnWaiting = false;
									whiteTurn = !whiteTurn;
									checkIfQueenUpgrade(clickedPawnNumber);
									break;
								}
							}
						}

					}

				}
			}
			////////////////////////////////////////////////////////  second attack
			else {
				if (rightPositionClicked and isPawnWaiting == false) {

					if (clickedArea == mustClickPawnArea) {
						for (int i = 0; i < gameBoard.pawnsOnBoard.size(); i++) {
							if (gameBoard.pawnsOnBoard[i].getPosition() == clickedArea) {
								gameBoard.pawnsOnBoard[i].setScaleBigger();
								searchAvaiableMoves(&gameBoard.pawnsOnBoard[i]);
								isPawnWaiting = true;
								clickedPawnNumber = i;
								firstClickedPawn = clickedArea;
								break;
							}
						}
					}
				}
				else if (rightPositionClicked and isPawnWaiting == true) {
					bool alreadyMoved = false;
					if (clickedArea == firstClickedPawn) {
						isPawnWaiting = false;
						gameBoard.pawnsOnBoard[clickedPawnNumber].setScaleNormal();
					}
					else {
						//you are moving your pawn while attacking
						for (int i = 0; i < gameBoard.pawnsOnBoard[clickedPawnNumber].attackingMoves.size(); i++) {
							if (gameBoard.pawnsOnBoard[clickedPawnNumber].attackingMoves[i] == clickedArea) {
								gameBoard.pawnsOnBoard[clickedPawnNumber].setPosition(clickedArea);
								Vector2i toRemove((clickedArea.x + firstClickedPawn.x) / 2, (clickedArea.y + firstClickedPawn.y) / 2);
								gameBoard.pawnsOnBoard[clickedPawnNumber].setScaleNormal();
								alreadyMoved = true;
								checkIfQueenUpgrade(clickedPawnNumber);

								gameBoard.removePawns(toRemove);

								for (int j = 0; j < gameBoard.pawnsOnBoard.size(); j++) {
									if (gameBoard.pawnsOnBoard[j].getPosition() == clickedArea) {
										searchAvaiableMoves(&gameBoard.pawnsOnBoard[j]); //after attacking move you search if other moves are avaiable

										if (gameBoard.pawnsOnBoard[j].attackingMoves.size() == 0) { //if there are next moves the player have to choose the same pawn
											whiteTurn = !whiteTurn;
											secondMove = false;
										}
										else {
											mustClickPawnArea = clickedArea;
											secondMove = true;
										}
										isPawnWaiting = false;
										break;
									}
								}
								isPawnWaiting = false;

								break;
							}
						}

						//normal move without attack
						if (alreadyMoved == false) {
							for (int i = 0; i < gameBoard.pawnsOnBoard[clickedPawnNumber].normalMoves.size(); i++) {
								if (gameBoard.pawnsOnBoard[clickedPawnNumber].normalMoves[i] == clickedArea) {
									gameBoard.pawnsOnBoard[clickedPawnNumber].setPosition(clickedArea);
									alreadyMoved = true;
									gameBoard.pawnsOnBoard[clickedPawnNumber].setScaleNormal();
									isPawnWaiting = false;
									whiteTurn = !whiteTurn;
									checkIfQueenUpgrade(clickedPawnNumber);
									break;
								}
							}
						}

					}

				}
			}
		}
		else {
			//bot is black pawns
			if (searchIfMustAttack(BLACK)) {
				bool attack = true;
				if (secondMove == false) {
					for (int i = 0; i < gameBoard.pawnsOnBoard.size(); i++) {
						if (gameBoard.pawnsOnBoard[i].color == BLACK) {
							searchAvaiableMoves(&gameBoard.pawnsOnBoard[i]);

							//available attacking move
							if (gameBoard.pawnsOnBoard[i].attackingMoves.size()) {
								gameBoard.pawnsOnBoard[i].setScaleBigger();
								//sleep(800);

								Vector2i toRemove(0, 0);
								clickedArea = gameBoard.pawnsOnBoard[i].attackingMoves[0];
								firstClickedPawn = gameBoard.pawnsOnBoard[i].getPosition();

								if (gameBoard.pawnsOnBoard[i].getQueen()) {
									if (firstClickedPawn.x < clickedArea.x and firstClickedPawn.y < clickedArea.y) { //right forward
										toRemove = rightForward;
									}
									else if (firstClickedPawn.x > clickedArea.x and firstClickedPawn.y < clickedArea.y) { //left forward
										toRemove = leftForward;
									}
									else if (firstClickedPawn.x > clickedArea.x and firstClickedPawn.y > clickedArea.y) { //left backwards
										toRemove = leftBackwards;
									}
									else if (firstClickedPawn.x < clickedArea.x and firstClickedPawn.y > clickedArea.y) { //right backwards
										toRemove = rightBackwards;
									}
								}
								else {
									toRemove.x = (clickedArea.x + firstClickedPawn.x) / 2;
									toRemove.y = (clickedArea.y + firstClickedPawn.y) / 2;
								}

								gameBoard.pawnsOnBoard[i].setPosition(clickedArea);
								gameBoard.pawnsOnBoard[i].setScaleNormal();
								checkIfQueenUpgrade(i);
								gameBoard.removePawns(toRemove);

								for (int j = 0; j < gameBoard.pawnsOnBoard.size(); j++) {
									if (gameBoard.pawnsOnBoard[j].getPosition() == clickedArea) {
										searchAvaiableMoves(&gameBoard.pawnsOnBoard[j]); //after attacking move you search if other moves are avaiable

										if (gameBoard.pawnsOnBoard[j].attackingMoves.size() == 0) { //if there are next moves the player have to choose the same pawn
											whiteTurn = !whiteTurn;
											secondMove = false;
											break;
										}
										else {
											mustPickPawnNumber = j;
											secondMove = true;
										}
										break;
									}
								}
								if (secondMove == false) {
									break;
								}

							}
						}
					}
				}
				else {
					searchAvaiableMoves(&gameBoard.pawnsOnBoard[mustPickPawnNumber]);
					gameBoard.pawnsOnBoard[mustPickPawnNumber].setScaleBigger();
					Vector2i toRemove(0, 0);
					clickedArea = gameBoard.pawnsOnBoard[mustPickPawnNumber].attackingMoves[0];
					firstClickedPawn = gameBoard.pawnsOnBoard[mustPickPawnNumber].getPosition();
					if (gameBoard.pawnsOnBoard[mustPickPawnNumber].getQueen()) {
						if (firstClickedPawn.x < clickedArea.x and firstClickedPawn.y < clickedArea.y) { //right forward
							toRemove = rightForward;
						}
						else if (firstClickedPawn.x > clickedArea.x and firstClickedPawn.y < clickedArea.y) { //left forward
							toRemove = leftForward;
						}
						else if (firstClickedPawn.x > clickedArea.x and firstClickedPawn.y > clickedArea.y) { //left backwards
							toRemove = leftBackwards;
						}
						else if (firstClickedPawn.x < clickedArea.x and firstClickedPawn.y > clickedArea.y) { //right backwards
							toRemove = rightBackwards;
						}
					}
					else {
						toRemove.x = (clickedArea.x + firstClickedPawn.x) / 2;
						toRemove.y = (clickedArea.y + firstClickedPawn.y) / 2;
					}

					gameBoard.pawnsOnBoard[mustPickPawnNumber].setPosition(clickedArea);
					gameBoard.pawnsOnBoard[mustPickPawnNumber].setScaleNormal();
					checkIfQueenUpgrade(mustPickPawnNumber);
					gameBoard.removePawns(toRemove);

					for (int j = 0; j < gameBoard.pawnsOnBoard.size(); j++) {
						if (gameBoard.pawnsOnBoard[j].getPosition() == clickedArea) {
							searchAvaiableMoves(&gameBoard.pawnsOnBoard[j]); //after attacking move you search if other moves are avaiable

							if (gameBoard.pawnsOnBoard[j].attackingMoves.size() == 0) { //if there are next moves the player have to choose the same pawn
								whiteTurn = !whiteTurn;
								secondMove = false;
								attack = false;
							}
							else {
								mustPickPawnNumber = j;
								secondMove = true;
							}
							break;
						}
					}
				}
			}

			else {
				int pawnNum = getVectorNumber();

				searchAvaiableMoves(&gameBoard.pawnsOnBoard[pawnNum]);
				int movement;

				if (gameBoard.pawnsOnBoard[pawnNum].normalMoves.size() > 1) {
					movement = pawnNum % 2;
				}
				else {
					movement = 0;
				}

				clickedArea = gameBoard.pawnsOnBoard[pawnNum].normalMoves[movement];
				gameBoard.pawnsOnBoard[pawnNum].setPosition(clickedArea);
				whiteTurn = !whiteTurn;
				checkIfQueenUpgrade(pawnNum);
			}
		}
	}
	checkGameOver();
}

void Game::checkIfQueenUpgrade(int number) {
	if (clickedArea.y == 0 and gameBoard.pawnsOnBoard[number].color == BLACK) {
		gameBoard.pawnsOnBoard[number].setQueen();
	}

	else if (clickedArea.y == 7 and gameBoard.pawnsOnBoard[number].color == WHITE) {
		gameBoard.pawnsOnBoard[number].setQueen();
	}
}

int Game::getVectorNumber() {
	vector<int> numTable;
	numTable.clear();
	int howMany;
	for (int i = 0; i < gameBoard.pawnsOnBoard.size(); i++) {
		if (gameBoard.pawnsOnBoard[i].color == BLACK) {
			searchAvaiableMoves(&gameBoard.pawnsOnBoard[i]);
			if (gameBoard.pawnsOnBoard[i].normalMoves.size()) {
				numTable.push_back(i);
			}
		}
	}

	howMany = numTable.size();

	srand(static_cast<unsigned int>(time(nullptr)));

	int number;

	try {
		if (howMany > 0) {
			number = rand() % howMany;
		}
		else if (howMany == 0) {
			throw 55;
		}
	}
	catch (int e) {
		number = 0;
	}

	return numTable[number];

}

bool Game::waitPlayAgainButton() {
	checkPlayButtonClick();
	if (mousePos.x > 697 and mousePos.x < 1222 and mousePos.y > 701 and mousePos.y < 785) {
		play = true;
		initPlansza();
		whiteWon = false;
		blackWon = false;
		return true;
	}
	return false;
}

//Drawing the image
void Game::render() {
	this->window->clear();
	if (whiteWon == false and blackWon == false and play == true) {
		this->window->draw(backgroundImage);
		this->gameBoard.drawPawns(*window);
	}
	else {
		this->window->draw(backgroundImage);
		waitPlayAgainButton();
	}
	this->window->display();
}