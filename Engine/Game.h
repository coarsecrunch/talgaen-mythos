#pragma once

#include <vector>
#include <string>
#include <functional>
#include <map>

#include "Camera.h"
#include "Cmn.h"
#include "Map.h"
#include "Window.h"
#include "Layer.h"
#include "IDynamic.h"
#include "IRenderable.h"
#include "oolua/oolua.h"
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

	typedef std::function<void(GameObject*, int)> KeyCallback;

	class Game : public IDynamic
	{
	public:

		Game();
		~Game();

		int Init(int width, int height, const char* name);
		static void LUA_REGISTER(LuaEngine*);

		virtual void update(F32 dt) override;
		virtual void render();

		const Camera& getCamera() const { return *mCamera; }
		void setCamera(const Camera& value) { *mCamera = value; }

		Window& getWindow() { return mWindow; }

		//void exec();
		void game_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void game_resize_window(GLFWwindow* window, int w, int h);
		
		void addObj(GameObject* obj);
		void removeObj(GameObject* obj);
		void clearObjs();

		AssetManager* getManager() { return &mManager; }

		void addKeyCallback(char c, GameObject* obj, KeyCallback);

		cpSpace* getSpace() { return mSpace; }

		void printJelly() const;
	protected:
		std::vector<GameObject*> mGameObjects;
		Camera* mCamera;
		cpBody* rectBody;
		Layer mMapLayer;
		Layer mObjectsLayer;
		std::shared_ptr<Renderer> mRenderer;
		cpSpace* mSpace;
		AssetManager mManager;
		
		std::multimap<char, std::pair<GameObject*, KeyCallback>> mKeyCallbacks;
		GameObject* mPlayer;
		Window mWindow;
	};
	
}