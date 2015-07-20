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
	GameObject::GameObject(IRenderable* rdr, std::function<void(GameObject*)> tempInitFunc, F32 x, F32 y)
		: GAME(nullptr)
		, DESTROY(false)
		, pmRenderable(rdr)
		, isAnimated{false}
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
				mBody = cpBodyNew(10.0f, INFINITY);  
				mShape = cpBoxShapeNew(mBody, mBox->getW(), mBox->getH(), 0);
				cpBodySetPosition(mBody, cpv(x, y));
				setFriction(0.94f);
				setCollisionType(COLL_DEFAULT);
			}
			else if (dynamic_cast<AnimSprite*>(pmRenderable.get()))
			{
				AnimSprite* ptr = (AnimSprite*)pmRenderable.get();
				mBox = &ptr->box();
				isAnimated = true;
				mBody = cpBodyNew(10.0f, INFINITY);
				mShape = cpBoxShapeNew(mBody, mBox->getW(), mBox->getH(), 0);	
				cpBodySetPosition(mBody, cpv(x, y));
				setFriction(0.94f);
				setCollisionType(COLL_DEFAULT);
			}
		}
		else
		{
			TALGA_ASSERT(0, "nullptr passed to GameObject");
		}
	}

	GameObject::GameObject(const GameObject& cpy)
		: GAME(cpy.GAME)
		, DESTROY(false)
		, pmRenderable(cpy.pmRenderable)
		, isAnimated{ cpy.isAnimated }
		, mBox{ cpy.mBox } //need to copy functions too
		, stagedFunc(cpy.stagedFunc)
		, updateFunc(cpy.updateFunc)
		, unstagedFunc(cpy.unstagedFunc)
	{
	}

	void GameObject::update(F32 dt)
	{
		if (isAnimated)
		{
			static_cast<AnimSprite*>(pmRenderable.get())->update(dt);
			Rectangle& r = static_cast<AnimSprite*>(pmRenderable.get())->box();
			r.setX(cpBodyGetPosition(mBody).x);
			r.setY(-cpBodyGetPosition(mBody).y);
			r.setOrientation(-cpBodyGetAngle(mBody));

			if (updateFunc)
				updateFunc(this, dt);

		}
		else
		{
			static_cast<Sprite*>(pmRenderable.get())->update(dt);
			Rectangle& r = static_cast<AnimSprite*>(pmRenderable.get())->box();
			r.setX(cpBodyGetPosition(mBody).x);
			r.setY(-cpBodyGetPosition(mBody).y);
			r.setOrientation(-cpBodyGetAngle(mBody));
			
			if (updateFunc)
				updateFunc(this, dt);
		}
	}

	void GameObject::loadScript(std::string path)
	{
		OOLUA::Lua_func_ref stagedFuncRef;
		OOLUA::Lua_func_ref unstagedFuncRef;
		OOLUA::Lua_func_ref updateFuncRef;
		OOLUA::Lua_table_ref keyCallbacksTbl;
		std::string tblname = getFileNameFromPathWithoutExtension(path);
		talga::LuaEngine::instance()->ExecuteFile(path);

		OOLUA::Table tempTbl(talga::LuaEngine::instance()->getGlobalTable(tblname));
		TALGA_WARN(tempTbl.valid(), "incorrect script at " + path + " the table's name MUST match the name of the script" );
		
		if (!tempTbl.valid())	
			return;

		TALGA_MSG("script " + path + " successfully loaded");

		tempTbl.at("stagedFunc", stagedFuncRef);
		tempTbl.at("unstagedFunc", unstagedFuncRef);
		tempTbl.at("updateFunc", updateFuncRef);
		tempTbl.at("keyCallbacks", keyCallbacksTbl);

		
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
		
		if (keyCallbacksTbl.valid())
		{
			OOLUA::Table keyCallbacksTable(keyCallbacksTbl);

			oolua_pairs(keyCallbacksTable)
			{
				OOLUA::Lua_func_ref trythis;
				std::string key;
				OOLUA::pull(keyCallbacksTable.state(), trythis);
				OOLUA::pull(keyCallbacksTable.state(), key);


				KeyCallbackFunc func;
				func = trythis;

				char c = key[0];

				addKeyCallback(key[0], func);

				OOLUA::push(keyCallbacksTable.state(), key);
			}
			oolua_pairs_end()

		}
		else
		{
			TALGA_WARN(0, "failed to find keyCallbacks table in " + tblname)
		}
	}

	CollisionCallbackFunc GameObject::getCollisionCallback(I32 collisionWith)
	{
		return mCollisionCallbacks[collisionWith];
	}

	void GameObject::addCollisionCallback(I32 collisionWith, OOLUA::Lua_func_ref ref)
	{
		I32 mycoltype = cpShapeGetCollisionType(mShape);
		I32 othercoltype = collisionWith;
		CollisionCallbackFunc func;
		func = ref;
		mCollisionCallbacks.insert(std::pair<I32, CollisionCallbackFunc>(collisionWith, func));
		mData.push_back(new CollisionData{ this, collisionWith });

		cpCollisionHandler* handler = cpSpaceAddCollisionHandler(GAME->getSpace(), cpShapeGetCollisionType(mShape), collisionWith);
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
		cpShapeSetCollisionType(mShape, type);
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

	void GameObject::applyForce(vec2 force)
	{
		cpBodyApplyForceAtLocalPoint(mBody, cpv(force[0], force[1]), cpvzero);
	}
	void GameObject::applyForceX(F32 x)
	{
		cpBodyApplyForceAtLocalPoint(mBody, cpv(x, cpBodyGetForce(mBody).y), cpvzero);
	}
	void GameObject::applyForceY(F32 y)
	{
		cpBodyApplyForceAtLocalPoint(mBody, cpv(cpBodyGetForce(mBody).x, y), cpvzero);
	}
	void GameObject::applyImpulseY(F32 impulse)
	{
		cpBodyApplyImpulseAtLocalPoint(mBody, cpv(0, impulse), cpvzero);
	}
	void GameObject::applyImpulseX(F32 impulse)
	{
		cpBodyApplyImpulseAtLocalPoint(mBody, cpv(impulse, 0), cpvzero);
	}
	F32 GameObject::getVx() const
	{
		return cpBodyGetForce(mBody).x;
	}
	F32 GameObject::getVy() const
	{
		return cpBodyGetForce(mBody).y;
	}
	void GameObject::playAnimation(const std::string& animName, I32 speed, bool loop)
	{
		if (isAnimated)
		{
			static_cast<AnimSprite*>(pmRenderable.get())->playAnimation(animName, speed, loop);
		}
	}
	F32 GameObject::getMass() const
	{ 
		return cpBodyGetMass(mBody); 
	}
	void GameObject::setMass(F32 value)
	{ 
		cpBodySetMass(mBody, value); 
		cpBodySetMoment(mBody, cpMomentForBox(cpBodyGetMass(mBody), mBox->getW(), mBox->getH()));
	}
	void GameObject::setFriction(F32 value)
	{
		cpShapeSetFriction(mShape, value);
	}
	F32 GameObject::getFriction() const
	{
		return cpShapeGetFriction(mShape);
	}
	void GameObject::setMoment(F32 value)
	{
		cpBodySetMoment(mBody, value);
	}
	F32 GameObject::getMoment() const
	{
		return cpBodyGetMoment(mBody);
	}
	void GameObject::destroy()
	{
		DESTROY = true;
	}
	GameObject::~GameObject()
	{
		cpShapeFree(mShape);
		cpBodyFree(mBody);

		for (auto it = mData.begin(); it != mData.end(); ++it)
			delete *it;
	}
}