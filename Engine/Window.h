#pragma once

#include "Cmn.h"
struct GLFWwindow;

namespace talga
{
	/* Window should only ever be created once
	calling the constructor a second time will caues
	undefined behaviour because of the glfw
	and glew initialization */
	class Window
	{
	public:
		static void initGL();

		Window(I32 width, I32 height);
		~Window();

		GLFWwindow* getWindow() { return mWindow; }
		void swap();
		void clear();
	protected:
		GLFWwindow* mWindow;
		I32 mWidth;
		I32 mHeight;
	};
}