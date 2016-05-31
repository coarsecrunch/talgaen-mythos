#include "luareg.h"
#include "Cmn.h"
#include "Game.h"
#include "GLFW\glfw3.h"
#include <iostream>
#include "Clock.h"
#include "LuaEngine.h"

#include "sys.h"
#include "stb/stb_vorbis.c"
#include "SoundPlayer.h"

#include "Entity.h"
#include "Component.h"

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

namespace talga
{
	class CheeseComp : public Component
	{
	public:
		std::string pops = "poop";
	};

	class BellyComp : public Component
	{
	public:
		std::string pops = "belly";
	};
}



int main(int argc, char** argv)
{
	int num = 20;
	talga::CheeseComp comp;
	talga::CheeseComp cheese;
	talga::BellyComp belly;
	talga::Entity<talga::BellyComp, talga::CheeseComp> hellojello(&belly, &cheese);

	auto t = hellojello.getComponent<talga::BellyComp>();
	std::cout << t->pops << std::endl;


	TALGA_MSG("working directory: " + talga::getWorkingDirectory());
	talga::Clock clock;
	clock.Init();
	
	std::string mapPath;
	if (argc != 2)
	{
		TALGA_WARN(0, "tried to start program with incorrect arguments")

		//return -1;
	}
	else
	{
		//mapPath = argv[1];
		TALGA_MSG("loading map " << mapPath);
	}
	
	GAME = new talga::Game();
	GAME->Init(WIDTH, HEIGHT, "hello talga");

	glfwSetWindowSizeCallback(GAME->getWindow().getWindow(), resize_callback);
	glfwSetKeyCallback(GAME->getWindow().getWindow(), key_callback);
	glfwSetMouseButtonCallback(GAME->getWindow().getWindow(), mouse_press_callback);
	glfwSetCursorPosCallback(GAME->getWindow().getWindow(), mouse_move_callback);

	LUA_REGISTER_TYPES();
	talga::LuaEngine::instance()->setGame(GAME);
	talga::LuaEngine::instance()->addGlobal("GAME", GAME);
	talga::LuaEngine::instance()->ExecuteStr("package.path = package.path .. \";../assets/scripts/?.lua\"");
	talga::LuaEngine::instance()->ExecuteStr("print = function(s) if type(s) == \"string\" then GAME:printToLuaPromptStr(s) elseif type(s) == \"number\" then GAME:printToLuaPromptFl(s) end end");
	talga::LuaEngine::instance()->ExecuteFile("../assets/scripts/script.lua");

	GAME->loadmap("../assets/maps/sandboxx.tmap");
	talga::U32 previousTime = 0;
	talga::U32 dt = 0;
	talga::U32 fps = 0;
	talga::U32 timeSince = 0;



	while (!glfwWindowShouldClose(GAME->getWindow().getWindow()))
	{
		dt = (clock.TimePassed()) - previousTime;
		timeSince += dt;
		previousTime = clock.TimePassed();

		if (timeSince >= 1000)
		{
			timeSince = 0;
			glfwSetWindowTitle(GAME->getWindow().getWindow(), (std::string("Talgaen Mythos   |   FPS: ") + std::to_string(fps)).c_str());
			fps = 0;
		}

		if (dt >= 31)
			dt = 31;

		GAME->getWindow().swap();
		GAME->getWindow().clear();

		glfwPollEvents();
		
		

		GAME->update(dt);
		GAME->render();

		++fps;
	}

	delete GAME;

	//system("PAUSE");

	return 0;
}
