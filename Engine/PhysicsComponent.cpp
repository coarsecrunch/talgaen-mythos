#include "PhysicsComponent.h"
#include "chipmunk/chipmunk_private.h"
namespace talga
{
	RectCollider::RectCollider(F32 width, F32 height, F32 x, F32 y)
		: mWidth(width)
		, mHeight(height)
	{
		mBody = cpBodyNew(10.0f, INFINITY);
		mShape = cpBoxShapeNew(mBody, width, height, 0);
		cpBodySetPosition(mBody, cpv(x, y));
		setFriction(0.94f);
	}

	void RectCollider::setMass(F32 value)
	{
		cpBodySetMass(mBody, value);
		cpBodySetMoment(mBody, cpMomentForBox(cpBodyGetMass(mBody), mWidth, mHeight));
	}
	void PhysicsComponent::setCollisionType(I32 value)
	{
		cpShapeSetCollisionType(mShape, value);
	}
	F32 PhysicsComponent::getOrientation() const
	{
		return cpBodyGetAngle(mBody);
	}
	void PhysicsComponent::applyForceX(F32 x)
	{
		cpBodyApplyForceAtLocalPoint(mBody, cpv(x, cpBodyGetForce(mBody).y), cpvzero);
	}
	void PhysicsComponent::applyForceY(F32 y)
	{
		cpBodyApplyForceAtLocalPoint(mBody, cpv(cpBodyGetForce(mBody).x, y), cpvzero);
	}
	void PhysicsComponent::applyImpulseY(F32 impulse)
	{
		cpBodyApplyImpulseAtLocalPoint(mBody, cpv(0, impulse), cpvzero);
	}
	void PhysicsComponent::applyImpulseX(F32 impulse)
	{
		cpBodyApplyImpulseAtLocalPoint(mBody, cpv(impulse, 0), cpvzero);
	}
	F32 PhysicsComponent::getVx() const
	{
		return cpBodyGetForce(mBody).x;
	}
	F32 PhysicsComponent::getVy() const
	{
		return cpBodyGetForce(mBody).y;
	}
	F32 PhysicsComponent::getX() const
	{
		return cpBodyGetPosition(mBody).x;
	}
	F32 PhysicsComponent::getY() const
	{
		return cpBodyGetPosition(mBody).y;
	}
	F32 PhysicsComponent::getMass() const
	{
		return cpBodyGetMass(mBody);
	}
	void PhysicsComponent::setX(F32 value)
	{
		cpBodySetPosition(mBody, cpv(value, getY()));
	}
	void PhysicsComponent::setY(F32 value)
	{
		cpBodySetPosition(mBody, cpv(getX(), value));
	}
	void PhysicsComponent::setFriction(F32 value)
	{
		cpShapeSetFriction(mShape, value);
	}
	F32 PhysicsComponent::getFriction() const
	{
		return cpShapeGetFriction(mShape);
	}
	void PhysicsComponent::setMoment(F32 value)
	{
		cpBodySetMoment(mBody, value);
	}
	F32 PhysicsComponent::getMoment() const
	{
		return cpBodyGetMoment(mBody);
	}
	PhysicsComponent::~PhysicsComponent()
	{
		cpShapeFree(mShape);
		cpBodyFree(mBody);
	}
}