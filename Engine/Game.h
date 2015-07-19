#pragma once

#include <vector>
#include <string>
#include <functional>
#include <map>

#include "Cmn.h"
#include "Camera.h"
#include "Map.h"
#include "Window.h"
#include "Layer.h"
#include "funkdefs.h"
struct GLFWwindow;
struct cpBody;
struct cpSpace;

#define TALGA_KEYPRESS 1
#define TALGA_KEYRELEASE 2
#define TALGA_KEYCONTINUE 3

namespace talga
{
	class LuaEngine;
	class GameObject;
	class Game;

	class Game
	{
	public:

		Game();
		~Game();

		int Init(int width, int height, const char* name);
		static void LUA_REGISTER(LuaEngine*);

		void update(F32 dt);
		void render();

		Camera& camera() { return mCamera; }

		Window& getWindow() { return mWindow; }

		//void exec();
		void game_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void game_resize_window(GLFWwindow* window, int w, int h);
		
		void addObj(GameObject* obj);
		void removeObj(GameObject* obj);
		void clearObjs();

		AssetManager* getManager() { return &mManager; }

		void addKeyCallback(char c, GameObject* obj, KeyCallbackFunc);

		cpSpace* getSpace() { return mSpace; }

		
	protected:
		std::vector<GameObject*> mGameObjects;
		Camera mCamera;
		cpBody* rectBody;
		Layer mMapLayer;
		Layer mObjectsLayer;
		std::shared_ptr<Renderer> mRenderer;
		cpSpace* mSpace;
		AssetManager mManager;
		
		std::multimap<char, std::pair<GameObject*, KeyCallbackFunc>> mKeyCallbacks;
		GameObject* mPlayer;
		Window mWindow;
	};
	
}