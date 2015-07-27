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
struct cpShape;

const int TALGA_KEYPRESS = 1;
const int TALGA_KEYRELEASE = 2;
const int TALGA_KEYCONTINUE = 3;

namespace talga
{
	class LuaEngine;
	class GameObject;
	class Game;
	class LuaDebugPrompt;

	class Game
	{
	public:
		Game();
		~Game();

		int Init(int width, int height, const char* name);

		void update(F32 dt);
		void render();

		Camera& camera() { return mCamera; }
		Window& getWindow() { return mWindow; }
		AssetManager* manager() { return &mManager; }
		cpSpace* getSpace() { return mSpace; }

		void addObj(GameObject* obj);
		void removeObj(GameObject* obj);
		void clearObjs();
		
		void game_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void game_resize_window(GLFWwindow* window, int w, int h);
		void game_mouse_press_callback(GLFWwindow* window, int button, int action, int mods);
		void game_mouse_move_callback(GLFWwindow* window, double x, double y);


		void addKeyCallback(char c, GameObject* obj, KeyCallbackFunc);
		
		void printToLuaPrompt(const std::string& str);
		void printToLuaPrompt(float str);
		void loadmap(const std::string& path);
		void clearMap();
	protected:
		std::vector<GameObject*> mGameObjects;
		Camera mCamera;
		cpBody* rectBody;
		Layer mMapLayer;
		Layer mObjectsLayer;
		Layer mUILayer;
		std::shared_ptr<Renderer> mRenderer;
		cpSpace* mSpace;
		AssetManager mManager;
		Map mCurrentMap;
		std::multimap<char, std::pair<GameObject*, KeyCallbackFunc>> mKeyCallbacks;
		GameObject* mPlayer;
		Window mWindow;
		LuaDebugPrompt* mPrompt;
		std::vector< std::pair<cpBody*, cpShape*> > mSceneGeom;

		double mMouseX;
		double mMouseY;
		I32 mWidth;
		I32 mHeight;
		bool mPromptIsSelected;
	};
	
}