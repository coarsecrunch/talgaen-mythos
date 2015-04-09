#include "Renderer.h"
#include "GL\glew.h"

namespace talga
{
	Renderer::Renderer()
	{
	}

	void Renderer::Init()
	{
		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);
		glGenBuffers(1, &mIBO);
		
		glBindVertexArray(mVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, RENDERER_INDICIES_SIZE, NULL, GL_DYNAMIC_DRAW);

		glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(SHADER_POSITION_LOC);
		glEnableVertexAttribArray(SHADER_COLOR_LOC);
		glEnableVertexAttribArray(SHADER_UV_LOC);

		glVertexAttribPointer(SHADER_POSITION_LOC, 4, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const void*) offsetof(VertexData, VertexData::position));
		glVertexAttribPointer(SHADER_COLOR_LOC, 4, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const void*)offsetof(VertexData, VertexData::color));
		glVertexAttribPointer(SHADER_UV_LOC, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const void*)offsetof(VertexData, VertexData::uv));

		glBindVertexArray(0);
	}
	
	void Renderer::begin()
	{
		mNextVertex = (VertexData*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void Renderer::submit(const ARenderableObj& obj)
	{
		
		mIndexCount += 6;
	}
	
	void Renderer::render()
	{

	}

	void Renderer::end()
	{
		glUnmapBuffer(mVBO);
	}

	Renderer::~Renderer()
	{
		glDeleteBuffers(1, &mVBO);
		glDeleteBuffers(1, &mIBO);
		glDeleteVertexArrays(1, &mVAO);
	}
}