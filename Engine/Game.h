#pragma once

#include <vector>
#include <string>
#include <map>

#include "ARenderableObj.h"
#include "PhysSprite.h"
#include "Character.h"
#include "Camera.h"
#include "Cmn.h"
#include "Map.h"

struct GLFWwindow;
union SDL_Event;

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


		void Input(SDL_Event* e);
		void Update(F32 dt);
		void ResolveCollisions();
		void Render(const AssetManager* man);
		void Destroy();
		void DestroyActor(I32 location);
		void AddKeyDownHandler(char c, void(*perform)());
		void RemoveKeyDownHandler(char c);
		void AddKeyUpHandler(char c, void(*perform)());
		void RemoveKeyUpHandler(char c);
		void AddSpr(PhysSprite* spr);
		void AddStaticSpr(PhysSprite* spr);

		void DeleteItem();

		Camera& getCamera() { return mCamera; } // XXX
		GLFWwindow* getWindow() { return mCamera.mWindow; }

	private:
		Camera mCamera;

		std::vector<PhysSprite*> mStaticActors;
		std::vector<PhysSprite*> mActors;

		std::multimap<char, void(*)()> mKeyDownHandlers;
		std::multimap<char, void(*)()> mKeyUpHandlers;

	public:
		void setRenderCollisions(bool value) { mRenderCollisions = value; }
		bool isRenderingCollisions() const { return mRenderCollisions; }
	private:
		bool mRenderCollisions = false;

	};

}