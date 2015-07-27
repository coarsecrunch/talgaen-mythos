#include "Game.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "Map.h"
#include "AssetManager.h"
#include "Collision.h"
#include <iostream>

#include "oolua/oolua.h"
#include "GameObject.h"
#include "luadebugprompt.h"

#include "chipmunk/chipmunk_private.h"
#include "collisiontypes.h"
#include "luareg.h"
#include "renderableshapes.h"
namespace talga
{
	const int MAX_GAMEOBJECTS = 3000;
	const F32 TINFINITY = INFINITY;
	Game::Game()
		: mCamera(1200, 900)
		, mWindow(1200, 900)
		, mMapLayer{ nullptr, -1, -1 }
		, mObjectsLayer{ nullptr, -1, -1 }
		, mUILayer{nullptr, -1, -1}
		, mGameObjects( )
		, mRenderer( nullptr )
		, mManager()
		, mKeyCallbacks()
		, mPlayer{ nullptr }
		, mMouseX{0.0}
		, mMouseY{0.0}
		, mPromptIsSelected{false}
		, mCurrentMap()
		, mSceneGeom()
		, mDebugMode{false}
	{
		mGameObjects.reserve(MAX_GAMEOBJECTS);
	}

	int Game::Init(int width, int height, const char* name)
	{
		mWidth = width;
		mHeight = height;
		mCamera.box().setW(width);
		mCamera.box().setH(height);

		cpVect gravity = cpv(0, -600);

		mSpace = cpSpaceNew();
		cpSpaceSetGravity(mSpace, gravity);
		cpSpaceSetIterations(mSpace, 20);

		mRenderer = std::shared_ptr<Renderer>(new Renderer{ "../assets/shaders/renderer2d.vert", "../assets/shaders/renderer2d.frag" });
		mManager.AddTexture("../assets/textures/testblock.png");
		mManager.AddTexture("../assets/textures/talgasheet.png");
		mManager.AddTexture("../assets/textures/luaprompt.png");
		mManager.AddMap("../assets/maps/sandboxx.tmap");
		mManager.addFont("../assets/fonts/EnvyR.ttf", 15);

		loadmap("../assets/maps/sandboxx.tmap");

		mMapLayer = Layer{ mRenderer, (F32)width, (F32)height };
		mObjectsLayer = Layer{ mRenderer, (F32)width, (F32)height };
		mUILayer = Layer{ mRenderer, (F32)width, (F32)height };
		mUILayer.setVisible(false);

		mRenderer->setCamera(&mCamera);

		mMapLayer.add(&mCurrentMap);
		mPrompt = new LuaDebugPrompt( mManager.GetTexture("luaprompt.png"), mManager.getFont("EnvyR.ttf"));
		mPrompt->box().setX(-width * 0.5f + mPrompt->box().getW() * 0.5f + 20);
		mPrompt->box().setY(-height * 0.5f + mPrompt->box().getH() * 0.5f + 20);

		mPrompt->box().updateVertsPosition();
		mUILayer.add(mPrompt);

		LuaEngine::instance()->addGlobal("TALGA_KEYPRESS", TALGA_KEYPRESS);
		LuaEngine::instance()->addGlobal("TALGA_KEYRELEASE", TALGA_KEYRELEASE);
		LuaEngine::instance()->addGlobal("TALGA_KEYCONTINUE", TALGA_KEYCONTINUE);

		LuaEngine::instance()->addGlobal("INFINITY", TINFINITY);

		

		return 0;
	}

	void Game::addObj(GameObject* obj)
	{
		if (obj->stagedFunc)
			obj->stagedFunc(obj);

		obj->mGAME = this;
		mGameObjects.push_back(obj);
		mObjectsLayer.add(obj->pmRenderable);
		
		TALGA_MSG("Game object has been added")
	}

	void Game::addKeyCallback(char c, GameObject* obj, KeyCallbackFunc callback)
	{
		mKeyCallbacks.insert(std::pair < char, std::pair<GameObject*, KeyCallbackFunc> > (toupper(c), std::pair<GameObject*, KeyCallbackFunc>(obj, callback) ));
	}

