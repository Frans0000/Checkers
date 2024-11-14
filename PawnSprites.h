#pragma once

#include <iostream>
#include <map>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

using namespace sf;

class PawnSprites {
private:
	Texture whitePawnTexture;
	Texture blackPawnTexture;
	Texture whiteQueenTexture;
	Texture blackQueenTexture;
public:

	PawnSprites();
	static Sprite whitePawnSprite;
	static Sprite blackPawnSprite;
	static Sprite whiteQueenSprite;
	static Sprite blackQueenSprite;
};

