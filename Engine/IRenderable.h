#pragma once
#include <memory>
#include <vector>

namespace talga
{
	class Renderer;
	class Camera;

	class IRenderable
	{
	public:
		virtual void render(Renderer* renderer, const Camera* camera) const = 0;

		virtual ~IRenderable() { clearChildren(); }

		void addChild(const IRenderable* rdr)
		{
			mChildren.push_back(rdr);
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

		const std::vector<const IRenderable*>& getChildren() const { return mChildren; }

	private:
		std::vector<const IRenderable*> mChildren;
	};

	typedef std::shared_ptr<IRenderable> SharedRdrPtr;
	typedef std::shared_ptr<const IRenderable> cSharedRdrPtr;
}