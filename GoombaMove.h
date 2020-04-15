#pragma once
#include "MoveComponent.h"

class GoombaMove : public MoveComponent {

	public:
		GoombaMove(class Goomba* owner);
		void Update(float deltaTime);

	private:
		const float MOVE_SPEED = 300.0f;
		const float Y_ACCELERATION = 2000.0f;
		const int DEATH_TIMER = 250;
		const float MAP_BOUND = 448.0f;
		const int Y_ACCEL_FACTOR = 2000.0f;
		class Goomba* mOwner;
		float mYSpeed;
		float mStompedTime, mCurrentTime;

};