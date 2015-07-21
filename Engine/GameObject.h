#pragma once

#include <map>
#include <functional>
#include <vector>

#include "IRenderable.h"
#include "Rectangle.h"
#include "IDynamic.h"
#include "Math/Vector2.h"
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
		GameObject(IRenderable* rdr, std::function<void(GameObject*)> = std::function<void(GameObject*)>(nullptr), F32 x = 0.0f, F32 y = 0.0f);
		GameObject(const GameObject& cpy);
		virtual ~GameObject();
		
		void loadScript(std::string path);

		F32 getMass() const;
		void setMass(F32 value); 
		
		void setFriction(F32 value);
		F32 getFriction() const;

		void setMoment(F32 value);
		F32 getMoment() const;

		void playAnimation(const std::string& animName, I32 speed, bool loop);
		
		SharedRdrPtr getRenderable() { return pmRenderable; }

		virtual void update(F32 dt) override;

		
		void applyForce(vec2 force);
		void applyForceX(F32 x);
		void applyForceY(F32 y);
		void applyImpulseY(F32 impulse);
		void applyImpulseX(F32 impulse);
		F32 getVx() const;
		F32 getVy() const;
		
		void setCollisionType(I32 type);
		void addCollisionCallback(I32 collisionWith, OOLUA::Lua_func_ref);
		CollisionCallbackFunc getCollisionCallback(I32 collsionWith);

		void addKeyCallback(char c, KeyCallbackFunc cback);
		void addKeyCallback(std::string c, OOLUA::Lua_func_ref ref);
		
		void destroy();

		StagedFunc stagedFunc;
		UpdateFunc updateFunc;
		UnstagedFunc unstagedFunc;
	protected:
		friend class Game;

		SharedRdrPtr pmRenderable;
		cpBody* mBody;
		cpShape* mShape;
		Rectangle* mBox;
		bool isAnimated;
		
		bool DESTROY;
		Game* GAME;

		std::map<I32, CollisionCallbackFunc> mCollisionCallbacks;
		std::vector<CollisionData*> mData;
	};

	struct CollisionData
	{
		GameObject* obj;
		I32 collisionWith;
	};

	
}