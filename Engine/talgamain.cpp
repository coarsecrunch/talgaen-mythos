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

#include "chipmunk\chipmunk_private.h"

const talga::I32 WIDTH = 800;
const talga::I32 HEIGHT = 600;
talga::Game* GAME = nullptr;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GAME->game_key_callback(window, key, scancode, action, mods);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	GAME->game_resize_window(window, w, h);
}

namespace talga
{

	void Game::game_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_REPEAT)
		{
			if (key == GLFW_KEY_W)
			{
				cpBodyApplyForceAtLocalPoint(rectBody, cpv(0, 500), cpv(0, 0));
			}
			if (key == GLFW_KEY_A)
			{
				cpBodyApplyForceAtLocalPoint(rectBody, cpv(-200, 0), cpv(0, 0));
			}
			if (key == GLFW_KEY_D)
			{
				cpBodyApplyForceAtLocalPoint(rectBody, cpv(200, 0), cpv(0, 0));
			}
		}
	}

	void Game::game_resize_window(GLFWwindow* window, int w, int h)
	{
		layer.setProjectionMatrix(w, h);
		getCamera().setW(w);
		getCamera().setH(h);
		glViewport(0, 0, w, h);
	}

	Layer* LAYER = nullptr;
	Camera* CAMERA = nullptr;


	void Game::exec()
	{
		TALGA_MSG("working directory: " + getWorkingDirectory());
		Clock clock;
		clock.Init();

		this->Init(WIDTH, HEIGHT, "hello talga");
		AssetManager manager;
		manager.AddTexture("../assets/textures/testblock.png");
		manager.AddTexture("../assets/textures/talgasheet.png");
		//manager.AddTexture("../assets/talgasheet.png");
		manager.AddMap("../assets/maps/sandboxx.tmap");

		U32 previousTime = 0;
		U32 dt = 0;
		U32 fps = 0;
		U32 timeSince = 0;

		glfwSetWindowSizeCallback(getWindow().getWindow(), resize_callback);
		glfwSetKeyCallback(getWindow().getWindow(), key_callback);

		AnimationSet set{ manager.GetTexture("talgasheet.png") };

		getCamera().getBox().setX(0);
		getCamera().getBox().setY(0);


		std::vector<Rect> talgaStandL{ { Rect{ 0, 0, 64, 64 }, Rect{ 64, 0, 64, 64 } } };

		set.addAnim("talgaStandL", talgaStandL);

		Renderer renderer("../assets/shaders/renderer2d.vert", "../assets/shaders/renderer2d.frag");
		renderer.setCamera(&getCamera());

		layer = Layer{ &renderer, (F32)WIDTH, (F32)HEIGHT };

		LAYER = &layer;
		CAMERA = &getCamera();

		Sprite* block = new Sprite(manager.GetTexture("testblock.png"));
		AnimSprite* spr = new AnimSprite{ &set };

		cpMap testMap = manager.GetMap("sandboxx.tmap");

		layer.add(testMap);
		layer.add(spr);
		layer.add(block);

		spr->playAnimation("talgaStandL", 1000, true);

		//spr->getBox().setX(100);
		//spr->getBox().setY(100);


		cpVect gravity = cpv(0, -20);

		cpSpace* space = cpSpaceNew();
		cpSpaceSetGravity(space, gravity);

		cpShape* ground = cpSegmentShapeNew(space->staticBody, cpv(0, -5), cpv(20, -5), 0);
		cpShapeSetFriction(ground, 1);
		cpSpaceAddShape(space, ground);

		cpFloat bwidth = 2;
		cpFloat bheight = 2;
		cpFloat mass = 1;

		cpFloat moment = cpMomentForBox(mass, bwidth, bheight);

		rectBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
		cpBodySetPosition(rectBody, cpv(3, 0));

		cpShape* rectShape = cpSpaceAddShape(space, cpBoxShapeNew(rectBody, bwidth, bheight, 0));
		cpShapeSetFriction(rectShape, 0.7f);

		while (!glfwWindowShouldClose(getWindow().getWindow()))
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

			getWindow().swap();
			getWindow().clear();

			glfwPollEvents();
			cpVect pos = cpBodyGetPosition(rectBody);
			cpFloat rot = cpBodyGetAngle(rectBody);
			//ACommand* command = performActor(kakey, talga, acaction, &luaEngine);

			//if (command)
			//	command->execute(talga);

			//acaction = -11;
			//kakey = -11;

			Camera& cam = getCamera();

			cam.update(0);
			//cam.getBox().setX(cam.getBox().getX() + 0.05f * 20 * abs(sin(clock.TimePassed())));
			//cam.getBox().setY(cam.getBox().getY() + 0.05f * 20 * abs(sin(clock.TimePassed())));
			block->getBox().setX(pos.x * 32.0f);
			block->getBox().setY(pos.y * -32.0f);
			block->getBox().setOrientation(rot);
			block->update(dt);
			spr->update(dt);
			Update(dt);
			ResolveCollisions();
			//game.Render(&manager);

			layer.getRenderer()->tStackPush(cam.getCameraMat());

			layer.render();

			layer.getRenderer()->tStackPop();
			cpSpaceStep(space, dt / 1000.0f);
			++fps;
		}
		delete spr;
		delete block;
	}


}

int main(int argc, char** argv)
{
	GAME = new talga::Game();
	GAME->exec();
	delete GAME;
	return 0;
}
