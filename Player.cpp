#include "Player.h"
#include "Game.h"
#include "PlayerMove.h"

Player::Player(class Game* game) : Actor(game){

	mGame = game;
	mPlayerComp = new AnimatedSprite(this, 200);

	InitAnims();
	mPlayerComp->SetAnimation("idle");

	mPlayerMoveComp = new MoveComponent(this);

	mPlayerCollision = new CollisionComponent(this);
	mPlayerCollision->SetSize(32, 32);

	mPlayerMove = new PlayerMove(this);

}

void Player::SetTexture(std::string s) {
	mPlayerComp->SetTexture(mGame->GetTexture(s));
}

// Initializes all player animations
void Player::InitAnims() {

	deadAnim.push_back(mGame->GetTexture("Assets/Mario/Dead.png"));
	mPlayerComp->AddAnimation("dead", deadAnim);

	idleAnim.push_back(mGame->GetTexture("Assets/Mario/Idle.png"));
	mPlayerComp->AddAnimation("idle", idleAnim);

	jumpLeft.push_back(mGame->GetTexture("Assets/Mario/JumpLeft.png"));
	mPlayerComp->AddAnimation("jumpLeft", jumpLeft);

	jumpRight.push_back(mGame->GetTexture("Assets/Mario/JumpRight.png"));
	mPlayerComp->AddAnimation("jumpRight", jumpRight);

	runLeft.push_back(mGame->GetTexture("Assets/Mario/RunLeft0.png"));
	runLeft.push_back(mGame->GetTexture("Assets/Mario/RunLeft1.png"));
	runLeft.push_back(mGame->GetTexture("Assets/Mario/RunLeft2.png"));
	mPlayerComp->AddAnimation("runLeft", runLeft);

	runRight.push_back(mGame->GetTexture("Assets/Mario/RunRight0.png"));
	runRight.push_back(mGame->GetTexture("Assets/Mario/RunRight1.png"));
	runRight.push_back(mGame->GetTexture("Assets/Mario/RunRight2.png"));
	mPlayerComp->AddAnimation("runRight", runRight);

}