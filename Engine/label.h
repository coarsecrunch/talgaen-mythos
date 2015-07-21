#pragma once

#include <string>

#include "IRenderable.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace talga
{
	class Label : public IRenderable
	{
	public:
		Label(std::string text, vec3 pos, vec4 color);
		virtual ~Label();

		virtual void render(Renderer* renderer, const Camera* camera) const override;

	private:
		std::string mText;
		vec3 mPosition;
		vec4 mColor;
	};

}