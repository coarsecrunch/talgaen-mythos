#include <GL/glew.h>
#include "Renderer.h"
#include "Rendering.h"
#include "Rectangle.h"
#include "Texture.h"
#include "AssetManager.h"
#include <iostream>

#include "ext/freetype-gl/freetype-gl.h"

namespace talga
{
	Renderer::Renderer(const char* vertexShaderPath, const char* fragmentShaderPath)
        : mVAO{0}
        , mVBO{0}
        , mIBO{0}
	{
		std::string vertSrc;
		LoadShaderSrc(vertexShaderPath, vertSrc);
		
		std::string fragSrc;
		LoadShaderSrc(fragmentShaderPath, fragSrc);

		mProgram = LoadCompileShaders(vertSrc.c_str(), fragSrc.c_str());
		mTransformationStack.push(mat4::identity());
		Init();

		mFTAtlas = ftgl::texture_atlas_new(512, 512, 1);
		mFTFont =  ftgl::texture_font_new_from_file(mFTAtlas, 30, "../assets/fonts/arial.ttf");
		
		ftgl::texture_font_get_glyph(mFTFont, 'A');
		ftgl::texture_font_get_glyph(mFTFont, 'B');
		ftgl::texture_font_get_glyph(mFTFont, 'a');
		ftgl::texture_font_get_glyph(mFTFont, '$');
		ftgl::texture_font_get_glyph(mFTFont, '%');
		ftgl::texture_font_get_glyph(mFTFont, '^');
		ftgl::texture_font_get_glyph(mFTFont, '8');
	}

	void Renderer::Init()
	{
		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);
		glGenBuffers(1, &mVBO);
		glGenBuffers(1, &mIBO);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * RENDERER_BUFFER_SIZE, NULL, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

		GLuint* indices = new GLuint[RENDERER_INDICIES_SIZE];

		int offset = 0;
		for (int i = 0; i < RENDERER_INDICIES_SIZE; i += 6)
		{
			indices[i] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 4] = offset + 2;
			indices[i + 3] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, RENDERER_INDICIES_SIZE * sizeof(U32), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(SHADER_POSITION_LOC);
		glEnableVertexAttribArray(SHADER_COLOR_LOC);
		glEnableVertexAttribArray(SHADER_UV_LOC);
		glEnableVertexAttribArray(SHADER_TID_LOC);
		glEnableVertexAttribArray(SHADER_TRANSPARENCYSCALE_LOC);

		vec3 testOffsetToFirstElementVec3;
		vec4 testOffsetToFirstElementVec4;
		vec2 testOffsetToFirstElementVec2;

		I32 posOffset = (offsetof(VertexData, VertexData::position) + ((U64)&testOffsetToFirstElementVec3[0] - (U64)&testOffsetToFirstElementVec3));
		I32 colorOffset = (offsetof(VertexData, VertexData::color) + ((U64)&testOffsetToFirstElementVec4[0] - (U64)&testOffsetToFirstElementVec4));
		I32 uvOffset = (offsetof(VertexData, VertexData::uv) + ((U64)&testOffsetToFirstElementVec2[0] - (U64)&testOffsetToFirstElementVec2));
		I32 tidOffset = offsetof(VertexData, VertexData::tid);
		I32 transparencyOffset = offsetof(VertexData, VertexData::transparencyScale);

