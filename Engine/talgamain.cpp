#define _GLFW_USE_DWM_SWAP_INTERVAL 1
#define CHIPMUNK_FFI
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
#include "sys.h"
#include "chipmunk/chipmunk.h"

using namespace talga;

const I32 WIDTH = 800;
const I32 HEIGHT = 600;


int kakey;
int acaction;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	kakey = key;
	acaction = action;
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
	cpVect gravity = cpv(0, 100);

	cpSpace* space = cpSpaceNew();
	cpSpaceSetGravity(space, gravity);

	//cpShape* ground = cpSegmentShapeNew(space->, cpv(0, 32 * 5), cpv(20 * 32, 32 * 5), 0);


	TALGA_MSG("working directory: " + getWorkingDirectory());
	Clock clock;
	clock.Init();
	Game game;

	game.Init(WIDTH, HEIGHT, "hello talga");
	AssetManager manager;
	manager.AddTexture("../assets/charactersheet.png");
	manager.AddTexture("../assets/textures/talgasheet.png");
	//manager.AddTexture("../assets/talgasheet.png");
	manager.AddMap("../assets/maps/sandboxx.tmap");

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

	Renderer renderer("../assets/shaders/renderer2d.vert", "../assets/shaders/renderer2d.frag");
	renderer.setCamera(&game.getCamera());

	Layer layer{&renderer, (F32)WIDTH, (F32)HEIGHT};
	
	LAYER = &layer;
	CAMERA = &game.getCamera();

	AnimSprite* spr = new AnimSprite { &set };

	cpMap testMap = manager.GetMap("sandboxx.tmap");

	layer.add(static_cast<const IRenderable*>(testMap));
	layer.add(spr);

	spr->playAnimation("talgaStandL", 1000, true);

	spr->getBox().setX(100);
	spr->getBox().setY(100);

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

		cam.update(0);
		//cam.getBox().setX(cam.getBox().getX() + 0.05f * 20 * abs(sin(clock.TimePassed())));
		//cam.getBox().setY(cam.getBox().getY() + 0.05f * 20 * abs(sin(clock.TimePassed())));
		spr->update(dt);
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
