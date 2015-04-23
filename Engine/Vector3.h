#pragma once
namespace talga
{
	class Vector3 {
	public:
		Vector3(float f = 0.0f, float f1 = 0.0f, float f2 = 1.0f);
		virtual ~Vector3();

		Vector3& operator=(const Vector3& copy);
		float& operator[](int index);
		const float& operator[](int index) const { return vec[index]; }
		float operator()(int index) const;

		float mag() const;
		float dx() const;
		float dy() const;
	private:
		float vec[3];
	};

	Vector3 normalize(const Vector3& vec);
	Vector3 cross(const Vector3& vec1, const Vector3& vec2);
	Vector3 project(const Vector3& vec, const Vector3& onto);
	float dot(const Vector3& vec1, const Vector3& vec2);
	Vector3 operator-(const Vector3& vec1, const Vector3& vec2);
	Vector3 operator+(const Vector3& vec1, const Vector3& vec2);
	Vector3 vecBetween(Vector3 point1, Vector3 point2);
	Vector3 operator*(const Vector3& vec, float scalar);

	typedef Vector3 vec3;
}