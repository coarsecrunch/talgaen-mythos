#include "Vector3.h"
#include <cmath>
#include "Vector2.h"
namespace talga
{
	vec3 normalize(const vec3& vec)
	{
		vec3 newVec = vec;
		float mag = vec.mag();
		if (mag == 0.0f)
			mag = 0.00000001f;

		newVec[0] = vec(0) / mag;
		newVec[1] = vec(1) / mag;
		newVec[2] = 0;

		return newVec;
	}

	vec3 cross(const vec3& vec1, const vec3& vec2)
	{
		vec3 newVec;
		newVec[0] = vec1(1) * vec2(2) - vec1(0) * vec2(1);
		newVec[1] = vec1(2) * vec2(0) - vec1(0) * vec2(2);
		newVec[2] = 0;
		return newVec;
	}

	vec3 operator-(const vec3& vec1, const vec3& vec2)
	{
		vec3 newVec;
		newVec[0] = vec1(0) - vec2(0);
		newVec[1] = vec1(1) - vec2(1);
		newVec[2] = 0;
		return newVec;
	}

	vec3 vecBetween(vec3 point1, vec3 point2)
	{
		float x = point2(0) - point1(0);
		float y = point2(1) - point1(1);

		return vec3(x, y, 0.0f);
	}

	float dot(const vec3& vec1, const vec3& vec2)
	{
		return (vec1(0)*vec2(0) + vec1(1) * vec2(1));
	}

	vec3 project(const vec3& vec, const vec3& onto)
	{
		vec3 projection;

		float dotProduct = dot(vec, onto);

		projection[0] = dotProduct * onto.dx();
		projection[1] = dotProduct * onto.dy();
		projection[2] = 0.0f;

		return projection;
	}
}