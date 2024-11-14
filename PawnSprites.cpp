#include "PawnSprites.h"

PawnSprites::PawnSprites()
{
	whitePawnTexture.loadFromFile("resources/white_pawn.png");
	whitePawnSprite.setTexture(whitePawnTexture);
	blackPawnTexture.loadFromFile("resources/black_pawn.png");
	blackPawnSprite.setTexture(blackPawnTexture);
	whiteQueenTexture.loadFromFile("resources/white_queen.png");
	whiteQueenSprite.setTexture(whiteQueenTexture);
	blackQueenTexture.loadFromFile("resources/black_queen.png");
	blackQueenSprite.setTexture(blackQueenTexture);
}

Sprite PawnSprites::blackPawnSprite;
Sprite PawnSprites::whitePawnSprite;
Sprite PawnSprites::blackQueenSprite;
Sprite PawnSprites::whiteQueenSprite;