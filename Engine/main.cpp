#include "Cmn.h"
#include "Rendering.h"
#include "SDL2\SDL.h"
#include "Map.h"
#include "Game.h"
#include "Texture.h"
#include "PhysSprite.h"
#include "Collision.h"
#include "Animation.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include <vector>
#include <iostream>
#include "Command.h"
#include "Character.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"
#include "AssetManager.h"
#include "Clock.h"

#include "ParticleEmitter.h"

#include "LuaEngine.h"
#include "LuaBridge\LuaBridge.h"


#include "Renderer.h"
#include "Layer.h"
#include "Sprite.h"
#include "AnimSet.h"
#include "Math/RandomGen.h"

using namespace talga;

const I32 WIDTH = 800;
const I32 HEIGHT = 600;


Game game;
std::string cmd;
int kakey;
int acaction;

void GLOBALS_LUA_REGISTER(LuaEngine* engine)
{
	using namespace luabridge;
	getGlobalNamespace(engine->getState())
		.beginNamespace("Engine")
		.addVariable("gamy", &game)
		.endNamespace();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	kakey = key;
	acaction = action;
	/*
	if (key == GLFW_KEY_0)
		bob.PlayAnimation(0, 5000, true);
	else if (key == GLFW_KEY_1)
		bob.PlayAnimation(1, 1000, false);
	else if (key == GLFW_KEY_2)
		bob.PlayAnimation(2, 1000, false);
	else if (key == GLFW_KEY_3)
		bob.PlayAnimation(3, 1000, false);
	*/
}

JumpCommand jump;
StandR standRCommand;
StandL standLCommand;
WalkR walkRCommand;
WalkL walkLCommand;
RunR runRCommand;
RunL runLCommand;

ACommand* performActor(int key, Character& actor, I32 action, LuaEngine* engine)
{
	static bool alreadyPlaying = false;

	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
		if (game.isRenderingCollisions())
			game.setRenderCollisions(false);
		else
			game.setRenderCollisions(true);

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		return &runRCommand;
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		return &runLCommand;
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		return &jump;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
	}
	else if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
	{
		engine->ExecuteStr("Engine.game:getPlayer()");
	}
	else if ((key == GLFW_KEY_D || key == GLFW_KEY_A) && action == GLFW_RELEASE)
	{
		if (actor.getDirection() == Character::DIRECTION::LEFT)
			return &standLCommand;
		else
			return &standRCommand;
	}
	else if ((key == GLFW_KEY_W || key == GLFW_KEY_S) && action == GLFW_RELEASE)
	{
		//spr.setVy(0.0f);
	}

	return nullptr;
}

void resize_window_callback(GLFWwindow* window, int width, int height)
{
	game.getCamera().setW(width);
	game.getCamera().setH(height);
}

