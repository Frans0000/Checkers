#include "Pawn.h"

Pawn::Pawn() {}
Pawn::Pawn(PawnColor col){
	this->color = col;
	
	if (col == WHITE) {
		pawnSprite = PawnSprites::whitePawnSprite;
	}
	else {
		pawnSprite = PawnSprites::blackPawnSprite;
	}
	
	pawnSprite.setScale(scaleConstant, scaleConstant);
	spriteWidth = pawnSprite.getTexture()->getSize().x * pawnSprite.getScale().x;
	spriteHeight = pawnSprite.getTexture()->getSize().y * pawnSprite.getScale().y;
	pawnSprite.setOrigin(pawnSprite.getTexture()->getSize().x / 2, pawnSprite.getTexture()->getSize().y / 2);
}

Pawn::~Pawn(){}

void Pawn::setPosition(Vector2i pos) {
	pawnSprite.setPosition(494 + pos.x * 121 + (spriteWidth / 2), 63 + pos.y * 120 + (spriteHeight / 2));
	pawnPosition = Vector2i(pos.x, pos.y);
}

void Pawn::setPosition(int x, int y) {
	pawnSprite.setPosition(494 + x * 121 + (spriteWidth / 2), 63 + y * 120 + (spriteHeight / 2));
	pawnPosition = Vector2i(x, y);
}

Vector2i Pawn::getPosition() {
	return pawnPosition;
}

void Pawn::draw(RenderWindow& window) {
	window.draw(pawnSprite);
}

void Pawn::setScaleBigger() {
	pawnSprite.setScale(biggerScaleConstant, biggerScaleConstant);
}

void Pawn::setScaleNormal() {
	pawnSprite.setScale(scaleConstant, scaleConstant);
}

void Pawn::setQueen() {
	queen = true;

	if (this->color == WHITE) {
		pawnSprite = PawnSprites::whiteQueenSprite;
	}
	else {
		pawnSprite = PawnSprites::blackQueenSprite;
	}

	pawnSprite.setScale(scaleConstant, scaleConstant);
	spriteWidth = pawnSprite.getTexture()->getSize().x * pawnSprite.getScale().x;
	spriteHeight = pawnSprite.getTexture()->getSize().y * pawnSprite.getScale().y;
	pawnSprite.setOrigin(pawnSprite.getTexture()->getSize().x / 2, pawnSprite.getTexture()->getSize().y / 2);
	pawnSprite.setPosition(494 + pawnPosition.x * 121 + (spriteWidth / 2), 63 + pawnPosition.y * 120 + (spriteHeight / 2));
}

bool Pawn::getQueen() {
	return queen;
}