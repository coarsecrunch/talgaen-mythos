#pragma once

#include <map>
#include <functional>
#include <vector>

#include "IRenderable.h"
#include "Rectangle.h"
#include "IDynamic.h"
#include "Math/Vector2.h"
#include "PhysicsComponent.h"
#include "funkdefs.h"
struct cpBody;
struct cpShape;

namespace talga
{
	class LuaEngine;
	class Game;
	class GameObject;

	struct CollisionData;

	class GameObject : public IDynamic
	{
	public:
		GameObject(IRenderable* rdr, PhysicsComponent* collider);
		GameObject(std::string script);
		GameObject(const GameObject& cpy);
		virtual ~GameObject();
		
		void playAnimation(const std::string& animName, I32 speed, bool loop);
		
		SharedRdrPtr getRenderable() { return pmRenderable; }

		virtual void update(F32 dt) override;
		
		void setCollisionType(I32 type);
		void addCollisionCallback(I32 collisionWith, OOLUA::Lua_func_ref);
		CollisionCallbackFunc getCollisionCallback(I32 collsionWith);

		void addKeyCallback(char c, KeyCallbackFunc cback);
		void addKeyCallback(std::string c, OOLUA::Lua_func_ref ref);
		
		void staged();
		void destroy();

		StagedFunc stagedFunc;
		UpdateFunc updateFunc;
		UnstagedFunc unstagedFunc;
	protected:
		friend class Game;

		SharedRdrPtr pmRenderable;
		PhysicsComponent* mCollider;
		Rectangle* mBox;
		bool isAnimated;
		
		bool DESTROY;
		Game* GAME;

		std::map<I32, CollisionCallbackFunc> mCollisionCallbacks;
		std::vector<CollisionData*> mData;
	};

	static struct CollisionData
	{
		GameObject* obj;
		I32 collisionWith;
	};

	
}
