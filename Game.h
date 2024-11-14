#pragma once

#include <iostream>
#include <map>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Board.h"

using namespace sf;

class Game {

private:

	//Variables
	int WIDTH;
	int HEIGHT;
	Texture backgroundTexture;
	Sprite backgroundImage;
	Event event;
	Vector2i mousePos;
	bool alreadyPressed;
	Vector2i clickedArea;
	Board gameBoard;
	Vector2i firstClickedPawn;
	bool isPawnWaiting = false;
	int clickedPawnNumber;
	bool whiteTurn = true;
	Vector2i mustClickPawnArea;
	bool secondMove = false;
	bool whiteWon = false;
	bool blackWon = false;
	bool play = false;
	bool bot = false;
	int mustPickPawnNumber;
public:

	//Variables
	RenderWindow* window;

	//Constructors
	Game();
	virtual ~Game();

	//Getters Accessors
	const bool isWindowOpen();

	//Functions
	bool waitPlayAgainButton();
	bool waitPlayButton();
	void mainMenu();
	void initWindow();
	void initPlansza();
	void frameUpdate();
	void pollEvents();
	void getMousePosition();
	void render();
	void checkButtonClick();
	bool whatPositionClicked();
	void searchAvaiableMoves(Pawn* clickedPawn);
	bool isValidPosition(Vector2i pos);
	PawnColor whatIsThere(int x, int y); 
	void movePawn();
	void checkIfQueenUpgrade(int number);
	bool searchIfMustAttack(PawnColor color);
	Vector2i rightForward;
	Vector2i leftForward;
	Vector2i rightBackwards;
	Vector2i leftBackwards;
	void checkGameOver();
	void checkPlayButtonClick();
	int getVectorNumber();
};