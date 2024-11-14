#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <Windows.h>

#include <iostream>
#include "Game.h"

using namespace sf;
//WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
int main(){
	Game game;
	PawnSprites spr = PawnSprites();
	bool playButton = false;

	game.initWindow();
	game.mainMenu();
	//game.initPlansza();

	while (game.isWindowOpen() and playButton == false) {
		playButton = game.waitPlayButton();
		
		game.frameUpdate();

		game.render();
	}

	while (game.isWindowOpen()) {
		//update


		game.frameUpdate();

		game.render();

	}

	return 0;
}