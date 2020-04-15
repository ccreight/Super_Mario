#pragma once
#include "SDL/SDL.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "Actor.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Random.h"
#include "Block.h"
#include "Player.h"
#include "Goomba.h"
#include "Spawner.h"
#include "SDL/SDL_mixer.h"


class Asteroid;

class Game {

public:

	Game();

	/*
		Initializes game.
		Returns true if the game successfully initializes and false otherwise.
	*/
	bool Initialize();

	// Shuts down SDL systems
	void Shutdown();

	// Runs the game loop.
	void RunLoop();

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void AddActor(Actor* a);
	void RemoveActor(Actor* a);

	SDL_Texture* GetTexture(std::string filename);

	void AddSprite(SpriteComponent* s);
	void RemoveSprite(SpriteComponent* s);

	void AddBlock(Block* b);
	void RemoveBlock(Block* b);

	void AddGoomba(Goomba* g);
	void RemoveGoomba(Goomba* g);

	std::vector<Block*> getBlocks() {
		return mBlocks;
	}

	void setCameraPos(Vector2 v) {
		mCameraPos = v;
	}

	Vector2 getCameraPos() {
		return mCameraPos;
	}

	const float WINDOW_HEIGHT = 448;
	const float WINDOW_WIDTH = 600;

	const float WALL_WIDTH = 32;

	Player* getPlayer() {
		return mPlayer;
	}

	std::vector<Goomba*> getGoombas() {
		return mEnemies;
	}

	Mix_Chunk* GetSound(const std::string& filename);

	int getChannel() {
		return mThemeChannel;
	}

protected:

	const float BLOCK_HEIGHT = 32;
	const float BLOCK_WIDTH = 32;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	bool mLeaveGame;
	float mPreviousTime;
	
	std::vector<Actor*> mActors;
	std::vector<SpriteComponent*> mSprites;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::vector<Block*> mBlocks;
	std::vector<Goomba*> mEnemies;

	std::unordered_map<std::string, Mix_Chunk*> mSounds;
	int mThemeChannel;

	void LoadData();
	void UnloadData();

	Player* mPlayer;
	Vector2 mCameraPos;

};
