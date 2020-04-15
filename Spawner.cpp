#include "Spawner.h"
#include "Game.h"

Spawner::Spawner(class Game* game) : Actor(game){
	mGame = game;
}

void Spawner::OnUpdate(float deltaTime) {

	// Creates Goomba if player is within range
	if (abs(mGame->getPlayer()->GetPosition().x - this->GetPosition().x) < IN_RANGE) {
		Goomba* g = new Goomba(mGame);
		g->SetPosition(this->GetPosition());
		this->SetState(ActorState::Destroy);
	}

}