	void Game::removeObj(GameObject* obj)
	{
		for (auto it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
		{

			if (*it == obj)
			{
				if ( (*it)->unstagedFunc )
					(*it)->unstagedFunc(*it);

				cpSpaceRemoveShape(mSpace, (*it)->mCollider->mShape);
				cpSpaceRemoveBody(mSpace, (*it)->mCollider->mBody);


				if (it == mGameObjects.end() - 1)
				{
					mObjectsLayer.remove(obj->pmRenderable);
					delete *it;
					mGameObjects.pop_back();
				}
				else
				{
					mObjectsLayer.remove(obj->pmRenderable);
					delete *it;
					*it = mGameObjects.back();
					mGameObjects.pop_back();
				}
				
				break;
			}
		}
	}

	void Game::clearObjs()
	{
		for (auto it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
		{
			if ((*it)->unstagedFunc)
				(*it)->unstagedFunc(*it);

			cpSpaceRemoveShape(mSpace, (*it)->mCollider->mShape);
			cpSpaceRemoveBody(mSpace, (*it)->mCollider->mBody);

			delete *it;
		}

		mGameObjects.clear();
	}
	void Game::update(F32 dt)
	{
		cpSpaceStep(mSpace, dt / 1000.0f);
		mCamera.update(dt);

		for (auto it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
		{
			(*it)->update(dt);
			if ((*it)->DESTROY)
			{
				removeObj(*it);
				if (it != mGameObjects.begin())
					--it;
			}
		}

	}

	void Game::render()
	{
		mMapLayer.getRenderer()->tStackPush(mCamera.getCameraMat());
		mMapLayer.render();
		mObjectsLayer.render();
		mMapLayer.getRenderer()->tStackPop();

		mUILayer.render();
	}

	void Game::clearMap()
	{
		for (auto it = mSceneGeom.begin(); it != mSceneGeom.end(); ++it)
		{
			cpSpaceRemoveShape(mSpace, it->second);
			cpSpaceRemoveBody(mSpace, it->first);
			
			cpShapeFree(it->second);
			cpBodyFree(it->first);
		}
		mSceneGeom.clear();
		clearObjs();
		
	}

	void Game::loadmap(const std::string& path)
	{
		clearMap();
		mCurrentMap = Map();
		mCurrentMap.load(path, mManager);
		for (auto it = mCurrentMap.getSceneGeom().cbegin(); it != mCurrentMap.getSceneGeom().cend(); ++it)
		{
			if (dynamic_cast<const RdrRect*>(*it))
			{
				RdrRect* rect = (RdrRect*)*it;
				rect->box().updateVerts();
				cpBody* body = cpSpaceAddBody(mSpace, cpBodyNewStatic());;
				cpBodySetPosition(body, cpv(rect->box().getX(), -rect->box().getY()));

				cpShape* shape = cpBoxShapeNew(body, rect->box().getW(), rect->box().getH(), 0);
				cpShapeSetFriction(shape, 0.99f);
				cpShapeSetCollisionType(shape, COLL_MAPGEOM);
				cpSpaceAddShape(mSpace, shape);
				

				mSceneGeom.push_back(std::pair<cpBody*, cpShape*>(body, shape));
			}
			else if (dynamic_cast<const RdrTri*>(*it))
			{
				RdrTri* tri = (RdrTri*)*it;
				tri->getBase().updateVerts();
				cpBody* body = cpSpaceAddBody(mSpace, cpBodyNewStatic());;
				cpBodySetPosition(body, cpv(tri->getBase().getX(), -tri->getBase().getY()));
	
				cpVect verts[3] = { cpv(tri->getBase().getRealVerts()[0].x(), -tri->getBase().getRealVerts()[0].y()),
					cpv(tri->getBase().getRealVerts()[1].x(), -tri->getBase().getRealVerts()[1].y()),
					cpv(tri->getBase().getRealVerts()[2].x(), -tri->getBase().getRealVerts()[2].y()) };

				

				cpShape* shape = cpPolyShapeNew(body, 3, verts, cpTransform{ 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f }, 0.0f);
				
				TALGA_PRVAL(tri->getBase().getRealVerts()[0].x())
					TALGA_PRVAL(tri->getBase().getRealVerts()[0].y())
					TALGA_PRVAL(cpPolyShapeGetVert(shape, 0).x)
					TALGA_PRVAL(cpPolyShapeGetVert(shape, 0).y)


				cpShapeSetFriction(shape, 0.99f);
				cpShapeSetCollisionType(shape, COLL_MAPGEOM);
				cpSpaceAddShape(mSpace, shape);


				mSceneGeom.push_back(std::pair<cpBody*, cpShape*>(body, shape));
			}
		}
		
	}

	// currently relies on the fact that glfw defines keys as their ascii value, probably should define my own constants
	void Game::game_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT || action == GLFW_RELEASE)
		{
			if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) return;

			if (key == GLFW_KEY_F10 && action == GLFW_PRESS)
			{
				mDebugMode = !mDebugMode;
				
				mUILayer.setVisible(mDebugMode);
				mCurrentMap.setRenderSceneGeom(mDebugMode);
			}

			if (mPromptIsSelected && mDebugMode)
			{
				if (action == GLFW_RELEASE) return;
				if (key == GLFW_KEY_BACKSPACE)
				{
					mPrompt->popCmd();
					return;
				}
				else if (key == GLFW_KEY_ENTER)
				{
					mPrompt->doCmd();
					return;
				}


				if (mods & GLFW_MOD_SHIFT)
				{
					
					switch (key)
					{
					case GLFW_KEY_9: mPrompt->pushCToCmd('('); break;
					case GLFW_KEY_0: mPrompt->pushCToCmd(')'); break;
					case GLFW_KEY_APOSTROPHE: mPrompt->pushCToCmd('\"'); break;
					case GLFW_KEY_SEMICOLON: mPrompt->pushCToCmd(':'); break;
					case GLFW_KEY_8: mPrompt->pushCToCmd('*'); break;
					default: mPrompt->pushCToCmd(toupper(key)); break;
					}
				}
				else
					mPrompt->pushCToCmd(tolower(key));
			}
			else
			{
				auto range = mKeyCallbacks.equal_range(key);

				int keyaction = -1;

				switch (action)
				{
				case GLFW_PRESS:
					keyaction = TALGA_KEYPRESS; break;
				case GLFW_RELEASE:
					keyaction = TALGA_KEYRELEASE; break;
				default:
					keyaction = -1; break;
				}

				for (auto it = range.first; it != range.second; ++it)
				{
					(*it).second.second(it->second.first, keyaction);
				}
			}
		}
	}

