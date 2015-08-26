#include "GameObject.h"
#include "chipmunk/chipmunk_private.h"
#include "Sprite.h"
#include "Game.h"
#include "collisiontypes.h"

#include "oolua/oolua_string.h"
#include "LuaEngine.h"
#include "sys.h"

#ifndef TALGA_QT_BUILD
extern talga::Game* GAME; // <------------- NOOOOOOOOOOOO DON'T DO IT THIS IS SOOOO BAD THIS ISN'T SUPPOSED TO BE A SINGLETON!!
#else
talga::Game* GAME = nullptr;
#endif
namespace talga
{
	GameObject::GameObject(IRenderable* rdr, PhysicsComponent* collider)
		: mGAME(GAME)
		, DESTROY(false)
		, pmRenderable(nullptr)
		, isAnimated{ false }
		, mCollider(nullptr)
		, mBox{ nullptr }
		, stagedFunc()
		, updateFunc()
		, unstagedFunc()
		, initFunc()
		, destroyFunc()
	{
		init(rdr, collider);
	}

	void GameObject::init(IRenderable* rdr, PhysicsComponent* collider)
	{
		setRenderable(rdr);
		setCollider(collider);
		setCollisionType(COLL_DEFAULT);

		if (initFunc)
			initFunc(this);
	}

	GameObject::GameObject()
		: mGAME(GAME)
		, DESTROY(false)
		, pmRenderable(nullptr)
		, isAnimated{ false }
		, mCollider(nullptr)
		, mBox{ nullptr }
		, stagedFunc()
		, updateFunc()
		, unstagedFunc()
		, initFunc()
		, destroyFunc()
	{
		init(nullptr, nullptr);
	}

	GameObject::GameObject(OOLUA::Lua_table_ref tbl)
		: mGAME(GAME)
		, DESTROY(false)
		, isAnimated{ false }
		, mBox{ nullptr }
		, pmRenderable{ nullptr }
		, mCollider{nullptr}
		, stagedFunc()
		, updateFunc()
		, unstagedFunc()
		, initFunc()
		, destroyFunc()
	{
		OOLUA::Lua_func_ref stagedFuncRef;
		OOLUA::Lua_func_ref unstagedFuncRef;
		OOLUA::Lua_func_ref updateFuncRef;
		OOLUA::Lua_func_ref initFuncRef;
		OOLUA::Lua_func_ref destroyFuncRef;

		OOLUA::Table tempTbl(tbl);

		TALGA_WARN(tempTbl.valid(), "incorrect game object table tried to load");
		
		if (!tempTbl.valid())
			return;

		TALGA_MSG("game object successfully loaded");

		tempTbl.at("init", initFuncRef);
		tempTbl.at("destroy", destroyFuncRef);
		tempTbl.at("staged", stagedFuncRef);
		tempTbl.at("unstaged", unstagedFuncRef);
		tempTbl.at("update", updateFuncRef);
		
		
		if (initFuncRef.valid())
			initFunc = initFuncRef;
		else
			TALGA_WARN(0, "failed to find initFunc in gameobject table")

		if (destroyFuncRef.valid())
			destroyFunc = destroyFuncRef;
		else
			TALGA_WARN(0, "failed to find destroyFunc in gameobject table")


		if (stagedFuncRef.valid())
			stagedFunc = stagedFuncRef;
		else
			TALGA_WARN(0, "failed to find stagedFunc in gameobject table");

		if (unstagedFuncRef.valid())
			unstagedFunc = unstagedFuncRef;
		else
			TALGA_WARN(0, "failed to find unstagedFunc in gameobject table");


		if (updateFuncRef.valid())
			updateFunc = updateFuncRef;
		else
			TALGA_WARN(0, "failed to find updateFunc in gameobject table");

		
		init(nullptr, nullptr);
	}

	GameObject::GameObject(const GameObject& cpy)
		: mGAME(cpy.mGAME)
		, DESTROY(false)
		, pmRenderable(cpy.pmRenderable)
		, mCollider(cpy.mCollider)
		, isAnimated{ cpy.isAnimated }
		, mBox{ cpy.mBox } 
		, stagedFunc(cpy.stagedFunc)
		, updateFunc(cpy.updateFunc)
		, unstagedFunc(cpy.unstagedFunc)
	{
	}

	const GameObject& GameObject::operator=(const GameObject& cpy)
	{
		mGAME = cpy.mGAME;
		pmRenderable = cpy.pmRenderable;
		mCollider = cpy.mCollider;
		isAnimated = cpy.isAnimated;
		mBox = cpy.mBox;
		stagedFunc = cpy.stagedFunc;
		updateFunc = cpy.updateFunc;
		unstagedFunc = cpy.unstagedFunc;

		return *this;
	}

	void GameObject::update(F32 dt)
	{
		if (!mCollider && !pmRenderable) return;
		if (isAnimated)
		{
			static_cast<AnimSprite*>(pmRenderable)->update(dt);
			Rectangle& r = static_cast<AnimSprite*>(pmRenderable)->box();

			if (mCollider)
			{
				mBox->setX(mCollider->getX());
				mBox->setY(-mCollider->getY());
				mBox->setOrientation(-mCollider->getOrientation());
				mBox->updateVerts();
			}

			if (updateFunc)
				updateFunc(this, dt);
		}
		else
		{
			static_cast<Sprite*>(pmRenderable)->update(dt);
			Rectangle& r = static_cast<AnimSprite*>(pmRenderable)->box();
			if (mCollider)
			{
				mBox->setX(mCollider->getX());
				mBox->setY(-mCollider->getY());
				mBox->setOrientation(-mCollider->getOrientation());
				mBox->updateVerts();

			}
			if (updateFunc)
				updateFunc(this, dt);
		}

	}

