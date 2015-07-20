#include "Game.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "Map.h"
#include "AssetManager.h"
#include "Collision.h"
#include <iostream>

#include "oolua/oolua.h"
#include "GameObject.h"

#include "chipmunk/chipmunk_private.h"
#include "collisiontypes.h"
#include "luareg.h"

namespace talga
{
	const int MAX_GAMEOBJECTS = 3000;

	Game::Game()
		: mCamera(800, 600)
		, mWindow(800, 600)
		, mMapLayer{ nullptr, -1, -1 }
		, mObjectsLayer{ nullptr, -1, -1 }
		, mGameObjects( )
		, mRenderer( nullptr )
		, mManager()
		, mKeyCallbacks()
		, mPlayer{ nullptr }
	{
		mGameObjects.reserve(MAX_GAMEOBJECTS);
	}

	int Game::Init(int width, int height, const char* name)
	{
		mCamera.box().setW(width);
		mCamera.box().setH(height);

		mRenderer = std::shared_ptr<Renderer>(new Renderer{ "../assets/shaders/renderer2d.vert", "../assets/shaders/renderer2d.frag" });
		mManager.AddTexture("../assets/textures/testblock.png");
		mManager.AddTexture("../assets/textures/talgasheet.png");
		mManager.AddMap("../assets/maps/sandboxx.tmap");

		mMapLayer = Layer{ mRenderer, 800, 600 };
		mObjectsLayer = Layer{ mRenderer, 800, 600 };
		
		cpVect gravity = cpv(0, -300);

		mSpace = cpSpaceNew();
		cpSpaceSetGravity(mSpace, gravity);
		cpSpaceSetIterations(mSpace, 10);

		cpShape* ground = cpSegmentShapeNew(mSpace->staticBody, cpv(0, -5 * 32), cpv(20 * 32, -5 * 32), 0);
		cpShapeSetFriction(ground, 0.97f);
		cpShapeSetCollisionType(ground, COLL_MAPGEOM);
		cpSpaceAddShape(mSpace, ground);

		mRenderer->setCamera(&mCamera);

		mMapLayer.add(mManager.GetMap("sandboxx.tmap"));

		return 0;
	}

	void Game::addObj(GameObject* obj)
	{
		obj->GAME = this;
		mGameObjects.push_back(obj);
		mObjectsLayer.add(obj->pmRenderable.get());
		cpSpaceAddBody(mSpace, obj->mBody);
		cpSpaceAddShape(mSpace, obj->mShape);
		
		TALGA_PRVAL(cpShapeGetCollisionType(obj->mShape));

		if (obj->stagedFunc)
			obj->stagedFunc(obj);

		TALGA_MSG("Game object has been added")
	}

	void Game::addKeyCallback(char c, GameObject* obj, KeyCallbackFunc callback)
	{
		mKeyCallbacks.insert(std::pair < char, std::pair<GameObject*, KeyCallbackFunc> > (toupper(c), std::pair<GameObject*, KeyCallbackFunc>(obj, callback) ));
	}

	void Game::removeObj(GameObject* obj)
	{
		for (auto it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
		{

			if (*it == obj)
			{
				if ( (*it)->unstagedFunc )
					(*it)->unstagedFunc(*it);

				cpSpaceRemoveShape(mSpace, (*it)->mShape);
				cpSpaceRemoveBody(mSpace, (*it)->mBody);


				if (it == mGameObjects.end() - 1)
				{
					mObjectsLayer.remove(obj->pmRenderable.get());
					delete *it;
					mGameObjects.pop_back();
				}
				else
				{
					mObjectsLayer.remove(obj->pmRenderable.get());
					delete *it;
					*it = mGameObjects.back();
					mGameObjects.pop_back();
				}
				
				break;
			}
		}
	}

	void Game::clearObjs()
	{
		for (auto it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
		{
			if ((*it)->unstagedFunc)
				(*it)->unstagedFunc(*it);

			delete *it;
		}

		mGameObjects.clear();
	}
	void Game::update(F32 dt)
	{
		cpSpaceStep(mSpace, dt / 1000.0f);
		mCamera.update(dt);

		for (auto it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
		{
			(*it)->update(dt);
			if ((*it)->DESTROY)
			{
				removeObj(*it);
				if (it != mGameObjects.begin())
					--it;
			}
		}
	}


	void Game::render()
	{
		mMapLayer.getRenderer()->tStackPush(mCamera.getCameraMat());
		mMapLayer.render();
		mObjectsLayer.render();
		mMapLayer.getRenderer()->tStackPop();
	}

	// currently relies on the fact that glfw defines keys as their ascii value, probably should define my own constants
	void Game::game_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto range = mKeyCallbacks.equal_range(key);

		int keyaction = -1;
		
		switch (action)
		{
		case GLFW_PRESS:
			keyaction = TALGA_KEYPRESS; break;
		case GLFW_RELEASE:
			keyaction = TALGA_KEYRELEASE; break;
		default:
			keyaction = -1; break;
		}
		
		for (auto it = range.first; it != range.second; ++it)
		{
			(*it).second.second(it->second.first, keyaction);
		}
	}

	void Game::game_resize_window(GLFWwindow* window, int w, int h)
	{
		mObjectsLayer.setProjectionMatrix(w, h);
		mMapLayer.setProjectionMatrix(w, h);
		mCamera.setW(w);
		mCamera.setH(h);
		glViewport(0, 0, w, h);
	}

	Game::~Game()
	{
		cpSpaceFree(mSpace);
		clearObjs();
		mMapLayer.clear();
		mObjectsLayer.clear();
	}

	

}

