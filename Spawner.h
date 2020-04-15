#pragma once
#include "Actor.h"

class Spawner : public Actor {

	public:
		Spawner(class Game* game);
		void OnUpdate(float deltaTime);

	private:
		Game* mGame;
		const int IN_RANGE = 600;

};