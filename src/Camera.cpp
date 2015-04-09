#include "SDL2\SDL.h"
#include "SDL2\SDL_image.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "Camera.h"
#include "Game.h"
#include "Map.h"
#include "Rect.h"
#include "Rendering.h"
#include <string>
#include <iostream>
#include "Matrix3x3.h"
#include "AssetManager.h"
#include "Animation.h"

namespace talga
{
	GLFWwindow* Camera::mWindow = nullptr;
	U32 Camera::mVAOs[numVAOs] = {};
	U32 Camera::mBuffers[numBuffers] = {};
	U32 Camera::mProgram = 0;
	I32 Camera::MVPShaderLoc = -1;


	U32 Camera::mColliderProgram = 0;
	I32 Camera::mColliderModelLoc = -1;
	I32 Camera::mColliderViewLoc = -1;
	I32 Camera::mColliderProjectionLoc = -1;
	I32 Camera::mColorUnifromLoc = -1;
	I32 Camera::mBigBellyLoc = -1;
	I32 Camera::mREDLoc = -1;


	//glfw error callback
	void error_callback(int code, const char* message)
	{
		std::cout << "GLFW error " << code << ": " << message << std::endl;
	}

	Camera::Camera(Game* game, vec4 position)
		: AObj(game, -1, -1, position)
		, mProjectionMatrix()
	{
		mProjectionMatrix = mat4((2.0f / (mWidth - 0.0f)), 0.0f, 0.0f, 0.0f, // -(right + left) / (right - left)
			0.0f, (2.0f / (0.0f - mHeight)), 0.0f, 0.0f, // -(top + bottom) / (top - bottom)
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	void Camera::setW(I32 value)
	{
		AObj::setW(value);

		mProjectionMatrix = mat4((2.0f / (mWidth - 0.0f)), 0.0f, 0.0f, 0.0f, // -(right + left) / (right - left)
			0.0f, (2.0f / (0.0f - mHeight)), 0.0f, 0.0f, // -(top + bottom) / (top - bottom)
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	void Camera::setH(I32 value)
	{
		AObj::setH(value);

		mProjectionMatrix = mat4((2.0f / (mWidth - 0.0f)), 0.0f, 0.0f, 0.0f, // -(right + left) / (right - left)
			0.0f, (2.0f / (0.0f - mHeight)), 0.0f, 0.0f, // -(top + bottom) / (top - bottom)
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	int Camera::InitRendering(int width, int height)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			//error
			std::cout << "Error SDL_Init(): " << SDL_GetError() << std::endl;
			return -1;
		}

		int img_flags = IMG_INIT_PNG;

		if (!(IMG_Init(img_flags) & img_flags))
		{
			std::cout << "Error Img_Init(): " << IMG_GetError() << std::endl;
			return -1;
		}

		glfwSetErrorCallback(error_callback);

		if (!glfwInit())
		{
			std::cout << "Failed to initialize GLFW: " << std::endl;
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		mWindow = glfwCreateWindow(width, height, "Talgaen Mythos", NULL, NULL);

		glfwMakeContextCurrent(mWindow);
		if (!mWindow)
		{
			glfwTerminate();

			return -1;
		}

		glewExperimental = GL_TRUE;

		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			std::cout << "error: " << glewGetErrorString(err) << std::endl;
			system("pause");
			return -1;
		}

		glGenVertexArrays(numVAOs, mVAOs);
		glBindVertexArray(mVAOs[TEXTURED_QUAD]);
		std::string vert;
		std::string frag;

		LoadShaderSrc("../../assets/shaders/tri.vert", vert);
		LoadShaderSrc("../../assets/shaders/tri.frag", frag);

		const char* vertSrc = vert.c_str();
		const char* fragSrc = frag.c_str();

		mProgram = LoadCompileShaders(vertSrc, fragSrc);

		MVPShaderLoc = glGetUniformLocation(mProgram, "MVP");
		glUseProgram(mProgram);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glBindVertexArray(0);

		//FOR DRAW QUAD

		glBindVertexArray(mVAOs[COLORED_QUAD]);

		std::string colliderVert;
		std::string colliderFrag;

		LoadShaderSrc("../../assets/shaders/collider.vert", colliderVert);
		LoadShaderSrc("../../assets/shaders/collider.frag", colliderFrag);

		const char* colliderVertSrc = colliderVert.c_str();
		const char* colliderFragSrc = colliderFrag.c_str();

		mColliderProgram = LoadCompileShaders(colliderVertSrc, colliderFragSrc);
		mColorUnifromLoc = glGetUniformLocation(mColliderProgram, "quadColor");
		mColliderModelLoc = glGetUniformLocation(mColliderProgram, "model");
		mColliderViewLoc = glGetUniformLocation(mColliderProgram, "view");
		mColliderProjectionLoc = glGetUniformLocation(mColliderProgram, "projection");

		mREDLoc = glGetUniformLocation(mColliderProgram, "RED");


		vec3 vPos[4] = { vec3(-0.5, 0.5), vec3(0.5f, 0.5f),
			vec3(-0.5f, -0.5f), vec3(0.5f, -0.5f) };

		U16 indices[6] = { 0, 2, 3, 3, 1, 0 };

		glGenBuffers(1, &mBuffers[ARRAY_BUFFER_COLLIDER_QUAD]);
		glGenBuffers(1, &mBuffers[ELEMENT_BUFFER_COLLIDER_QUAD]);
		glBindBuffer(GL_ARRAY_BUFFER, mBuffers[ARRAY_BUFFER_COLLIDER_QUAD]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 4, vPos, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[ELEMENT_BUFFER_COLLIDER_QUAD]);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(U16) * 6, indices, GL_STATIC_DRAW);

		I32 offseto = (U64)&vPos[0][0] - (U64)vPos;
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (const void*)offseto);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		return 0;
	}

	void Camera::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Camera::Render()
	{
		glViewport(0, 0, mWidth, mHeight);

		RenderTiles();
		RenderObjs();

	}

	void Camera::RenderObj(const ARenderableObj& obj, const AssetManager* man) const
	{
		glBindVertexArray(mVAOs[TEXTURED_QUAD]);
		glUseProgram(mProgram);
		glViewport(0, 0, mWidth, mHeight);
		glBindVertexArray(mVAOs[TEXTURED_QUAD]);

		obj.getAnimation()->tex->Bind();

		mat4 fix = obj.getMTWMat();

		fix = getCameraMat();

		fix = mProjectionMatrix * getCameraMat() * obj.getMTWMat();

		glUniformMatrix4fv(MVPShaderLoc, 1, GL_TRUE, &fix[0][0]);

		unsigned int offset = obj.getAnimation()->getOffset(obj.getFrame());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void*)offset);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glFlush();
	}

