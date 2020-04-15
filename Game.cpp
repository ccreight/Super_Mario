//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <iostream>
#include <stdlib.h> 
#include <time.h> 
#include <algorithm>
#include <SDL/SDL_image.h>
#include "Random.h"
#include "Block.h"
#include <fstream>



Game::Game() {

	mLeaveGame = false;
	mWindow = nullptr;
	mRenderer = nullptr;
	mPreviousTime = -1;
	mPlayer = nullptr;
	mThemeChannel = 0;

}

bool Game::Initialize() {

	Random::Init();

	// https://wiki.libsdl.org/SDL_Init#Remarks
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0) {
		std::cout << "Issue initializing SDL" << std::endl;
		return 0;
	}

	// https://wiki.libsdl.org/SDL_CreateWindow
	mWindow = SDL_CreateWindow("Super Mario", 100, 100, (int)WINDOW_WIDTH, (int)WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

	// https://wiki.libsdl.org/SDL_CreateRenderer
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	IMG_Init(IMG_INIT_PNG);
	LoadData();

	mPreviousTime = (float)SDL_GetTicks();

	return 1;

}

void Game::RunLoop() {

	mLeaveGame = 0;

	while (!mLeaveGame) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}

}

void Game::ProcessInput() {

	SDL_Event event;

	// https://wiki.libsdl.org/SDL_PollEvent
	while (SDL_PollEvent(&event)) {

		// Checks for escape condition
		if (event.type == SDL_QUIT) {
			mLeaveGame = true;
		}

	}

	// Checks if ESC was pressed
	const Uint8* state = SDL_GetKeyboardState(NULL);

	// Processing input
	std::vector<Actor*> tempActors = mActors;

	for (unsigned int i = 0; i < tempActors.size(); i++) {
		tempActors[i]->ProcessInput(state);
	}

	if (state[SDL_SCANCODE_ESCAPE]) {
		mLeaveGame = true;
	}

}

void Game::UpdateGame() {

	float currentTime = (float)SDL_GetTicks();

	// Keeping the frame rate around 16
	while (currentTime - mPreviousTime < 16) {
		currentTime = (float)SDL_GetTicks();
	}

	float deltaTime = currentTime - mPreviousTime;

	// Edge case handling
	if (deltaTime > 33) {
		deltaTime = 33;
	}

	std::vector<Actor*> actorsCopy = mActors;
	deltaTime /= 1000;

	// Updates the actors
	for (unsigned int i = 0; i < actorsCopy.size(); i++) {
		actorsCopy[i]->Update(deltaTime);
	}

	std::vector<Actor*> tempDestroy;

	// Preps necessary actors for deletion
	for (unsigned int i = 0; i < actorsCopy.size(); i++) {

		if (actorsCopy[i]->GetState() == ActorState::Destroy)
		{
			tempDestroy.push_back(actorsCopy[i]);
		}

	}

	for (unsigned int i = 0; i < tempDestroy.size(); i++) {
		delete tempDestroy[i];
	}

	mPreviousTime = (float)SDL_GetTicks();

}

void Game::GenerateOutput() {

	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
	SDL_RenderClear(mRenderer);
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// Draws visible sprites
	for (unsigned int i = 0; i < mSprites.size(); i++) {
		if (mSprites[i]->IsVisible()) {
			mSprites[i]->Draw(mRenderer);
		}
	}

	SDL_RenderPresent(mRenderer);

}

void Game::AddActor(Actor* a) {
	mActors.push_back(a);
}

void Game::RemoveActor(Actor* a) {

	std::vector<Actor*>::iterator iter;
	iter = find(mActors.begin(), mActors.end(), a);
	mActors.erase(iter);

}

void Game::AddBlock(Block* b) {
	mBlocks.push_back(b);
}

void Game::RemoveBlock(Block* b) {
	std::vector<Block*>::iterator iter;
	iter = find(mBlocks.begin(), mBlocks.end(), b);
	mBlocks.erase(iter);
}

