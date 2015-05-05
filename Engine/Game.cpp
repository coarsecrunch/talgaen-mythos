#include "Game.h"
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
		: mCamera(800, 600)
		, mWindow(800, 600)
	{
	}


	int Game::Init(int width, int height, const char* name)
	{
		//Camera::InitRendering(width, height);
		//Window::initGL();

		mCamera.getBox().setW(width);
		mCamera.getBox().setH(height);

		return 0;
	}

	void Game::Update(F32 dt)
	{
		mCamera.update(dt);
		
		/*for (auto obj = mActors.begin(); obj != mActors.end(); ++obj)
		{
			(*(*obj)).Update(dt);
		}*/
	}

	void Game::ResolveCollisions()
	{
		/*
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
		}*/
	}

	void Game::Render(const AssetManager* man)
	{
		/*

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

		*/
	}


	Game::~Game()
	{

	}
}