		glVertexAttribPointer(SHADER_POSITION_LOC, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const void*)posOffset);
		glVertexAttribPointer(SHADER_COLOR_LOC, 4, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const void*)colorOffset);
		glVertexAttribPointer(SHADER_UV_LOC, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const void*)uvOffset);
		glVertexAttribPointer(SHADER_TID_LOC, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const void*)tidOffset);
		glVertexAttribPointer(SHADER_TRANSPARENCYSCALE_LOC, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const void*)transparencyOffset);

		I32 samplers2Dnums[RENDERER_MAX_ACTIVE_TEXTURES] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

		glUseProgram(mProgram);

		I32 setTextures = glGetUniformLocation(mProgram, "textures");

		glUniform1iv(setTextures, 32, samplers2Dnums);

		glBindVertexArray(0);

		mTextureSlots.reserve(RENDERER_MAX_ACTIVE_TEXTURES);

		delete[] indices;
	}
	
	void Renderer::begin()
	{
		glBindVertexArray(mVAO);

		mNextVertex = (VertexData*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        auto err = glGetError();
    }

	//Verts must already be transformed before submit is called
	//
	void Renderer::submit(const Rectangle& imageBox, cpTex tex, F32 transparencyScale, UVFrame frame)
	{
		vec4 color = imageBox.getColor();

		float texId = -1;
		bool found = false;
		
		//if (!tex) 
			//tex = AssetManager::NO_TEXTURE;
		
		if (tex)
		{
			if (tex->id() > 0)
			{
				for (int i = 0; i < mTextureSlots.size(); ++i)
				{
					texId = i;
					if (mTextureSlots[i] == tex->id())
					{
						found = true;
						break;
					}
				}

				if (!found)
				{
					if (mTextureSlots.size() > RENDERER_MAX_ACTIVE_TEXTURES)
					{
						end();
						render();
						begin();
					}
					texId = mTextureSlots.size();
					mTextureSlots.push_back(tex->id());
				}
	
			}
		}

		mNextVertex->position = mTransformationStack.top() * imageBox.getVerts()[0];
		mNextVertex->color = imageBox.getColor();
		mNextVertex->uv = frame[0];
		mNextVertex->tid = texId;
		mNextVertex->transparencyScale = transparencyScale;
		++mNextVertex;

		mNextVertex->position = mTransformationStack.top() * imageBox.getVerts()[1];
		mNextVertex->color = imageBox.getColor();
		mNextVertex->uv = frame[1];
		mNextVertex->tid = texId;
		mNextVertex->transparencyScale = transparencyScale;
		++mNextVertex;
		
		mNextVertex->position = mTransformationStack.top() * imageBox.getVerts()[2];
		mNextVertex->color = imageBox.getColor();
		mNextVertex->uv = frame[2];
		mNextVertex->tid = texId;
		mNextVertex->transparencyScale = transparencyScale;
		++mNextVertex;
		
		mNextVertex->position = mTransformationStack.top() * imageBox.getVerts()[3];
		mNextVertex->color = imageBox.getColor();
		mNextVertex->uv = frame[3];
		mNextVertex->tid = texId;
		mNextVertex->transparencyScale = transparencyScale;
		++mNextVertex;
		
		mIndexCount += 6;
	}

	void Renderer::drawString(const std::string& str, vec3 pos, vec4 color)
	{
		using namespace ftgl;

		pos = vec3(100, 100);

		float texId = mFTAtlas->id;
		bool found = false;

		for (int i = 0; i < mTextureSlots.size(); ++i)
		{
			texId = i;
			if (mTextureSlots[i] == mFTAtlas->id)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			if (mTextureSlots.size() > RENDERER_MAX_ACTIVE_TEXTURES)
			{
				end();
				render();
				begin();
			}
			texId = mTextureSlots.size();
			mTextureSlots.push_back(mFTAtlas->id);
		}


		for (int i = 0; i < str.length(); ++i)
		{
			char c = str[i];
			texture_glyph_t* glyph = texture_font_get_glyph(mFTFont, c);

			if (glyph)
			{
				if (i > 0)
				{
					F32 kerning = texture_glyph_get_kerning(glyph, str[i - 1]);
					pos[0] += kerning;
				}
				F32 halfwidth = mFTAtlas->width * 0.5f;
				F32 halfheight = mFTAtlas->height * 0.5f;

				F32 x0 = pos.x() + glyph->offset_x;
				F32 y0 = pos.y() - glyph->offset_y;

				F32 x1 = x0 + glyph->width;
				F32 y1 = y0 + glyph->height;

				F32 u0 = glyph->s0;
				F32 v0 = glyph->t0;

				F32 u1 = glyph->s1;
				F32 v1 = glyph->t1;

				mNextVertex->position = mTransformationStack.top() * vec3(x0, y0);
				mNextVertex->uv = vec2(u0, v0);
				mNextVertex->color = color;
				mNextVertex->tid = texId;
				mNextVertex->transparencyScale = 1.0f;
				++mNextVertex;


				mNextVertex->position = mTransformationStack.top() * vec3(x1, y0);
				mNextVertex->uv = vec2(u1, v0);
				mNextVertex->color = color;
				mNextVertex->tid = texId;
				mNextVertex->transparencyScale = 1.0f;
				++mNextVertex;

				mNextVertex->position = mTransformationStack.top() * vec3(x1, y1);
				mNextVertex->uv = vec2(u1, v1);
				mNextVertex->color = color;
				mNextVertex->tid = texId;
				mNextVertex->transparencyScale = 1.0f;
				++mNextVertex;

				mNextVertex->position = mTransformationStack.top() * vec3(x0, y1);
				mNextVertex->uv = vec2(u0, v1);
				mNextVertex->color = color;
				mNextVertex->tid = texId;
				mNextVertex->transparencyScale = 1.0f;
				++mNextVertex;

				pos[0] += glyph->advance_x;
				pos[1] += glyph->advance_y;
				if (c == '\n')
					pos[1] += glyph->advance_y;

				mIndexCount += 6;
			}
		}

		F32 halfwidth = mFTAtlas->width * 0.5f;
		F32 halfheight = mFTAtlas->height * 0.5f;
	}

	void Renderer::render()
	{
		for (I32 i = 0; i < mTextureSlots.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mTextureSlots[i]);
		}

		glUseProgram(mProgram);

		glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, NULL);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		mTextureSlots.clear();
		mIndexCount = 0;
		glBindVertexArray(0);
	}


	void Renderer::end()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void Renderer::tStackPush(const mat4& mat)
	{
		mTransformationStack.push(mTransformationStack.top() * mat);
	}

	void Renderer::tStackPop()
	{
		mTransformationStack.pop();
	}

	bool Renderer::load(std::string path, AssetManager& manager)
	{
		return false;
	}

	bool Renderer::save(std::string path, AssetManager& manager)
	{
		return false;
	}

	void Renderer::destroy()
	{

	}

	Renderer::~Renderer()
	{
		glDeleteBuffers(1, &mVBO);
		glDeleteBuffers(1, &mIBO);
		glDeleteVertexArrays(1, &mVAO);
		glDeleteProgram(mProgram);
	}
}
