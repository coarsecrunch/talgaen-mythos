#include "Cmn.h"
#include "Rendering.h"
#include "Map.h"
#include "Game.h"
#include "Texture.h"
#include "Collision.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include <vector>
#include <iostream>
#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"

#include "AssetManager.h"
#include "Clock.h"
#include "Rect.h"
#include "ParticleEmitter.h"
#include "LuaEngine.h"
#include "GameObject.h"

#include "luareg.h"

#include "Renderer.h"
#include "Layer.h"
#include "Sprite.h"
#include "AnimSet.h"
#include "Math/RandomGen.h"
#include "sys.h"
#include "chipmunk/chipmunk_private.h"

#include "collisiontypes.h"

const talga::I32 WIDTH = 800;
const talga::I32 HEIGHT = 600;
static talga::Game* GAME = nullptr;
static talga::LuaEngine* LUA_ENGINE = nullptr;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GAME->game_key_callback(window, key, scancode, action, mods);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	GAME->game_resize_window(window, w, h);
}

talga::Game* getGame()
{
	return GAME;
}
static void LUA_REGISTER(talga::LuaEngine* engine)
{

}

int main(int argc, char** argv)
{
	TALGA_MSG("working directory: " + talga::getWorkingDirectory());
	talga::Clock clock;
	clock.Init();

	GAME = new talga::Game();
	GAME->Init(WIDTH, HEIGHT, "hello talga");
	
	//talga::Rectangle::LUA_REGISTER(talga::LuaEngine::instance());
	//talga::GameObject::LUA_REGISTER(talga::LuaEngine::instance());
	//talga::Camera::LUA_REGISTER(talga::LuaEngine::instance());
	//talga::Game::LUA_REGISTER(talga::LuaEngine::instance());
	//talga::UpdateFunc::LUA_REGISTER(talga::LuaEngine::instance());
	//talga::StagedFunc::LUA_REGISTER(talga::LuaEngine::instance());
	//LUA_REGISTER(talga::LuaEngine::instance());
	//OOLUA::LUA_REGISTER_TYPES();

	//talga::LuaEngine::instance()->push(GAME);

	talga::LuaEngine::instance()->ExecuteFile("../assets/scripts/script.lua");

	talga::U32 previousTime = 0;
	talga::U32 dt = 0;
	talga::U32 fps = 0;
	talga::U32 timeSince = 0;
	
	talga::AnimationSet set{ GAME->getManager()->GetTexture("talgasheet.png") };

	std::vector<talga::Rect> talgaStandL{ { talga::Rect{ 0, 0, 64, 64 }, talga::Rect{ 64, 0, 64, 64 } } };
	std::vector<talga::Rect> talgaStandR{ { talga::Rect{ 128, 0, 64, 64 }, talga::Rect{ 192, 0, 64, 64 } } };
	std::vector<talga::Rect> talgaWalkL;
	std::vector<talga::Rect> talgaWalkR;
	for (int i = 0; i < 6; ++i)
	{
		talgaWalkL.push_back(talga::Rect{ 64 * i, 64, 64, 64 });
	}
	for (int i = 6; i < 12; ++i)
	{
		talgaWalkR.push_back(talga::Rect{ 64 * i, 64, 64, 64 });
	}

	set.addAnim("talgaStandL", talgaStandL);
	set.addAnim("talgaStandR", talgaStandR);
	set.addAnim("talgaWalkL", talgaWalkL);
	set.addAnim("talgaWalkR", talgaWalkR);

	talga::Sprite* block = new talga::Sprite(GAME->getManager()->GetTexture("testblock.png"));
	talga::AnimSprite* spr = new talga::AnimSprite{ &set };
	
	talga::cpMap testMap = GAME->getManager()->GetMap("sandboxx.tmap");

	spr->playAnimation("talgaStandL", 1000, true);

	auto onTalgaHitGround = [](talga::GameObject* obj)
	{
		std::cout << "TALGA HIT THE GROUND!! YQYQYQ!" << std::endl;
	};

	auto onKeyPressA = [](talga::GameObject* obj, int action)
	{
		if (obj->getVx() > -100.0f)
		{
			obj->applyImpulseX(-1000.0f);
		}
	};

	auto onKeyPressD = [](talga::GameObject* obj, int action)
	{
		if (obj->getVx() < 100.0f)
		{
			obj->applyImpulseX(1000.0f);
		}
	};

	auto talgaInit = [=](talga::GameObject* obj)
	{
		obj->setCollisionType(talga::COLL_PLAYER);
		obj->addCollisionCallback(talga::COLL_MAPGEOM, onTalgaHitGround);
		obj->addKeyCallback('A', onKeyPressA);
		obj->addKeyCallback('D', onKeyPressD);

	};

	
	talga::GameObject* tga = new talga::GameObject(spr, talgaInit);
	talga::GameObject* blk = new talga::GameObject(block);

	//tga->loadScript("../assets/scripts/script.lua", talga::LuaEngine::instance());

	GAME->addObj(tga);
	GAME->addObj(blk);

	

	glfwSetWindowSizeCallback(GAME->getWindow().getWindow(), resize_callback);
	glfwSetKeyCallback(GAME->getWindow().getWindow(), key_callback);


	while (!glfwWindowShouldClose(GAME->getWindow().getWindow()))
	{
		GAME->getWindow().swap();
		GAME->getWindow().clear();

		glfwPollEvents();
		dt = clock.TimePassed() - previousTime;
		timeSince += dt;
		previousTime = clock.TimePassed();

		if (timeSince >= 1000)
		{
			timeSince = 0;
			std::cout << std::endl << "FPS: " << fps << std::endl;
			fps = 0;
		}

		GAME->update(dt);
		GAME->render();

		++fps;
	}

	delete GAME;

	return 0;
}
