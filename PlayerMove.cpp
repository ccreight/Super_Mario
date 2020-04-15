#include "PlayerMove.h"
#include "Game.h"

PlayerMove::PlayerMove(Player* actor) : MoveComponent(actor) {
	mOwner = actor;
	mYSpeed = 0.0f;
	mSpacePressed = false;
	mInAir = false;
}

void PlayerMove::Update(float deltaTime) {

	Vector2 position = Vector2((float)(.5 * mOwner->GetForward().x * GetForwardSpeed() * ((double)deltaTime)), (float)(mYSpeed * ((double)deltaTime)));
	mOwner->SetPosition(mOwner->GetPosition() + position);

	// Player falls off map
	if (mOwner->GetPosition().y > MAP_OFF) {

		SetAnim("dead");
		mOwner->SetState(ActorState::Paused);
		Mix_HaltChannel(mOwner->GetGame()->getChannel());
		Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Dead.wav"), 0);
		return;

	}
	
	// Player wins
	if (mOwner->GetPosition().x > MAP_END) {

		mOwner->SetState(ActorState::Paused);
		Mix_HaltChannel(mOwner->GetGame()->getChannel());
		Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/StageClear.wav"), 0);
		return;

	}

	std::vector<Goomba*> goombas = mOwner->GetGame()->getGoombas();

	// Checking Goomba collisions
	for (size_t i = 0; i < goombas.size(); i++) {

		Vector2 offset;
		CollSide c = mOwner->getCollision()->GetMinOverlap(goombas[i]->getCollision(), offset);

		if (c == CollSide::None) {
			continue;
		}

		// Stomp on a Goomba
		if (c == CollSide::Top || (mInAir && (c == CollSide::Left || c == CollSide::Right))) {

			Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Stomp.wav"), 0);
			if(goombas[i] != nullptr && goombas[i]->GetState() != ActorState::Destroy)
			goombas[i]->Destroy((float)SDL_GetTicks());
			mYSpeed = STOMP_ACCELERATION;
			mInAir = true;
			SetAnim("idle");
			
		}

		else {
			// Run into a Goomba
			if (!goombas[i]->getStomped()) {
				SetAnim("dead");
				mOwner->SetState(ActorState::Paused);
				Mix_HaltChannel(mOwner->GetGame()->getChannel());
				Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Dead.wav"), 0);
				return;
			}
		}

	}

	std::vector<Block*> blocks = mOwner->GetGame()->getBlocks();
	bool colliding = false;

	// Checking block collisions
	for (size_t i = 0; i < blocks.size(); i++) {

		Vector2 offset;
		CollSide c = mOwner->getCollision()->GetMinOverlap(blocks[i]->GetCollision(), offset);

		if (c == CollSide::None) {
			continue;
		}

		// On top of a block
		if (c == CollSide::Top && mYSpeed > 0.0f) {
			
			mYSpeed = 0.0f;

			if (GetForwardSpeed() > 0) {
				SetAnim("runRight");
			}
			else if (GetForwardSpeed() < 0) {
				SetAnim("runLeft");
			}
			else {
				SetAnim("idle");
			}
		}

		// Hit head on a block
		if (c == CollSide::Bottom && mYSpeed < 0.0f) {
			
			mYSpeed = 0.0f;
			Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Bump.wav"), 0);

			if (GetForwardSpeed() > 0) {
				SetAnim("jumpRight");
			}
			else if (GetForwardSpeed() < 0) {
				SetAnim("jumpLeft");
			}
			else {
				SetAnim("idle");
			}

		}

		colliding = true;
		mInAir = false;
		mOwner->SetPosition(mOwner->GetPosition() + offset);

	}

	// In the air
	if (!colliding) {
		mInAir = true;
		if (GetForwardSpeed() > 0) {
			SetAnim("jumpRight");
		}
		else if (GetForwardSpeed() < 0) {
			SetAnim("jumpLeft");
		}
	}

	mYSpeed += deltaTime * Y_ACCELERATION;

	if (GetForwardSpeed() == 0) {
		SetAnim("idle");
	}

	// Camera adjustments
	if (mOwner->GetPosition().x < mOwner->GetGame()->getCameraPos().x) {
		mOwner->SetPosition(Vector2(mOwner->GetGame()->getCameraPos().x, mOwner->GetPosition().y));
	}

	if (mOwner->GetPosition().x - LEFT_BOUND > 0 && mOwner->GetPosition().x - LEFT_BOUND > mOwner->GetGame()->getCameraPos().x) {
		mOwner->GetGame()->setCameraPos(Vector2(mOwner->GetPosition().x - LEFT_BOUND, mOwner->GetGame()->getCameraPos().y));
	}

}

void PlayerMove::SetAnim(std::string str) {

	AnimatedSprite* animComp = mOwner->getAnimComp();
	animComp->SetAnimation(str);

}

void PlayerMove::ProcessInput(const Uint8* keyState) {

	// Handles movement of the player left and right
	if (keyState[SDL_SCANCODE_LEFT]) {
		SetForwardSpeed((float)(-1 * MOVE_SPEED));
	}
	else if (keyState[SDL_SCANCODE_RIGHT]) {
		SetForwardSpeed((float)(MOVE_SPEED));
	}
	else {
		SetForwardSpeed(0);
	}

	// Handles jumping
	if (!mInAir) {

		if (keyState[SDL_SCANCODE_SPACE] && !mSpacePressed) {
			mYSpeed = STOMP_ACCELERATION * 2;
			mSpacePressed = true;
			mInAir = true;
			Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
		}

		if (!keyState[SDL_SCANCODE_SPACE]) {
			mSpacePressed = false;
		}

	}

}