	CollisionCallbackFunc GameObject::getCollisionCallback(I32 collisionWith)
	{
		return mCollisionCallbacks[collisionWith];
	}

	void GameObject::addCollisionCallback(I32 collisionWith, OOLUA::Lua_func_ref ref)
	{
		I32 mycoltype = cpShapeGetCollisionType(mCollider->mShape);
		I32 othercoltype = collisionWith;
		CollisionCallbackFunc func;
		func = ref;
		mCollisionCallbacks.insert(std::pair<I32, CollisionCallbackFunc>(collisionWith, func));
		mData.push_back(new CollisionData{ this, collisionWith });

		cpCollisionHandler* handler = cpSpaceAddCollisionHandler(GAME->getSpace(), cpShapeGetCollisionType(mCollider->mShape), collisionWith);
		handler->userData = (cpDataPointer)mData.back();
		handler->beginFunc = 

		[](cpArbiter *arb, struct cpSpace *space, cpDataPointer data) -> cpBool
		{
			CollisionData* colData = (CollisionData*)data;
			LuaCollisionData luaData = {};
			luaData.normal = vec2(arb->n.x, arb->n.y);
			colData->obj->getCollisionCallback(colData->collisionWith)(colData->obj, &luaData);

			return true;
		};
	}

	void GameObject::addDefaultCollisionCallback(OOLUA::Lua_func_ref ref)
	{
		I32 mycoltype = cpShapeGetCollisionType(mCollider->mShape);
		CollisionCallbackFunc func;
		func = ref;
		mCollisionCallbacks.insert(std::pair<I32, CollisionCallbackFunc>(mycoltype, func));
		mData.push_back(new CollisionData{ this, mycoltype });

		cpCollisionHandler* handler = cpSpaceAddWildcardHandler(GAME->getSpace(), cpShapeGetCollisionType(mCollider->mShape));
		handler->userData = (cpDataPointer)mData.back();
		handler->beginFunc =

			[](cpArbiter *arb, struct cpSpace *space, cpDataPointer data) -> cpBool
		{
			CollisionData* colData = (CollisionData*)data;
			LuaCollisionData luaData = {};
			luaData.normal = vec2(arb->n.x, arb->n.y);
			colData->obj->getCollisionCallback(colData->obj->getCollisionType())(colData->obj, &luaData);

			return true;
		};
	}

	void GameObject::setRenderable(IRenderable* renderable)
	{
		if (pmRenderable)
			delete pmRenderable;

		pmRenderable = renderable;

		if (pmRenderable)
		{
			if (dynamic_cast<Sprite*>(pmRenderable))
			{
				Sprite* ptr = (Sprite*)pmRenderable;
				mBox = &ptr->box();
				isAnimated = false;
			}
			else if (dynamic_cast<AnimSprite*>(pmRenderable))
			{
				AnimSprite* ptr = (AnimSprite*)pmRenderable;
				mBox = &ptr->box();
				isAnimated = true;
			}
			else
			{
				TALGA_ASSERT(0, "unknown renderable passed to GameObject");
			}
		}
		else
		{
			TALGA_WARN(0, "nullptr passed to GameObject");
		}
	}

	void GameObject::setCollider(PhysicsComponent* collider)
	{
		if (mCollider && collider)
		{
			cpSpaceRemoveBody(mGAME->getSpace(), mCollider->mBody);
			cpSpaceRemoveShape(mGAME->getSpace(), mCollider->mShape);
			delete mCollider;
			mCollider = collider;
			cpSpaceAddBody(mGAME->getSpace(), mCollider->mBody);
			cpSpaceAddShape(mGAME->getSpace(), mCollider->mShape);
		}
		else if (mCollider && !collider)
		{
			cpSpaceRemoveBody(mGAME->getSpace(), mCollider->mBody);
			cpSpaceRemoveShape(mGAME->getSpace(), mCollider->mShape);;
			delete mCollider;
			mCollider = collider;
		}
		else if (!mCollider && collider)
		{
			mCollider = collider;
			cpSpaceAddBody(mGAME->getSpace(), mCollider->mBody);
			cpSpaceAddShape(mGAME->getSpace(), mCollider->mShape);
		}
		
	}

	void GameObject::setCollisionType(I32 type)
	{
		TALGA_WARN(mCollider, "tried to set collision type of non exsitent collider")
		
		if (mCollider)
			mCollider->setCollisionType(type);
	}

	I32 GameObject::getCollisionType() const
	{
		if (mCollider)
			return mCollider->getCollisionType();
	}

	void GameObject::addKeyCallback(char c, KeyCallbackFunc cback)
	{
		GAME->addKeyCallback(c, this, cback);
	}

	void GameObject::addKeyCallback(std::string c, OOLUA::Lua_func_ref ref)
	{
		KeyCallbackFunc func;
		func = ref;
		GAME->addKeyCallback(c[0], this, func);
	}

	void GameObject::playAnimation(const std::string& animName, I32 speed, bool loop)
	{
		if (isAnimated)
		{
			static_cast<AnimSprite*>(pmRenderable)->playAnimation(animName, speed, loop);
		}
	}

	void GameObject::destroy()
	{
		if (destroyFunc)
			destroyFunc(this);

		delete pmRenderable;
		delete mCollider;
		for (auto it = mData.begin(); it != mData.end(); ++it)
			delete *it;
	}

	GameObject::~GameObject()
	{
	}
}
