#pragma once

#include <map>
#include <functional>

#include "IRenderable.h"
#include "Rectangle.h"
#include "IDynamic.h"
#include "Math/Vector2.h"
#include "Game.h"
#include "Callbacks.h"

struct cpBody;
struct cpShape;

namespace talga
{
	class LuaEngine;
	typedef std::function <void(GameObject*)> CollisionCallback;
	struct CollisionData;

	class GameObject : public IDynamic
	{
	public:
		GameObject(IRenderable* rdr, std::function<void(GameObject*)> = std::function<void(GameObject*)>(nullptr), F32 x = 0.0f, F32 y = 0.0f);
		GameObject(const GameObject& cpy);
		virtual ~GameObject();

		static void LUA_REGISTER(LuaEngine*);
		
		void loadScript(std::string path, LuaEngine* engine);

		F32 getMass() const;
		void setMass(F32 value); 
		
		void setFriction(F32 value);
		F32 getFriction() const;

		void setMoment(F32 value);
		F32 getMoment() const;

		void playAnimation(const std::string& animName, I32 speed, bool loop);
		
		SharedRdrPtr getRenderable() { return pmRenderable; }

		virtual void update(F32 dt) override;

		void setCollisionType(I32 type);
		void applyForce(vec2 force);
		void applyForceX(F32 x);
		void applyForceY(F32 y);
		void applyImpulseY(F32 impulse);
		void applyImpulseX(F32 impulse);

		F32 getVx() const;
		F32 getVy() const;

		void addCollisionCallback(I32 collisionWith, CollisionCallback);
		CollisionCallback getCollisionCallback(I32 collsionWith);
		void addKeyCallback(char c, KeyCallback cback);
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

		std::map<I32, CollisionCallback> mCollisionCallbacks;
		std::vector<CollisionData*> mData;
	};

	struct CollisionData
	{
		GameObject* obj;
		I32 collisionWith;
	};
}
