#include "GameObject.h"
#include "chipmunk/chipmunk_private.h"
#include "Sprite.h"
#include "Game.h"
#include "collisiontypes.h"

#include "oolua/oolua_string.h"
#include "LuaEngine.h"
#include "luareg.h"
#include "sys.h"

namespace talga
{
	GameObject::GameObject(IRenderable* rdr, PhysicsComponent* collider)
		: GAME(nullptr)
		, DESTROY(false)
		, pmRenderable(rdr)
		, isAnimated{false}
		, mCollider(collider)
		, mBox{ nullptr }
		, stagedFunc()
		, updateFunc()
		, unstagedFunc()
	{
		if (rdr)
		{
			if (dynamic_cast<Sprite*>(pmRenderable.get()))
			{
				Sprite* ptr = (Sprite*) pmRenderable.get();
				mBox = &ptr->box();
				setCollisionType(COLL_DEFAULT);
			}
			else if (dynamic_cast<AnimSprite*>(pmRenderable.get()))
			{
				AnimSprite* ptr = (AnimSprite*)pmRenderable.get();
				mBox = &ptr->box();
				isAnimated = true;
				setCollisionType(COLL_DEFAULT);
			}
			else
			{
				TALGA_ASSERT(0, "unknown renderable passed to GameObject");
			}
		}
		else
		{
			TALGA_ASSERT(0, "nullptr passed to GameObject");
		}
	}

	GameObject::GameObject(std::string path)
		: GAME(nullptr)
		, DESTROY(false)
		, isAnimated{ false }
		, mBox{ nullptr }
		, stagedFunc()
		, updateFunc()
		, unstagedFunc()
	{
		OOLUA::Lua_func_ref stagedFuncRef;
		OOLUA::Lua_func_ref unstagedFuncRef;
		OOLUA::Lua_func_ref updateFuncRef;
		std::string tblname = getFileNameFromPathWithoutExtension(path);
		talga::LuaEngine::instance()->ExecuteFile(path);

		OOLUA::Table tempTbl(talga::LuaEngine::instance()->getGlobalTable(tblname));
		TALGA_WARN(tempTbl.valid(), "incorrect script at " + path + " the table's name MUST match the name of the script");

		if (!tempTbl.valid())
			return;

		TALGA_MSG("script " + path + " successfully loaded");

		tempTbl.at("stagedFunc", stagedFuncRef);
		tempTbl.at("unstagedFunc", unstagedFuncRef);
		tempTbl.at("updateFunc", updateFuncRef);


		if (stagedFuncRef.valid())
			stagedFunc = stagedFuncRef;
		else
			TALGA_WARN(0, "failed to find stagedFunc in table in " + tblname);

		if (unstagedFuncRef.valid())
			unstagedFunc = unstagedFuncRef;
		else
			TALGA_WARN(0, "failed to find unstagedFunc in table in " + tblname);

		if (updateFuncRef.valid())
			updateFunc = updateFuncRef;
		else
			TALGA_WARN(0, "failed to find updateFunc in table in " + tblname);

	}

	GameObject::GameObject(const GameObject& cpy)
		: GAME(cpy.GAME)
		, DESTROY(false)
		, pmRenderable(cpy.pmRenderable)
		, mCollider(cpy.mCollider)
		, isAnimated{ cpy.isAnimated }
		, mBox{ cpy.mBox } //need to copy functions too
		, stagedFunc(cpy.stagedFunc)
		, updateFunc(cpy.updateFunc)
		, unstagedFunc(cpy.unstagedFunc)
	{
	}

	void GameObject::staged()
	{
		if (stagedFunc)
			stagedFunc(this);

	}

	void GameObject::update(F32 dt)
	{
		if (isAnimated)
		{
			static_cast<AnimSprite*>(pmRenderable.get())->update(dt);
			Rectangle& r = static_cast<AnimSprite*>(pmRenderable.get())->box();

			if (mCollider)
			{
				mBox->setX(mCollider->getX());
				mBox->setY(-mCollider->getY());
				mBox->setOrientation(-mCollider->getOrientation());
			}

			if (updateFunc)
				updateFunc(this, dt);
		}
		else
		{
			static_cast<Sprite*>(pmRenderable.get())->update(dt);
			Rectangle& r = static_cast<AnimSprite*>(pmRenderable.get())->box();
			if (mCollider)
			{
				mBox->setX(mCollider->getX());
				mBox->setY(-mCollider->getY());
				mBox->setOrientation(-mCollider->getOrientation());
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
			colData->obj->getCollisionCallback(colData->collisionWith)(colData->obj);

			return true;
		};
	}

	void GameObject::setCollisionType(I32 type)
	{
		TALGA_ASSERT(mCollider, "tried to set collision type of non exsitent collider")
		mCollider->setCollisionType(type);
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
			static_cast<AnimSprite*>(pmRenderable.get())->playAnimation(animName, speed, loop);
		}
	}

	void GameObject::destroy()
	{
		DESTROY = true;
	}
	GameObject::~GameObject()
	{
		delete mCollider;
		for (auto it = mData.begin(); it != mData.end(); ++it)
			delete *it;
	}
}