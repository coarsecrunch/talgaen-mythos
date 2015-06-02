#pragma once

#define MAX_STRBFR 50
#include "tassert.h"

#define TALGA_UBUNTU_BUILD

namespace talga
{
	class Texture;
	class Map;
	class AnimationSet;

#ifdef TALGA_WINDOWS_BUILD
	typedef signed __int8 I8;
	typedef signed __int16 I16;
	typedef signed __int32 I32;
	typedef signed __int64 I64;
	typedef unsigned __int8 U8;
	typedef unsigned __int16 U16;
	typedef unsigned __int32 U32;
	typedef unsigned __int64 U64;
	typedef char C8;
	typedef wchar_t C16;
	typedef float F32;
    typedef double F64;
#endif
#ifdef TALGA_UBUNTU_BUILD
    typedef signed char I8;
    typedef signed short I16;
    typedef signed int I32;
    typedef signed long long I64;
    typedef unsigned char U8;
    typedef unsigned short U16;
    typedef unsigned int U32;
    typedef unsigned long long U64;
    typedef char C8;
    typedef wchar_t C16;
    typedef float F32;
    typedef double F64;
#endif

	//strings
	typedef const C8* cStr;
	typedef C8 cStrBfr[MAX_STRBFR];

	//engine specific
	typedef const Texture* cpTex;
	typedef const AnimationSet* cpAnimSet;
	typedef const Map* cpMap;
	typedef const Map& crMap;

	const F32 GRAVITY = 0.25f;
	const F32 MAX_SPEED = 14.0f;
	const F32 FRICTION = 0.754f;
	const F32 DT_VEL_RATIO = 0.05f;
}
