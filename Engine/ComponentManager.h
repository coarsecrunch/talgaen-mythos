#pragma once

#include <vector>
#include <array>
#include "Cmn.h"

namespace talga
{
	template<typename... Args>
	class ComponentManagerTuple;

	template<>
	class ComponentManagerTuple<>
	{};

	constexpr I32 MAX_COMPONENTS = 10000;

	template<typename Head, typename... Args>
	class ComponentManagerTuple 
	{
	public:
		ComponentManager()
		{
			mComponents.reserve(MAX_COMPONENTS);
		}
		~ComponentManager();
	
	protected:
		std::vector<Head> mComponents;
		ComponentManager<Args...> tail;
	};
}