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

#define GROUND_COLLISION 1
#define COLLISION_PHYSOBJ 2
static bool applyRight = false;
static bool applyLeft = false;
static bool applyUp = false;
static bool jump = true;
const talga::I32 WIDTH = 800;
const talga::I32 HEIGHT = 600;
talga::Game* GAME = nullptr;

talga::AnimSprite* TALGA = nullptr;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GAME->game_key_callback(window, key, scancode, action, mods);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	GAME->game_resize_window(window, w, h);
}

cpBool beginPhysGround(cpArbiter *arb, cpSpace *space, cpDataPointer data)
{
	auto collisionNormal = cpArbiterGetNormal(arb);
	
	if (collisionNormal.y <= 0.01f)
	{
		std::cout << "CORRECT COLLISION!" << std::endl;
		jump = true;
	}
	
	return 1;
}

namespace talga
{
	void Game::game_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			if (key == GLFW_KEY_W)
			{
				std::cout << "UP" << std::endl;
				applyUp = true;
			}
			if (key == GLFW_KEY_A)
			{
				TALGA->playAnimation("talgaWalkL", 500, true);
				applyLeft = true;
			}
			if (key == GLFW_KEY_D)
			{
				TALGA->playAnimation("talgaWalkR", 500, true);
				applyRight = true;
			}
		}
		else if (action == GLFW_RELEASE)
		{

			if (key == GLFW_KEY_W)
			{
				applyUp = false;
			}
			if (key == GLFW_KEY_A)
			{
				TALGA->playAnimation("talgaStandL", 1000, true);
				applyLeft = false;
			}
			if (key == GLFW_KEY_D)
			{
				TALGA->playAnimation("talgaStandR", 1000, true);

				applyRight = false;
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
		manager.AddTexture("../assets/textures/talgasheet.png");
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
		std::vector<Rect> talgaStandR{ { Rect{ 128, 0, 64, 64 }, Rect{ 192, 0, 64, 64 } } };
		std::vector<Rect> talgaWalkL;
		std::vector<Rect> talgaWalkR;
		for (int i = 0; i < 6; ++i)
		{
			talgaWalkL.push_back(Rect{64 * i, 64, 64, 64});
		}
		for (int i = 6; i < 12; ++i)
		{
			talgaWalkR.push_back(Rect{ 64 * i, 64, 64, 64 });
		}
		set.addAnim("talgaStandL", talgaStandL);
		set.addAnim("talgaStandR", talgaStandR);
		set.addAnim("talgaWalkL", talgaWalkL);
		set.addAnim("talgaWalkR", talgaWalkR);

		Renderer renderer("../assets/shaders/renderer2d.vert", "../assets/shaders/renderer2d.frag");
		renderer.setCamera(&getCamera());

		layer = Layer{ &renderer, (F32)WIDTH, (F32)HEIGHT };

		LAYER = &layer;
		CAMERA = &getCamera();
		

		Sprite* block = new Sprite(manager.GetTexture("testblock.png"));
		AnimSprite* spr = new AnimSprite{ &set };
		TALGA = spr;
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
		cpShapeSetFriction(ground, 0.97f);
		cpSpaceAddShape(space, ground);

		cpFloat bwidth = 2;
		cpFloat bheight = 2;
		cpFloat mass = 20;

		cpFloat moment = cpMomentForBox(mass, bwidth, bheight);

		cpBody* blockBody = cpSpaceAddBody(space, cpBodyNew(mass, moment + 10));
		cpBodySetPosition(blockBody, cpv(5, 0));

		rectBody = cpSpaceAddBody(space, cpBodyNew(8, INFINITY));
		cpBodySetPosition(rectBody, cpv(3, 0));

		cpShape* rectShape = cpSpaceAddShape(space, cpBoxShapeNew(rectBody, 1.8f, bheight, 0));
		cpShapeSetFriction(rectShape, 0.99f);
		cpShape* blockShape = cpSpaceAddShape(space, cpBoxShapeNew(blockBody, bwidth, bheight, 0));
		cpShapeSetFriction(blockShape, 0.95f);

		cpShapeSetCollisionType(ground, GROUND_COLLISION);
		cpShapeSetCollisionType(rectShape, COLLISION_PHYSOBJ);
		//cpSpaceSetDamping(space, 0.5f);

		//cpSpaceAddCollisionHandler(space, GROUND_COLLISION, COLLISION_PHYSOBJ)->beginFunc = beginPhysGround;
		cpSpaceAddWildcardHandler(space, COLLISION_PHYSOBJ)->beginFunc = beginPhysGround;

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
			glfwPollEvents();
			getWindow().swap();
			getWindow().clear();

			auto x = cpBodyGetVelocity(rectBody).x;
			

			if (applyUp && jump)
			{
				cpBodyApplyForceAtLocalPoint(rectBody, cpv(x, 5000), cpv(0, 0));
				//cpBodySetVelocity(rectBody, cpv(x, 10));
				jump = false;
			}

			auto vy = cpBodyGetVelocity(rectBody).y;

			if (applyLeft)
			{
				if (cpBodyGetVelocity(rectBody).x >= -6)
				{
					cpBodyApplyForceAtLocalPoint(rectBody, cpv(-500, vy), cpv(0, 0));

				}
				//cpBodySetVelocity(rectBody, cpv(-5, vy));
			}
			else if (applyRight)
			{
				if (cpBodyGetVelocity(rectBody).x <= 6)
				{
					cpBodyApplyForceAtLocalPoint(rectBody, cpv(500, vy), cpv(0, 0));

				}
			}

			cpVect pos = cpBodyGetPosition(rectBody);
			cpFloat rot = cpBodyGetAngle(rectBody);
			auto blockPos = cpBodyGetPosition(blockBody);
			auto blockRot = cpBodyGetAngle(blockBody);
			//ACommand* command = performActor(kakey, talga, acaction, &luaEngine);

			//if (command)
			//	command->execute(talga);

			//acaction = -11;
			//kakey = -11;

			Camera& cam = getCamera();

			cam.update(0);
			cam.getBox().setX(spr->getBox().getX());
			cam.getBox().setY(spr->getBox().getY());
			spr->getBox().setX(pos.x * 32.0f);
			spr->getBox().setY(pos.y * -32.0f);
			spr->getBox().setOrientation(-rot);
			block->getBox().setX(blockPos.x * 32.0f);
			block->getBox().setY(blockPos.y * -32.0f);
			block->getBox().setOrientation(-blockRot);
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
		cpSpaceFree(space);
		cpBodyFree(rectBody);
		cpBodyFree(blockBody);
		cpShapeFree(rectShape);
		cpShapeFree(blockShape);
		
		
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
