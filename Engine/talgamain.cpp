#include "luareg.h"
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

#include "Renderer.h"
#include "Layer.h"
#include "Sprite.h"
#include "label.h"
#include "AnimSet.h"
#include "Math/RandomGen.h"
#include "sys.h"
#include "chipmunk/chipmunk_private.h"

#include "collisiontypes.h"

const talga::I32 WIDTH = 1200;
const talga::I32 HEIGHT = 900;
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

int main(int argc, char** argv)
{
	TALGA_MSG("working directory: " + talga::getWorkingDirectory());
	talga::Clock clock;
	clock.Init();

	GAME = new talga::Game();
	GAME->Init(WIDTH, HEIGHT, "hello talga");
	
	LUA_REGISTER_TYPES();

	talga::LuaEngine::instance()->setGame(GAME);
	talga::LuaEngine::instance()->addGlobal("GAME", GAME);
	talga::AnimationSet set{ GAME->getManager()->GetTexture("talgasheet.png") };
	talga::LuaEngine::instance()->ExecuteStr("print = function(str) GAME:printToLuaPrompt(str) end");

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

	talga::AnimSprite* spr = new talga::AnimSprite{ &set };

	talga::cpMap testMap = GAME->getManager()->GetMap("sandboxx.tmap");

	spr->playAnimation("talgaStandL", 1000, true);

	talga::GameObject* tga = new talga::GameObject(spr);

	tga->loadScript("../assets/scripts/script.lua");
	
	GAME->addObj(tga);
	
	glfwSetWindowSizeCallback(GAME->getWindow().getWindow(), resize_callback);
	glfwSetKeyCallback(GAME->getWindow().getWindow(), key_callback);

	talga::U32 previousTime = 0;
	talga::U32 dt = 0;
	talga::U32 fps = 0;
	talga::U32 timeSince = 0;
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

	system("PAUSE");

	return 0;
}
