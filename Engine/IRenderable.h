#pragma once
#include <memory>
#include <vector>
#include "Rectangle.h"
#include "Renderer.h"

namespace talga
{
	class Renderer;
	class Camera;

	class IRenderable
	{
	public:
		virtual void render(Renderer* renderer, const Camera* camera) const
		{
			renderer->tStackPush(mBox.getTransformationMatrix());
			for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
				(*it)->render(renderer, camera);
			renderer->tStackPop();
		}

		IRenderable() : pmParent(nullptr), mChildren(), mBox(-1, -1) {}

		virtual ~IRenderable() { clearChildren(); }

		void addChild(IRenderable* rdr)
		{
			mChildren.push_back(rdr);
			const_cast<IRenderable*>(rdr)->pmParent = this;
		}

		void removeChild(const IRenderable* rdr)
		{
			for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
			{
				if (*it == rdr && it != mChildren.begin())
				{
					delete *it;
					*it = mChildren.back();
					mChildren.pop_back();
				}
				else if (it == mChildren.begin())
				{
					delete *it;
					mChildren.pop_back();
				}
			}
		}

		void clearChildren()
		{
			for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
			{
				delete *it;
			}

			mChildren.clear();
		}

		const IRenderable* getParent() { return pmParent; }
		const std::vector<const IRenderable*>& getChildren() const { return mChildren; }
		const Rectangle& box() const { return mBox; }
		Rectangle& box() { return mBox; }

		vec3 getGlobalPosition() const
		{
			vec3 pos = mBox.getPosition();

			const IRenderable* parent = pmParent;
			
			while (parent)
			{
				pos = parent->box().getTransformationMatrix() * pos;
				parent = parent->pmParent;
			}

			return pos;
		}
	private:
		Rectangle mBox;
		std::vector<const IRenderable*> mChildren;
		const IRenderable* pmParent;
	};

	typedef std::shared_ptr<IRenderable> SharedRdrPtr;
	typedef std::shared_ptr<const IRenderable> cSharedRdrPtr;
}