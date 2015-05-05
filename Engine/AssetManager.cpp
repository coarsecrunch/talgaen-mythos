#include "AssetManager.h"
#include "GL\glew.h"
#include "Rendering.h"
#include "Camera.h"
#include "Texture.h"
#include "Rect.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Cmn.h"
#include "Animation.h"
#include "AnimSet.h"
#include "Math/Operations.h"

#define RELATIVE_ASSETS_PATH std::string("../assets/")

namespace talga
{

	static struct Vertex
	{
		vec3 position;
		vec3 coord;
	};

	Vertex* verts;
	U16* indis;

	/* Amounts for the max amounts for stored assets. If vector are constantly dynamically allocated
	then the addresses for the elements change and the pointers to them become invalid */
	const I32 MAX_ANIMATIONS = 100;
	const I32 MAX_TEXTURES = 1000;
	const I32 MAX_MAPS = 50;


	AssetManager::AssetManager()
		: mBuffers{}
		, mAnimations{}
		, mTextures{}
		, mAnimationsToLoad{}
		, mMaps{}
		, mAnimationSets{}
		, NO_TEXTURE{ nullptr }
	{
		mAnimations.reserve(MAX_ANIMATIONS);
		mAnimationSets.reserve(MAX_ANIMATIONS);
		mTextures.reserve(MAX_TEXTURES);
		mMaps.reserve(MAX_MAPS);
		mAnimationsToLoad.reserve(MAX_ANIMATIONS);

		//AddTexture(RELATIVE_ASSETS_PATH + "notex.png");
		//NO_TEXTURE = GetTexture("notex.png");
	}

	void AssetManager::AddTexture(std::string path)
	{
		mTextures.push_back(Texture{path});
	}

	void AssetManager::AddAnimation(std::string texName, std::string name, std::vector<Rect> frames)
	{
		cpTex tex = nullptr;

		tex = GetTexture(texName);


		if (tex == nullptr) return;

		mAnimationsToLoad.push_back(toLoadAnim{ name, frames, tex });
	}

	void AssetManager::LoadMap(std::string path)
	{
		std::ifstream stream;

		stream.open(path);
		TALGA_ASSERT(stream.is_open(), std::string("failed to read map ") + path);

		char cc = '\0';
		I32 index = path.size();

		while (cc != '\\' && cc != '/')
		{
			--index;
			cc = path.at(index);
		}

		I32 startIndex = index + 1;
		I32 length = (path.size() - startIndex);
		std::string name = path.substr(startIndex, length);


		I32 tileWidth;
		I32 tileHeight;
		I32 mapWidth;
		I32 mapHeight;
		I32 numTextures;

		std::vector<Tile> tiles;
		std::string tempTex;

		stream >> tileWidth;
		stream >> tileHeight; // tileHeight
		stream >> mapWidth; // mapWidth
		stream >> mapHeight; // mapHeight
		stream >> numTextures; // numTiles

		for (auto i = 0; i < numTextures; ++i)
		{
			stream >> tempTex;
			cpTex tex = GetTexture(tempTex);
			
			if (!tex)
			{
				AddTexture( RELATIVE_ASSETS_PATH + tempTex);
				tex = GetTexture(tempTex);
			}
			
			TALGA_WARN(tex, std::string("map ") + name + " tried to load " + tempTex + " but could not");


			if (tex)
			{
				I32 framesPerRow = tex->w() / tileWidth;
				I32 framesPerColumn = tex->h() / tileHeight;

				for (auto y = 0; y < framesPerColumn; ++y)
				{
					for (auto x = 0; x < framesPerRow; ++x)
					{
						//TL TR BR BL
						/*tiles.push_back(Tile{ tex, UVFrame{ {
								GET_UV((F32)tileWidth * x, tex->w(), (F32)tileHeight * y, tex->h()),
								GET_UV( (F32)tileWidth * x + tileWidth, tex->w(), (F32)tileHeight * y, tex->h()),
								GET_UV((F32)tileWidth * x + tileWidth, tex->w(), (F32)tileHeight * y + tileHeight, tex->h()),
								GET_UV((F32)tileWidth * x, tex->w(), (F32)tileHeight * y + tileHeight, tex->h())
							} } });
						*/
						
						tiles.push_back(Tile{ tex, UVFrame{ {
								GET_UV((F32)tileWidth * x, tex->w(), (F32)tileHeight * y + tileHeight, tex->h()),
						
						GET_UV((F32)tileWidth * x + tileWidth, tex->w(), (F32)tileHeight * y + tileHeight, tex->h()),
						GET_UV((F32)tileWidth * x + tileWidth, tex->w(), (F32)tileHeight * y, tex->h()),
						GET_UV((F32)tileWidth * x, tex->w(), (F32)tileHeight * y, tex->h())
						} } });
						
					}
				}
			}
			else // push back default UVs for the NO_TEXTURE texture
			{
				tiles.push_back(Tile{ tex, UVFrame{{ vec2{0.0f, 1.0f}, vec2{1.0f, 1.0f}, vec2{1.0f, 0.0f}, vec2{0.0f, 0.0f} } }});
			}
		}

		std::vector<I32> mapdata(mapWidth * mapHeight);

		for (int y = 0; y < mapHeight; ++y)
		{
			for (int x = 0; x < mapWidth; ++x)
			{
				stream >> mapdata[y * mapWidth + x];
			}
		}

		Map map(name, tiles, mapdata, mapWidth, mapHeight, tileWidth, tileHeight);
		mMaps.push_back(map);
	}

