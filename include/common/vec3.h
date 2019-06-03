#ifndef _COMMON_VEC3_H_
#define _COMMON_VEC3_H_

#include <cassert>
#include <cmath>

template<typename T>
class vec3 {
public:
	template<typename T1, typename T2, typename T3>
	vec3(T1 x, T2 y, T3 z)
		: x(static_cast<T>(x)),
		y(static_cast<T>(y)),
		z(static_cast<T>(z))
	{ assert(!std::isnan(x) && !std::isnan(y) && !std::isnan(z)); }

	explicit vec3(T v) : vec3(v, v, v) { }

	vec3() :vec3(static_cast<T>(0)) {}

	template<typename U>
	vec3(const vec3<U> & v) : vec3(v.x, v.y, v.z) {}

public:
	T & operator[](int n) {
		assert(n >= 0 && n < 3);
		return _data[n];
	}
	const T & operator[](int n) const {
		assert(n >= 0 && n < 3);
		return _data[n];
	}
	const vec3 operator+(const vec3 & rhs) const {
		return { x + rhs.x,y + rhs.y,z + rhs.z };
	}
	vec3 & operator+=(const vec3 & rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	const vec3 operator-(const vec3 & rhs) const {
		return { x - rhs.x,y - rhs.y,z - rhs.z };
	}
	vec3 & operator-=(const vec3 & rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	const vec3 operator*(T k) const {
		return { x*k,y*k,z*k };
	}
	vec3 & operator*=(T k) {
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}
	friend const vec3 operator*(T k, vec3 v) {
		return v * k;
	}

	const vec3 operator/(T k) const {
		assert(k != 0);
		float invK = 1.f / k;
		return { x*invK,y*invK,z*invK };
	}
	vec3 & operator/=(T k) {
		assert(k != 0);
		float invK = 1.f / k;
		return *this *= invK;
	}

public:
	union {
		struct {
			T x, y, z;
		};
		struct {
			T r, g, b;
		};
		struct { T _data[3]; };
	};
};

using vec3f = vec3<float>;
using vec3i = vec3<int>;

#endif // !_COMMON_VEC3_H_
