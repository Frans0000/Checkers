#pragma once

#include <iostream>
#include <map>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "PawnSprites.h"
#include "Board.h"

using namespace sf;

enum PawnColor {
	WHITE,
	BLACK,
	EMPTY
};

class Pawn {

protected:
	//Variables
	Sprite pawnSprite;
	float scaleConstant = 0.093;
	float biggerScaleConstant = 0.11;
	float spriteWidth, spriteHeight;
	Vector2i pawnPosition;
	bool queen = false;

public:
	Pawn();
	Pawn(PawnColor);
	~Pawn();


	//Variables
	PawnColor color;
	std::vector<sf::Vector2i> normalMoves;
	std::vector<sf::Vector2i> attackingMoves;

	//Functions
	void setScaleBigger();
	void setScaleNormal();
	void setPosition(Vector2i pos);
	void setPosition(int x, int y);
	Vector2i getPosition();
	void draw(RenderWindow& window);
	void searchAvaiableMoves(Pawn clickedPawn);
	void setQueen();
	bool getQueen();
};