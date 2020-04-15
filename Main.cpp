//
//  Main.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <iostream>

int main(int argc, char** argv)
{
	Game game;

	if (game.Initialize()) {
		game.RunLoop();
		game.Shutdown();
	}
	else {
		std::cout << "Error initializing the game." << std::endl;
	}

	return 0;
}
