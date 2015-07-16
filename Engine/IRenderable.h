#pragma once
#include <memory>

namespace talga
{
	class Renderer;
	class Camera;

	class IRenderable
	{
	public:
		virtual void render(Renderer* renderer, const Camera* camera) const = 0;

		virtual ~IRenderable() {}
	};

	typedef std::shared_ptr<IRenderable> SharedRdrPtr;
	typedef std::shared_ptr<const IRenderable> cSharedRdrPtr;
}