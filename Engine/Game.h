#pragma once

#include <vector>
#include <string>
#include <map>

#include "Camera.h"
#include "Cmn.h"
#include "Map.h"
#include "Window.h"
#include "Layer.h"

struct GLFWwindow;
struct cpBody;
namespace talga
{
	class LuaEngine;
	
	class Game
	{
	public:
		static void LUA_REGISTER(LuaEngine*);

		enum KEYTYPE { KEY_DOWN, KEY_UP };
		Game();
		~Game();

		int Init(int width, int height, const char* name);

		void Update(F32 dt);
		void ResolveCollisions();
		void Render(const AssetManager* man);

		Camera& getCamera() { return mCamera; } // XXX

		Window& getWindow() { return mWindow; }

		void setRenderCollisions(bool value) { mRenderCollisions = value; }
		bool isRenderingCollisions() const { return mRenderCollisions; }

		void exec();
		void game_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void game_resize_window(GLFWwindow* window, int w, int h);
	protected:
		Camera mCamera;
		cpBody* rectBody;
		Layer layer;


		bool mRenderCollisions = false;
		Window mWindow;
	};

}