	static vec3 UV(float x, float xMax, float y, float yMax, float frameH)
	{
		return vec3(x / xMax, (y / yMax));
	}

	/* NOTE -- Had problems with UV coordiantes, figured out the problem was that the image
	is flipped upsideown, top and bottom corners are switched with eachtother */
	void AssetManager::LoadAnimations()
	{
		int lNumVerts = 0;

		for (int i = 0; i < mAnimationsToLoad.size(); ++i)
		{
			for (int j = 0; j < mAnimationsToLoad[i].frames.size(); ++j)
			{
				lNumVerts += 4; // 4 vertices per frame
			}
		}

		Vertex* vertices = new Vertex[lNumVerts];
		U16* indices = new U16[U32(1.5f * lNumVerts)];
		int bytesUsed = 0;

		for (int i = 0, vert = 0, indi = 0; i < mAnimationsToLoad.size(); ++i)
		{
			Animation anim;
			anim.frames = mAnimationsToLoad[i].frames;
			anim.tex = mAnimationsToLoad[i].tex;
			anim.numFrames = mAnimationsToLoad[i].frames.size();
			anim.name = mAnimationsToLoad[i].name;

			anim.bufferLoc = bytesUsed;

			bytesUsed += sizeof(U16) * 6 * mAnimationsToLoad[i].frames.size();

			for (int j = 0; j < mAnimationsToLoad[i].frames.size(); ++j, vert += 4, indi += 6)
			{
				vec3 tl(-mAnimationsToLoad[i].frames[j].w * 0.5, mAnimationsToLoad[i].frames[j].h * 0.5);

				vec3 blUV = UV(mAnimationsToLoad[i].frames[j].x, mAnimationsToLoad[i].tex->w(), mAnimationsToLoad[i].frames[j].y, mAnimationsToLoad[i].tex->h(), mAnimationsToLoad[i].frames[j].h);

				vec3 tr(mAnimationsToLoad[i].frames[j].w * 0.5, mAnimationsToLoad[i].frames[j].h * 0.5);

				vec3 brUV = UV(mAnimationsToLoad[i].frames[j].x + mAnimationsToLoad[i].frames[j].w, mAnimationsToLoad[i].tex->w(), mAnimationsToLoad[i].frames[j].y, mAnimationsToLoad[i].tex->h(), mAnimationsToLoad[i].frames[j].h);

				vec3 bl(-mAnimationsToLoad[i].frames[j].w * 0.5, -mAnimationsToLoad[i].frames[j].h * 0.5);

				vec3 tlUV = UV(mAnimationsToLoad[i].frames[j].x, mAnimationsToLoad[i].tex->w(), mAnimationsToLoad[i].frames[j].y + mAnimationsToLoad[i].frames[j].h, mAnimationsToLoad[i].tex->h(), mAnimationsToLoad[i].frames[j].h);

				vec3 br(mAnimationsToLoad[i].frames[j].w * 0.5, -mAnimationsToLoad[i].frames[j].h * 0.5);

				vec3 trUV = UV(mAnimationsToLoad[i].frames[j].x + mAnimationsToLoad[i].frames[j].w, mAnimationsToLoad[i].tex->w(), mAnimationsToLoad[i].frames[j].y + mAnimationsToLoad[i].frames[j].h, mAnimationsToLoad[i].tex->h(), mAnimationsToLoad[i].frames[j].h);

				vertices[vert] = Vertex{ tl, tlUV };
				vertices[vert + 1] = Vertex{ tr, trUV };
				vertices[vert + 2] = Vertex{ br, brUV };
				vertices[vert + 3] = Vertex{ bl, blUV };

				U16 i1 = indices[indi] = vert;
				U16 i2 = indices[indi + 1] = vert + 1;
				U16 i3 = indices[indi + 2] = vert + 3;
				U16 i4 = indices[indi + 3] = vert + 1;
				U16 i5 = indices[indi + 4] = vert + 2;
				U16 i6 = indices[indi + 5] = vert + 3;
			}

			mAnimations.push_back(anim);
		}

		/*glBindVertexArray(Camera::mVAOs[Camera::TEXTURED_QUAD]);

		glGenBuffers(numBuffers, mBuffers);
		glBindBuffer(GL_ARRAY_BUFFER, mBuffers[ARRAY_BUFFER_TEXTURED_QUAD]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * lNumVerts, vertices, GL_STATIC_DRAW);

		unsigned int offsetVerts = (unsigned int)&vertices[0].position[0] - (unsigned int)vertices;
		unsigned int offsetCoords = (unsigned int)&vertices[0].coord[0] - (unsigned int)vertices;

		glVertexAttribPointer(TEXTURED_QUAD_vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetVerts);
		glVertexAttribPointer(TEXTURED_QUAD_vUvCoord, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetCoords);
		glEnableVertexAttribArray(TEXTURED_QUAD_vPosition);
		glEnableVertexAttribArray(TEXTURED_QUAD_vUvCoord);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[ELEMENT_BUFFER_TEXTURED_QUAD]);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * unsigned int(1.5f * lNumVerts), indices, GL_STATIC_DRAW);


		glBindVertexArray(0);


		static I32 time = 0;

		if (time++ == 0) return;
		char* currentPos; // point at beginning of vertex and cast as Vertex*

		U16 index; // * sizeof(Vertex) should give you the byte offset

		index = GetAnimation("talgaRunR")->getOffset(0);

		currentPos = (char*)indices + index;

		U16 num = *(U16*)currentPos;

		Vertex* posCast = &vertices[num];
		*/
		delete[] indices;
		delete[] vertices;
	}

	cpMap AssetManager::GetMap(std::string name) const
	{
		for (int i = 0; i < mMaps.size(); ++i)
		{
			if (mMaps[i].getName() == name)
				return &mMaps[i];
		}

		TALGA_WARN(0, std::string("failed to find map ") + name);
		return nullptr;
	}

	cpAnim AssetManager::GetAnimation(std::string name) const
	{
		for (int i = 0; i < mAnimations.size(); ++i)
		{
			if (mAnimations[i].name == name)
				return &mAnimations[i];
		}

		TALGA_WARN(0, std::string("failed to find map ") + name);
		return nullptr;
	}
	cpTex AssetManager::GetTexture(std::string name) const
	{
		for (int i = 0; i < mTextures.size(); ++i)
		{
			if (mTextures[i].name == name)
			{
				return &mTextures[i];
			}
		}

		TALGA_WARN(0, std::string("failed to find texture ") + name);
		return nullptr;
	}
	AssetManager::~AssetManager()
	{
		for (Texture& tex : mTextures)
		{
			tex.destroy();
		}
		delete[] indis;
		delete[] verts;
	}
}