void Game::LoadData() {

	mThemeChannel = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);

	// Initializes the walls backdrop
	Actor* walls = new Actor(this);
	Vector2 wallPos = Vector2(3392, 224);
	walls->SetPosition(wallPos);
	SpriteComponent* wallComp = new SpriteComponent(walls);
	wallComp->SetTexture(GetTexture("Assets/Background.png"));

	std::string line;
	std::ifstream myfile("Assets/Level1.txt");

	int verticalIndex = 16, horizontalIndex = 16;

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{

			for (unsigned int i = 0; i < line.length(); i++) {

				// Instantiates the blocks
				if (line[i] != '.' && line[i] != 'P' && line[i] != 'Y') {
					Block* b1 = new Block(this);
					b1->SetPosition(Vector2((float)horizontalIndex, (float)verticalIndex));
					std::string blockType = "Assets/Block" + std::string(1, line[i]) + ".png";
					b1->SetTexture(GetTexture(blockType));
					b1->SetType(line[i]);
				}

				// Instantiates player
				if (line[i] == 'P') {
					mPlayer = new Player(this);
					mPlayer->SetPosition(Vector2((float)horizontalIndex, (float)verticalIndex));
				}

				// Instantiates spawner
				if (line[i] == 'Y') {
					Spawner* s = new Spawner(this);
					s->SetPosition(Vector2((float)horizontalIndex, (float)verticalIndex));
				}

				horizontalIndex += (int)BLOCK_WIDTH;

			}

			horizontalIndex = 16;
			verticalIndex += (int)BLOCK_HEIGHT;

		}
		myfile.close();
	}

}

// Clears game data
void Game::UnloadData() {

	while (!mActors.empty()) {
		delete mActors.back();
	}

	for (std::pair<std::string, SDL_Texture*> element : mTextures) {
		SDL_DestroyTexture(element.second);
	}

	for (std::pair<std::string, Mix_Chunk*> sound : mSounds) {
		Mix_FreeChunk(sound.second);
	}

	mTextures.clear();

}

SDL_Texture* Game::GetTexture(std::string filename) {

	std::unordered_map<std::string, SDL_Texture*>::iterator iter = mTextures.find(filename);

	// Searches for pre-existing texture or adds it to the map of textures
	if (iter == mTextures.end()) {
		SDL_Surface* surface = IMG_Load(filename.c_str());

		if (surface == nullptr) {
			SDL_Log("Failed to load image %s \n", filename.c_str());
			return nullptr;
		}

		// Adding the texture to the map
		SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
		SDL_FreeSurface(surface);
		mTextures[filename] = texture;
		return texture;
	}
	else {
		return mTextures.at(filename);
	}

}

// Adds a sprite to the game
void Game::AddSprite(SpriteComponent* s) {

	mSprites.push_back(s);

	std::sort(mSprites.begin(), mSprites.end(),
		[](SpriteComponent* a, SpriteComponent* b) {
			return a->GetDrawOrder() < b->GetDrawOrder();
		});

}

void Game::RemoveSprite(SpriteComponent* s) {

	std::vector<SpriteComponent*>::iterator iter;
	iter = find(mSprites.begin(), mSprites.end(), s);
	mSprites.erase(iter);

}

void Game::AddGoomba(Goomba* g) {

	mEnemies.push_back(g);

}

void Game::RemoveGoomba(Goomba* g) {

	std::vector<Goomba*>::iterator iter;
	iter = find(mEnemies.begin(), mEnemies.end(), g);
	mEnemies.erase(iter);

}

Mix_Chunk* Game::GetSound(const std::string& filename) {

	std::unordered_map<std::string, Mix_Chunk*>::iterator iter = mSounds.find(filename);

	// Searches for pre-existing texture or adds it to the map of textures
	if (iter == mSounds.end()) {
		Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());

		if (sound == nullptr) {
			SDL_Log("Failed to load image %s \n", filename.c_str());
			return nullptr;
		}

		// Adding the texture to the map
		mSounds[filename] = sound;
		return sound;
	}
	else {
		return mSounds.at(filename);
	}

}

void Game::Shutdown() {

	UnloadData();
	IMG_Quit();

	Mix_CloseAudio();

	// Shutdown the SDL components
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	atexit(SDL_Quit);

}