#include "Goomba.h"
#include "Game.h"
#include <iostream>

Goomba::Goomba(class Game* game) : Actor(game) {

	mGame = game;
	mGame->AddGoomba(this);

	mGoombaComp = new AnimatedSprite(this, 150);

	// Create the walk animation
	std::vector<SDL_Texture*> walkAnim{
	GetGame()->GetTexture("Assets/Goomba/Walk0.png"),
	GetGame()->GetTexture("Assets/Goomba/Walk1.png")
		};

	mGoombaComp->AddAnimation("walk", walkAnim);
	mGoombaComp->SetAnimation("walk");

	mGoombaComp->SetTexture(mGame->GetTexture("Assets/Goomba/Walk0.png"));

	mGoombaCollision = new CollisionComponent(this);
	mGoombaCollision->SetSize(32, 32);

	mGoombaMoveComp = new GoombaMove(this);
	mGoombaMoveComp->SetForwardSpeed(-1 * MOVE_SPEED);

	mIsStomped = false;

}

Goomba::~Goomba() {
	mGame->RemoveGoomba(this);
}

void Goomba::Destroy(float currentTime) {

	// Set dead animation
	std::vector<SDL_Texture*> deadAnim{
	GetGame()->GetTexture("Assets/Goomba/Dead.png")
	};

	mGoombaComp->AddAnimation("dead", deadAnim);
	mGoombaComp->SetAnimation("dead");

	mGoombaMoveComp->SetForwardSpeed(0);
	
	mGoombaCollision->SetSize(32, 32);

	mIsStomped = true;

}