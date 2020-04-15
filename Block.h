#pragma once

#include "Actor.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"

class Block : public Actor {

	public:
		Block(class Game* game);
		void SetTexture(SDL_Texture* t);

		void SetType(char t);
		char GetType();

		CollisionComponent* GetCollision() {
			return mBlockCollision;
		}
		~Block();

	private:
		SpriteComponent* mBlockComp;
		class Game* mGame;
		CollisionComponent* mBlockCollision;
		char type;

};