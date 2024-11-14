#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <Windows.h>
#include <iostream>
#include <vector>
#include "Pawn.h"

using namespace sf;
using namespace std;

class Board {
private:
	

public:
	std::vector<class Pawn> pawnsOnBoard;
	Board();

	//Functions
	void initPawns();
	void drawPawns(RenderWindow& window);
	void removePawns(Vector2i positionToRemove);
	void makePawnBigger(Vector2i positionToBigger);
	void makePawnNormal(Vector2i positionToNormal);
};
