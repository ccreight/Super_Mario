#pragma once
#include "MoveComponent.h"
#include <SDL/SDL_image.h>
#include "Player.h"

class PlayerMove : public MoveComponent {

	public:
		PlayerMove(Player* owner);
		void Update(float deltaTime);
		void ProcessInput(const Uint8* keyState);
		void SetAnim(std::string str);

	private:
		const float LEFT_BOUND = 300.0f;
		const float MOVE_SPEED = 300.0f;
		const float STOMP_ACCELERATION = -350.0f;
		const float Y_ACCELERATION = 2000.0f;
		const float MAP_END = 6368.0f;
		const float MAP_OFF = 448.0f;
		Player* mOwner;
		float mYSpeed;
		bool mSpacePressed, mInAir;

};