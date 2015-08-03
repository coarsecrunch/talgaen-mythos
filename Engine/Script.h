#pragma once

#include "AAsset.h"

namespace talga
{
	class Script : public AAsset
	{
	public:
		virtual bool load(std::string path, AssetManager& manager) override;
		virtual bool save(std::string path, AssetManager& manager) override;
		virtual void destroy() override;
		
		void execute() const;

		Script();
		~Script();
	private:
	};
}