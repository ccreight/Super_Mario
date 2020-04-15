#include "Block.h"
#include "Game.h"
#include <iostream>

Block::Block(class Game* game) : Actor(game) {
	mGame = game;
	mBlockComp = new SpriteComponent(this);
	mBlockCollision = new CollisionComponent(this);

	mBlockCollision->SetSize(32, 32);
	mGame->AddBlock(this);

	type = ' ';
}

Block::~Block() {
	mGame->RemoveBlock(this);
}

void Block::SetTexture(SDL_Texture* t) {
	mBlockComp->SetTexture(t);
}

char Block::GetType() {
	return type;
}

void Block::SetType(char t) {
	type = t;
}