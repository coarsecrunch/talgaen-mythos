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
		GameObject();
		GameObject(IRenderable* rdr, PhysicsComponent* collider);
		GameObject(std::string script);
		GameObject(const GameObject& cpy);
		const GameObject& operator=(const GameObject& cpy);
		virtual ~GameObject();
		
		
		void setRenderable(IRenderable* renderable);
		IRenderable* getRenderable() { return pmRenderable; }

		void setCollider(PhysicsComponent* collider);
		PhysicsComponent* getCollider() { return mCollider; }

		virtual void update(F32 dt) override;
		void setCollisionType(I32 type);
		I32 getCollisionType() const;
		void addCollisionCallback(I32 collisionWith, OOLUA::Lua_func_ref);
		void addDefaultCollisionCallback(OOLUA::Lua_func_ref);
		CollisionCallbackFunc getCollisionCallback(I32 collsionWith);

		void addKeyCallback(char c, KeyCallbackFunc cback);

		void addKeyCallback(std::string c, OOLUA::Lua_func_ref ref);
		
		void playAnimation(const std::string& animName, I32 speed, bool loop);

		StagedFunc stagedFunc;
		UpdateFunc updateFunc;
		UnstagedFunc unstagedFunc;
		
		void init(IRenderable* rdr, PhysicsComponent* collider);
		void destroy();

	protected:
		friend class Game;

		IRenderable* pmRenderable;
		PhysicsComponent* mCollider;
		
		Game* mGAME;

	private:
		bool DESTROY;
		Rectangle* mBox;
		bool isAnimated;
		std::map<I32, CollisionCallbackFunc> mCollisionCallbacks;
		std::vector<CollisionData*> mData;
	};

	static struct CollisionData
	{
		GameObject* obj;
		I32 collisionWith;
	};

	
}
