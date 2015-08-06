#include "Window.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"


namespace talga
{
	Window::Window(I32 width, I32 height)
		: mWindow(nullptr)
		, mWidth(width)
		, mHeight(height)
	{
		const int glfwState = glfwInit();
		TALGA_ASSERT(glfwState, "Failed to initialize glfw, exiting...");

		mWindow = glfwCreateWindow(mWidth, mHeight, "Talgaen Mythos", NULL, NULL);
		
		
		if (!mWindow)
		{
			glfwTerminate();
			TALGA_ASSERT(0, "Failed to make window, exiting...");
		}

		glfwMakeContextCurrent(mWindow);
		glfwSwapInterval(1);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		TALGA_ASSERT(err == GLEW_OK, "Failed to initialize glew, exiting...");

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Window::initGL()
	{	//obselete
		//TALGA_ASSERT(glfwInit(), "Failed to initialize glfw, exiting...");
	}

	void Window::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::swap()
	{
		glfwSwapBuffers(mWindow);
	}

	Window::~Window()
	{
		glfwTerminate();
	}
}
