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
#include "PhysicsComponent.h"

const talga::I32 WIDTH = 1200;
const talga::I32 HEIGHT = 900;
talga::Game* GAME = nullptr;
talga::LuaEngine* LUA_ENGINE = nullptr;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GAME->game_key_callback(window, key, scancode, action, mods);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	GAME->game_resize_window(window, w, h);
}

void mouse_press_callback(GLFWwindow* window, int button, int action, int mods)
{
	GAME->game_mouse_press_callback(window, button, action, mods);
}

void mouse_move_callback(GLFWwindow * window, double x, double y)
{
	GAME->game_mouse_move_callback(window, x, y);
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
	talga::AnimationSet set{ GAME->manager()->GetTexture("talgasheet.png"), "talgaAnims" };
	talga::LuaEngine::instance()->ExecuteStr("print = function(s) if type(s) == \"string\" then GAME:printToLuaPromptStr(s) elseif type(s) == \"number\" then GAME:printToLuaPromptFl(s) end end");

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

	set.addAnim("talgaWalkL", talgaWalkL);
	set.addAnim("talgaWalkR", talgaWalkR);

	talga::AnimSprite* spr = new talga::AnimSprite{ &set };

	talga::cpMap testMap = GAME->manager()->GetMap("sandboxx.tmap");


	talga::GameObject* tga = new talga::GameObject("../assets/scripts/talga.lua");
	
	GAME->addObj(tga);

	talga::LuaEngine::instance()->ExecuteFile("../assets/scripts/script.lua");
	
	glfwSetWindowSizeCallback(GAME->getWindow().getWindow(), resize_callback);
	glfwSetKeyCallback(GAME->getWindow().getWindow(), key_callback);
	glfwSetMouseButtonCallback(GAME->getWindow().getWindow(), mouse_press_callback);
	glfwSetCursorPosCallback(GAME->getWindow().getWindow(), mouse_move_callback);

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

	//system("PAUSE");

	return 0;
}
