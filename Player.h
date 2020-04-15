#pragma once

#include "Actor.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "MoveComponent.h"
#include <string>

class Player : public Actor {

	public:
		Player(class Game* game);
		CollisionComponent* getCollision() {
			return mPlayerCollision;
		}
		void SetTexture(std::string s);
		void InitAnims();

		AnimatedSprite* getAnimComp() {
			return mPlayerComp;
		}


	private:
		AnimatedSprite* mPlayerComp;
		class Game* mGame;
		CollisionComponent* mPlayerCollision;
		MoveComponent* mPlayerMoveComp;
		class PlayerMove* mPlayerMove;
		std::vector<SDL_Texture*> deadAnim;
		std::vector<SDL_Texture*> idleAnim;
		std::vector<SDL_Texture*> jumpLeft;
		std::vector<SDL_Texture*> jumpRight;
		std::vector<SDL_Texture*> runLeft;
		std::vector<SDL_Texture*> runRight;

};