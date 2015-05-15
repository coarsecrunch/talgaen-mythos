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
#include "LuaBridge\LuaBridge.h"

#include "Renderer.h"
#include "Layer.h"
#include "Sprite.h"
#include "AnimSet.h"
#include "Math/RandomGen.h"

using namespace talga;

const I32 WIDTH = 800;
const I32 HEIGHT = 600;


int kakey;
int acaction;

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

/*
void resize_window_callback(GLFWwindow* window, int width, int height)
{
	game.getCamera().getBox().setW(width);
	game.getCamera().getBox().setH(height);
}*/

Layer* LAYER = nullptr;
Camera* CAMERA = nullptr;

void resize_window_callback(GLFWwindow* window, int w, int h)
{
	LAYER->setProjectionMatrix(w, h);
	CAMERA->setW(w);
	CAMERA->setH(h);
	glViewport(0, 0, w, h);
}

int main(int argc, char** argv)
{

	Clock clock;
	clock.Init();
	Game game;

	LuaEngine luaEngine;
	luaEngine.Init();

	Game::LUA_REGISTER(&luaEngine);

	game.Init(WIDTH, HEIGHT, "hello talga");
	AssetManager manager;
	//manager.AddTexture("../assets/charactersheet.png");
	manager.AddTexture("../assets/sprite_sheet.png");
	manager.AddTexture("../assets/talgasheet.png");
	manager.AddMap("../assets/test.map");

	

	U32 previousTime = 0;
	U32 dt = 0;
	U32 fps = 0;
	U32 timeSince = 0;
	
	glfwSetWindowSizeCallback(game.getWindow().getWindow(), resize_window_callback);
	AnimationSet set{ manager.GetTexture("talgasheet.png") };

	game.getCamera().getBox().setX(0);
	game.getCamera().getBox().setY(0);


	std::vector<Rect> talgaStandL{ { Rect{ 0, 0, 64, 64 }, Rect{ 64, 0, 64, 64 } } };

	set.addAnim("talgaStandL", talgaStandL);

	Renderer renderer("..\\assets\\shaders\\renderer2d.vert", "..\\assets\\shaders\\renderer2d.frag");
	renderer.setCamera(&game.getCamera());

	Layer layer{&renderer, (F32)WIDTH, (F32)HEIGHT};
	
	LAYER = &layer;
	CAMERA = &game.getCamera();

	AnimSprite* spr = new AnimSprite { &set };

	Sprite* sprity = new Sprite( manager.GetTexture("sprite_sheet.png") );
	sprity->getBox().setX(300);

	cpMap testMap = manager.GetMap("test.map");

	layer.add(static_cast<const IRenderable*>(testMap));
	layer.add(spr);
	layer.add(sprity);

	spr->playAnimation("talgaStandL", 1000, true);

	spr->getBox().setX(200);
	spr->getBox().setY(200);
	
	sprity->getBox().setX(100);
	sprity->getBox().setY(400);

	while (!glfwWindowShouldClose(game.getWindow().getWindow()))
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
		
		game.getWindow().swap();
		game.getWindow().clear();
		
		glfwPollEvents();

		//ACommand* command = performActor(kakey, talga, acaction, &luaEngine);
		
		//if (command)
		//	command->execute(talga);

		acaction = -11;
		kakey = -11;

		Camera& cam = game.getCamera();

		//cam.getBox().setX(cam.getBox().getX() - 0.05f);
		cam.update(0);

		spr->update(dt);
		sprity->update(dt);
		game.Update(dt);
		game.ResolveCollisions();
		//game.Render(&manager);
		
		layer.getRenderer()->tStackPush(cam.getCameraMat());
		
		layer.render();

		layer.getRenderer()->tStackPop();

		++fps;
	}

	return 0;
}
