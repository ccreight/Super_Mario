#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "GoombaMove.h"
#include <string>
#include "AnimatedSprite.h"

class Goomba : public Actor {

public:
	Goomba(class Game* game);
	~Goomba();

	CollisionComponent* getCollision() {
		return mGoombaCollision;
	}

	bool getStomped() {
		return mIsStomped;
	}

	void Destroy(float currentTime);

private:
	AnimatedSprite* mGoombaComp;
	class Game* mGame;
	CollisionComponent* mGoombaCollision;
	GoombaMove* mGoombaMoveComp;
	const int MOVE_SPEED = 100;
	bool mIsStomped;
	

};