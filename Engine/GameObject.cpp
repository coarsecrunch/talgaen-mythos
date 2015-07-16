#include "GameObject.h"
#include "chipmunk/chipmunk_private.h"
#include "Sprite.h"
#include "Game.h"
#include "collisiontypes.h"
namespace talga
{
	GameObject::GameObject(Game* game, IRenderable* rdr, std::function<void(GameObject*)> tempInitFunc, F32 x, F32 y)
		: GAME(game)
		, DESTROY(false)
		, pmRenderable(rdr)
		, isAnimated{false}
		, mBox{ nullptr }
		, initFunc(tempInitFunc)
	{
		if (rdr)
		{
			if (dynamic_cast<Sprite*>(pmRenderable.get()))
			{
				Sprite* ptr = (Sprite*) pmRenderable.get();
				mBox = &ptr->getBox();
				mBody = cpBodyNew(10.0f, INFINITY);  
				mShape = cpBoxShapeNew(mBody, mBox->getW(), mBox->getH(), 0);
				cpBodySetPosition(mBody, cpv(x, y));
				setFriction(0.94f);
				setCollisionType(COLL_DEFAULT);

				if (initFunc)
				{
					initFunc(this);
				}
			}
			else if (dynamic_cast<AnimSprite*>(pmRenderable.get()))
			{
				AnimSprite* ptr = (AnimSprite*)pmRenderable.get();
				mBox = &ptr->getBox();
				isAnimated = true;
				mBody = cpBodyNew(10.0f, INFINITY);
				mShape = cpBoxShapeNew(mBody, mBox->getW(), mBox->getH(), 0);	
				cpBodySetPosition(mBody, cpv(x, y));
				setFriction(0.94f);
				setCollisionType(COLL_DEFAULT);
				if (initFunc)
				{
					initFunc(this);
				}
			}
		}
		else
		{
			TALGA_ASSERT(0, "nullptr passed to GameObject");
		}
	}

	void GameObject::update(F32 dt)
	{
		if (isAnimated)
		{
			static_cast<AnimSprite*>(pmRenderable.get())->update(dt);
			Rectangle& r = static_cast<AnimSprite*>(pmRenderable.get())->getBox();
			r.setX(cpBodyGetPosition(mBody).x);
			r.setY(-cpBodyGetPosition(mBody).y);
			r.setOrientation(-cpBodyGetAngle(mBody));

			if (updateFunc)
				updateFunc(this, dt);

		}
		else
		{
			static_cast<Sprite*>(pmRenderable.get())->update(dt);
			Rectangle& r = static_cast<AnimSprite*>(pmRenderable.get())->getBox();
			r.setX(cpBodyGetPosition(mBody).x);
			r.setY(-cpBodyGetPosition(mBody).y);
			r.setOrientation(-cpBodyGetAngle(mBody));
			
			if (updateFunc)
				updateFunc(this, dt);
		}
	}

	CollisionCallback GameObject::getCollisionCallback(I32 collisionWith)
	{
		return mCollisionCallbacks[collisionWith];
	}


	void GameObject::addCollisionCallback(I32 collisionWith, CollisionCallback callback)
	{
		I32 mycoltype = cpShapeGetCollisionType(mShape);
		I32 othercoltype = collisionWith;
		mCollisionCallbacks.insert(std::pair<I32, CollisionCallback>(collisionWith, callback));
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

	void GameObject::addKeyCallback(char c, KeyCallback cback)
	{
		GAME->addKeyCallback(c, this, cback);
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
		if (destroyedFunc)
			destroyedFunc(this);

		cpShapeFree(mShape);
		cpBodyFree(mBody);

		for (auto it = mData.begin(); it != mData.end(); ++it)
			delete *it;

	}
}