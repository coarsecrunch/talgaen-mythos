#pragma once

#include "../Cmn.h"
#include "Vector2.h"

namespace talga
{
  template<typename T>
  T abs(T num)
  {
    if (num >= 0)
    {
      return num;
    }

    return -num;
  }

	vec2 GET_UV(F32 x, F32 xMax, F32 y, F32 yMax);
}