int main(int argc, char** argv)
{
	LuaEngine luaEngine;
	luaEngine.Init();

	Character::LUA_REGISTER(&luaEngine);
	Game::LUA_REGISTER(&luaEngine);
	GLOBALS_LUA_REGISTER(&luaEngine);

	luaEngine.ExecuteStr("print 'testCommand'");

	game.Init(WIDTH, HEIGHT, "hello talga");
	AssetManager manager;
	manager.AddTexture("../assets/charactersheet.png");
	manager.AddTexture("../assets/sprite_sheet.png");
	manager.AddTexture("../assets/talgasheet.png");
	manager.LoadMap("../assets/test.map");

	Clock clock;
	clock.Init();

	std::vector<Rect> still;
	still.push_back(Rect{ 0, 0, 32, 64 });
	still.push_back(Rect{ 32, 0, 32, 64 });
	std::vector<Rect> walk;
	walk.push_back(Rect{ 0, 64, 32, 64 });
	walk.push_back(Rect{ 32, 64, 32, 64 });
	walk.push_back(Rect{ 64, 64, 32, 64 });
	walk.push_back(Rect{ 96, 64, 32, 64 });

	manager.AddAnimation("charactersheet.png", "bobStillAnim", still);
	manager.AddAnimation("charactersheet.png", "bobWalkAnim", walk);

	std::vector<Rect> talgaStillL;
	std::vector<Rect> talgaStillR;
	std::vector<Rect> talgaWalkL;
	std::vector<Rect> talgaWalkR;
	std::vector<Rect> talgaRunL;
	std::vector<Rect> talgaRunR;

	//create stillL
	talgaStillL.push_back(Rect{ 0, 0, 64, 64 });
	talgaStillL.push_back(Rect{ 64, 0, 64, 64 });
	//create stillR
	talgaStillR.push_back(Rect{ 128, 0, 64, 64 });
	talgaStillR.push_back(Rect{ 192, 0, 64, 64 });
	//create walkL
	for (int i = 0; i < 6; ++i)
		talgaWalkL.push_back(Rect{ i * 64, 64, 64, 64 });
	//create walkR
	for (int i = 6; i < 12; ++i)
		talgaWalkR.push_back(Rect{ i * 64, 64, 64, 64 });
	//create runL
	for (int i = 0; i < 6; ++i)
		talgaRunL.push_back(Rect{ i * 64, 128, 64, 64 });
	//create runR
	for (int i = 6; i < 12; ++i)
		talgaRunR.push_back(Rect{ i * 64, 128, 64, 64 });

	manager.AddAnimation("talgasheet.png", "talgaStillL", talgaStillL);
	manager.AddAnimation("talgasheet.png", "talgaStillR", talgaStillR);
	manager.AddAnimation("talgasheet.png", "talgaWalkL", talgaWalkL);
	manager.AddAnimation("talgasheet.png", "talgaWalkR", talgaWalkR);
	manager.AddAnimation("talgasheet.png", "talgaRunL", talgaRunL);
	manager.AddAnimation("talgasheet.png", "talgaRunR", talgaRunR);

	manager.LoadAnimations();

	std::vector<cpAnim> talgaAnims;

	//talgaAnims.reserve((I32)Character::ANIM::numAnimations);
	talgaAnims.resize((I32)Character::ANIM::numAnimations);

	talgaAnims[(I32)Character::ANIM::STANDL] = (manager.GetAnimation("talgaStillL"));
	talgaAnims[(I32)Character::ANIM::STANDR] = (manager.GetAnimation("talgaStillR"));
	talgaAnims[(I32)Character::ANIM::WALKL] = (manager.GetAnimation("talgaWalkL"));
	talgaAnims[(I32)Character::ANIM::WALKR] = (manager.GetAnimation("talgaWalkR"));
	talgaAnims[(I32)Character::ANIM::RUNR] = (manager.GetAnimation("talgaRunR"));
	talgaAnims[(I32)Character::ANIM::RUNL] = (manager.GetAnimation("talgaRunL"));

	std::vector<cpAnim> anims;
	anims.push_back(manager.GetAnimation("bobStillAnim"));
	anims.push_back(manager.GetAnimation("bobWalkAnim"));

	Character talga(&game, vec4(100.0f, 0.0f, 0.0f, 1.0f), vec4(0.0f, 0.0f, 0.0f, 0.0f), talgaAnims);
	PhysSprite bob(&game, vec4(300.0f, 0.0f, 0.0f, 1.0f), vec4(0.0f, 0.0f, 0.0f, 0.0f), anims);

	glfwMakeContextCurrent(game.getWindow());
	glfwSetKeyCallback(game.getWindow(), key_callback);
	glfwSetWindowSizeCallback(game.getWindow(), resize_window_callback);
	glfwSwapInterval(0);



	U32 previousTime = 0;
	U32 dt = 0;
	U32 fps = 0;
	U32 timeSince = 0;
	F32 rot = 0;
	bool positive = true;

	AnimationSet set{ manager.GetTexture("talgasheet.png") };

	set.addAnim("poop", talgaWalkL);

	Renderer renderer("../assets/shaders/renderer2d.vert", "../assets/shaders/renderer2d.frag");
	renderer.setCamera(&game.getCamera());

	Layer layer{&renderer, WIDTH, HEIGHT};
	
	AnimSprite spr{ &set };

	Sprite sprity{ manager.GetTexture("sprite_sheet.png") };
	sprity.getBox().setX(300);

	seedRand(clock.TimePassed());

	cpMap testMap = manager.GetMap("test.map");

	layer.add(static_cast<const IRenderable*>(testMap));
	layer.add(&spr);
	layer.add(&sprity);
	

	spr.playAnimation("poop", 1000, true);

	

	while (!glfwWindowShouldClose(game.getWindow()))
	{

		dt = clock.TimePassed() - previousTime;
		timeSince += dt;
		previousTime = clock.TimePassed();

		if (timeSince >= 1000)
		{
			timeSince = 0;
			std::cout << std::endl << "FPS: " << fps << std::endl;
			fps = 0;
		}
		
		
		glfwSwapBuffers(game.getWindow());
		game.getCamera().Clear();
		
		glfwPollEvents();

		//ACommand* command = performActor(kakey, talga, acaction, &luaEngine);
		
		//if (command)
		//	command->execute(talga);

		acaction = -11;
		kakey = -11;

		Camera& cam = game.getCamera();

		spr.update(dt);
		sprity.update(dt);
		game.Update(dt);
		game.ResolveCollisions();
		//game.Render(&manager);
		
		layer.getRenderer()->push(cam.getCameraMat());
		
		layer.render();

		layer.getRenderer()->pop();

		++fps;
	}

	return 0;
}