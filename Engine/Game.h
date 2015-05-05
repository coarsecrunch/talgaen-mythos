#pragma once

#include <vector>
#include <string>
#include <map>

#include "Camera.h"
#include "Cmn.h"
#include "Map.h"
#include "Window.h"

struct GLFWwindow;

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
	protected:
		Camera mCamera;


		bool mRenderCollisions = false;
		Window mWindow;
	};

}