	void Camera::RenderTiles()
	{
		glBindVertexArray(mVAOs[TEXTURED_QUAD]);
		glUseProgram(mProgram);
		glViewport(0, 0, mWidth, mHeight);


		mGame->getCurrentMap().getSheet()->tex->Bind();

		mat4 fix;

		I32 tileY = mGame->getCurrentMap().CartToTile(getX(), getY()).y;
		I32 tileX = mGame->getCurrentMap().CartToTile(getX(), getY()).x;

		for (I32 y = tileY; y <= mHeight / mGame->getCurrentMap().getTileHeight() + tileY; ++y)
		{
			for (I32 x = tileX; x <= mWidth / mGame->getCurrentMap().getTileWidth() + tileX; ++x)
			{
				if (!mGame->getCurrentMap().Exists(x, y))
					continue;

				U32 offset = mGame->getCurrentMap().getSheet()->getOffset(mGame->getCurrentMap().getTileIndex(x, y));

				fix = mProjectionMatrix * getCameraMat() * mat4(1.0f, 0.0f, 0.0f, mGame->getCurrentMap().TileToCart(x, y).x + (0.5f * mGame->getCurrentMap().getTileWidth()),
					0.0f, 1.0f, 0.0f, mGame->getCurrentMap().TileToCart(x, y).y + (mGame->getCurrentMap().getTileHeight() * 0.5f),
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);

				glUniformMatrix4fv(MVPShaderLoc, 1, GL_TRUE, &fix[0][0]);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void*)offset);
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glFlush();
	}


	void Camera::Update(F32 dt)
	{
		updateCameraScaleMat();
		updateCameraRotMat();
		updateCameraTransMat();
		updateCameraMat();
	}

	void Camera::RenderObjs()
	{
	}


	void Camera::Destroy()
	{
	}

	const mat4& Camera::getCameraMat() const
	{
		return mCameraMat;
	}


	void Camera::updateCameraScaleMat()
	{
		mCameraScaleMat = mat4((1.0f / mScaleX), 0.0f, 0.0f, 0.0f,
			0.0f, (1.0f / mScaleY), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	void Camera::updateCameraRotMat()
	{
		mCameraRotMat = mat4(cos(mRotation), sin(mRotation), 0.0f, 0.0f,
			-sin(mRotation), cos(mRotation), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	void Camera::updateCameraTransMat()
	{
		mCameraTransMat = mat4(1.0f, 0.0f, 0.0f, (-getX() + (-0.5 * mWidth)),
			0.0f, 1.0f, 0.0f, (-getY() + (-0.5 * mHeight)),
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	void Camera::updateCameraMat()
	{
		mCameraMat = mCameraScaleMat * mCameraRotMat * mCameraTransMat;
	}

	void Camera::RenderQuad(const vec4& pos, const vec4& color, const vec4& size) const
	{
		glBindVertexArray(mVAOs[COLORED_QUAD]);
		glUseProgram(mColliderProgram);
		glViewport(0, 0, mWidth, mHeight);

		mat4 fix = mat4(1.0f, 0.0f, 0.0f, pos(0) + (size(2) * 0.5f),
			0.0f, 1.0f, 0.0f, pos(1) + (size(3) * 0.5f),
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f)
			* mat4(size(2), 0.0f, 0.0f, 0.0f,
			0.0f, size(3), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);


		glUniform1fv(mColorUnifromLoc, 4, &color[0]);
		glUniformMatrix4fv(mColliderModelLoc, 1, GL_TRUE, &fix[0][0]);
		glUniformMatrix4fv(mColliderViewLoc, 1, GL_TRUE, &mCameraMat[0][0]);
		glUniformMatrix4fv(mColliderProjectionLoc, 1, GL_TRUE, &mProjectionMatrix[0][0]);



		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void*)0);
		glFlush();
		glBindVertexArray(0);
	}

	Camera::~Camera()
	{
	}


	void Camera::RenderCollider(PhysSprite& collider)
	{
		if (collider.isColliding())
			RenderQuad(collider.getPos(), vec4(0.0f, 0.0f, 1.0f, 0.6f), vec4(0.0f, 0.0f, collider.getW(), collider.getH()));
		else
			RenderQuad(collider.getPos(), vec4(1.0f, 0.0f, 0.0f, 0.6f), vec4(0.0f, 0.0f, collider.getW(), collider.getH()));
	}
}