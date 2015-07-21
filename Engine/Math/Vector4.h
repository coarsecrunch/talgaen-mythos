#pragma once
namespace talga
{
	class Vector4 {
	public:
		Vector4(float f = 0.0f, float f1 = 0.0f, float f2 = 0.0f, float f3 = 1.0f);
		virtual ~Vector4();

		Vector4& operator=(const Vector4& copy);

		const float& operator[](int index) const;
		float& operator[](int index);
		float operator()(int index) const;
		float x() const { return vec[0]; }
		float y() const { return vec[1]; }
		float z() const { return vec[2]; }
		float w() const { return vec[3]; }

		float r() const { return vec[0]; }
		float g() const { return vec[1]; }
		float b() const { return vec[2]; }
		float a() const { return vec[3]; }


		float mag() const;
		float dx() const;
		float dy() const;
		float dz() const;
	private:
		float vec[4];
	};

	Vector4 normalize(const Vector4& vec);
	Vector4 cross(const Vector4& vec1, const Vector4& vec2);
	Vector4 project(const Vector4& vec, const Vector4& onto);
	float dot(const Vector4& vec1, const Vector4& vec2);
	Vector4 operator-(const Vector4& vec1, const Vector4& vec2);
	Vector4 operator+(const Vector4& vec1, const Vector4& vec2);
	Vector4 vecBetween(Vector4 point1, Vector4 point2);
	Vector4 operator*(const Vector4& vec, float scalar);

	typedef Vector4 vec4;
}