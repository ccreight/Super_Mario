#include "GoombaMove.h"
#include "Game.h"
#include "Goomba.h"
#include <iostream>

GoombaMove::GoombaMove(Goomba* owner) : MoveComponent(owner) {
	mOwner = owner;
	mYSpeed = 0.0f;
	mStompedTime = 0.0f;
	mCurrentTime = 0.0f;
}

void GoombaMove::Update(float deltaTime) {

	// Initial frame that the Goomba gets stomped
	if (mOwner->getStomped() && mStompedTime == 0.0f) {
		mStompedTime = SDL_GetTicks();
		return;
	}

	if (mOwner->getStomped()) {
		mCurrentTime = SDL_GetTicks();
	}

	// Delays death animation
	if (mCurrentTime - mStompedTime > DEATH_TIMER) {
		mOwner->SetState(ActorState::Destroy);
		return;
	}

	Vector2 position = Vector2((float)(mOwner->GetForward().x * GetForwardSpeed() * ((double)deltaTime)), (float)(mYSpeed * ((double)deltaTime)));
	mOwner->SetPosition(mOwner->GetPosition() + position);

	std::vector<Block*> blocks = mOwner->GetGame()->getBlocks();
	bool colliding = false;

	// Checks block collisions
	for (size_t i = 0; i < blocks.size(); i++) {

		Vector2 offset;
		CollSide c = mOwner->getCollision()->GetMinOverlap(blocks[i]->GetCollision(), offset);

		if (c == CollSide::None) {
			continue;
		}

		// On top of block
		if (c == CollSide::Top && mYSpeed > 0.0f) {
			mYSpeed = 0.0f;
		}

		// Falls off map
		if (mOwner->GetPosition().y > MAP_BOUND) {
			mOwner->SetState(ActorState::Destroy);
			return;
		}

		// Hits side of a block
		if (c == CollSide::Left || c == CollSide::Right) {
			SetForwardSpeed(-1 * GetForwardSpeed());
		}

		colliding = true;
		mOwner->SetPosition(mOwner->GetPosition() + offset);

	}

	std::vector<Goomba*> goombas = mOwner->GetGame()->getGoombas();

	// Checks collisions with other Goombas
	for (int i = 0; i < goombas.size(); i++) {

		if (goombas[i] == mOwner) {
			continue;
		}

		Vector2 offset;
		CollSide c = mOwner->getCollision()->GetMinOverlap(goombas[i]->getCollision(), offset);

		if (c == CollSide::None) {
			continue;
		}

		// Reverses velocity upon collision with other Goomba
		if (c == CollSide::Left || c == CollSide::Right) {
			SetForwardSpeed(-1 * GetForwardSpeed());
		}

		mOwner->SetPosition(mOwner->GetPosition() + offset);

	}

	mYSpeed += deltaTime * Y_ACCEL_FACTOR;

}