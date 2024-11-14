#pragma once
#include "Pawn.h"

#include <iostream>
#include <map>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

using namespace sf;

class QueenPawn :public Pawn {
protected:
	int var1;
	int var2;

	double var3;
public:
	QueenPawn(){};
	void additionalFun();
};