	void Game::printToLuaPrompt(const std::string& str)
	{
		mPrompt->print(str);
	}

	void Game::printToLuaPrompt(float f)
	{
		std::string temp = std::to_string(f);
		for (auto it = temp.rbegin(); it != temp.rend(); ++it)
		{
			if (*it == '.')
			{
				temp.pop_back();
				break;
			}
			else if (*it == '0')
			{
				temp.pop_back();
			}
			else
			{
				break;
			}
		}
		mPrompt->print(temp);
	}

	void Game::game_resize_window(GLFWwindow* window, int w, int h)
	{
		mWidth = w;
		mHeight = h;
		mObjectsLayer.setProjectionMatrix(w, h);
		mMapLayer.setProjectionMatrix(w, h);
		mUILayer.setProjectionMatrix(w, h);
		mCamera.setW(w);
		mCamera.setH(h);
		mPrompt->box().setX(-w * 0.5f + mPrompt->box().getW() * 0.5f + 20);
		mPrompt->box().setY(-h * 0.5f + mPrompt->box().getH() * 0.5f + 20);

		mPrompt->box().updateVertsPosition();
		glViewport(0, 0, w, h);
	}

	void Game::game_mouse_press_callback(GLFWwindow* window, int button, int action, int mods)
	{
		if (mPrompt->wasSelected(mMouseX - 0.5f * mWidth, mMouseY - 0.5f * mHeight) && mDebugMode)
			mPromptIsSelected = true;
		else
			mPromptIsSelected = false;
	}

	void Game::game_mouse_move_callback(GLFWwindow* window, double x, double y)
	{
		mMouseX = x;
		mMouseY = y;
	}

	Game::~Game()
	{
		clearMap();
		cpSpaceFree(mSpace);

		mMapLayer.clear();
		mObjectsLayer.clear();
		mUILayer.clear();
		delete mPrompt;
	}

	

}

