#pragma once

#include "Cmn.h"

struct cpBody;
struct cpShape;

namespace talga
{
	class PhysicsComponent
	{
	public:
		
		virtual ~PhysicsComponent();

		F32 getMass() const;
		virtual void setMass(F32 value) = 0;
		void setFriction(F32 value);
		F32 getFriction() const;
		void setMoment(F32 value);
		F32 getMoment() const;

		void applyForceX(F32 x);
		void applyForceY(F32 y);
		void applyImpulseY(F32 impulse);
		void applyImpulseX(F32 impulse);
		void setX(F32 value);
		void setY(F32 value);
		F32 getVx() const;
		F32 getVy() const;
		F32 getX() const;
		F32 getY() const;
		F32 getOrientation() const;

		void setCollisionType(I32 type);
	protected:
		friend class GameObject;
		friend class Game;

		cpBody* mBody;
		cpShape* mShape;
	};

	class RectCollider : public PhysicsComponent
	{
	public:
		RectCollider(F32 width, F32 height, F32 x = 0.0f, F32 y = 0.0f);

		virtual void setMass(F32 value) override;
	protected:
		F32 mWidth;
		F32 mHeight;
	};

}