#include "Game.h"
#include "SDL2\SDL.h"
#include "SDL2\SDL_image.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "Map.h"
#include "AssetManager.h"
#include "Collision.h"
#include <iostream>

#include "LuaEngine.h"
#include "LuaBridge\LuaBridge.h"

namespace talga
{
	const int MAX_GAMEOBJECTS = 1000;

	void Game::LUA_REGISTER(LuaEngine* engine)
	{
		using namespace luabridge;
		getGlobalNamespace(engine->getState())
			.beginNamespace("Engine")
			.beginClass<Game>("Game")
			.addConstructor<void(*)(void)>()
			.endClass()
			.endNamespace();
	}

	Game::Game()
		: mCamera(this)
		, mKeyDownHandlers()
		, mKeyUpHandlers()
		, mActors()
		, mStaticActors()
	{
	}

	void Game::DestroyActor(I32 location)
	{
		mActors[location] = mActors.back();
		mActors[location]->setListLoc(location);
		mActors.pop_back();
	}

	int Game::Init(int width, int height, const char* name)
	{
		Camera::InitRendering(width, height);
		mCamera.setW(width);
		mCamera.setH(height);

		mActors.reserve(MAX_GAMEOBJECTS);
		mStaticActors.reserve(MAX_GAMEOBJECTS);

		return 0;
	}

	void Game::Input(SDL_Event* e)
	{
		//char c = e->key.keysym.sym;
		if (mKeyDownHandlers.size() > 0)
		{
			(*(mKeyDownHandlers.find('k'))).second();
		}

		//(*mKeyUpHandlers.find(c)).second();
	}

	void Game::Update(F32 dt)
	{
		mCamera.Update(dt);
		for (auto obj = mActors.begin(); obj != mActors.end(); ++obj)
		{
			(*(*obj)).Update(dt);
		}
	}

	void Game::ResolveCollisions()
	{
		for (auto obj = mActors.begin(); obj != mActors.end(); ++obj)
		{
			if (!(**obj).checkCollisions())
				continue;

			for (auto staticObj = mStaticActors.begin(); staticObj != mStaticActors.end(); ++staticObj)
			{
				FindResCollision(**obj, (**staticObj).getCollider());
			}
		}

		for (auto obj = mActors.begin(); obj != mActors.end(); ++obj)
		{
			if (!(**obj).checkCollisions())
				continue;

			for (auto obj2 = obj + 1; obj2 != mActors.end(); ++obj2)
			{
				if (!(**obj2).checkCollisions())
					continue;
				ResMovingMovingCol(**obj, **obj2);
			}
		}
	}

	void Game::Render(const AssetManager* man)
	{
		Camera::Clear();


		for (auto rdrObj = mActors.begin(); rdrObj != mActors.end(); ++rdrObj)
		{
			if (!(*rdrObj)->isVisible()) continue;

			(**rdrObj).Render(mCamera);

			if (mRenderCollisions)
				mCamera.RenderCollider(**rdrObj);

		}

		if (mRenderCollisions)
		{
			for (auto colliderObj = mStaticActors.begin(); colliderObj != mStaticActors.end(); ++colliderObj)
			{
				mCamera.RenderCollider(**colliderObj);
			}
		}


	}

	void Game::AddSpr(PhysSprite* spr)
	{
		mActors.push_back(spr);
		//return mActors.back();
	}

	void Game::AddStaticSpr(PhysSprite* spr)
	{
		mStaticActors.push_back(spr);
		//return mStaticActors.back();
	}

	void Game::AddKeyDownHandler(char c, void(*perform)())
	{
		mKeyDownHandlers.insert(std::pair<char, void(*)()>(c, perform));

	}

	void Game::RemoveKeyDownHandler(char c)
	{
		mKeyDownHandlers.erase(c);
	}

	void Game::AddKeyUpHandler(char c, void(*perform)())
	{
		mKeyUpHandlers.insert(std::pair<char, void(*)()>(c, perform));
	}

	void Game::RemoveKeyUpHandler(char c)
	{
		mKeyUpHandlers.erase(c);
	}

	Game::~Game()
	{

	}
}