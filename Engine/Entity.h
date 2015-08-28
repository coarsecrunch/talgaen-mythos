#pragma once

#include <tuple>
#include "Transformation2D.h"
#include "Component.h"
#include "TemplateNonsense.h"

namespace talga
{
	//class RenderableComponent;
	//class PhysicsComponent;

	template<typename... Args>
	class Entity;
	
	template<>
	class Entity<> {
	public:
		template<typename T>
		T* getComponent()
		{
			return nullptr;
		}
	};

	template<typename Head, typename... Args>

	class Entity<Head, Args...>
	{
		public:
			Entity(Head* head, Args*... tail)
				: mHead(head)
				, mTail(tail...)
			{
				static_assert(std::is_base_of<Component, Head>::value, "tried to instantiate entity with non component typename");
				static_assert(!is_any<Head, Args...>::value, "tried to instantiate entity with duplicate component typenames");

			}

		template<typename T>
		T* getComponent()
		{
			if (std::is_same<T, Head>::value)
				return reinterpret_cast<T*>(mHead);
			else
				return mTail.getComponent<T>();
		}

		Entity() = delete;
		~Entity() { }

	protected:

		Head* mHead;
		Entity<Args...> mTail;
	};
}
