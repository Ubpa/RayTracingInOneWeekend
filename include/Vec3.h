#ifndef _VEC3_H_
#define _VEC3_H_

#include <cassert>
#include <cmath>

template<typename T>
class Vec3;
using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;

template<typename T>
class Vec3 {
public:
	template<typename T1, typename T2, typename T3>
	Vec3(T1 x, T2 y, T3 z)
		: x(static_cast<T>(x)),
		y(static_cast<T>(y)),
		z(static_cast<T>(z))
	{ assert(!std::isnan<double>(x) && !std::isnan<double>(y) && !std::isnan<double>(z)); }

	explicit Vec3(T v) : Vec3(v, v, v) { }

	Vec3() :Vec3(static_cast<T>(0)) {}

	template<typename U>
	Vec3(const Vec3<U> & v) : Vec3(v.x, v.y, v.z) {}

public:
	// 元素获取
	T & operator[](int n) {
		assert(n >= 0 && n < 3);
		return _data[n];
	}
	const T & operator[](int n) const {
		assert(n >= 0 && n < 3);
		return _data[n];
	}

	// 插值
	template<typename U>
	static const Vec3 Lerp(const Vec3 & lhs, const Vec3 & rhs, U t) {
		float tF = static_cast<float>(t);
		assert(tF >= 0. && tF <= 1.);
		Vec3f lhsF = lhs;
		Vec3f rhsF = rhs;
		return (1.0f - tF) * lhsF + tF * rhsF;
	}
	template<typename U>
	const Vec3 LerpWith(const Vec3 & rhs, U t) const {
		return Lerp(*this, rhs, t);
	}

	// 线性运算: +, -, *, /
	const Vec3 operator+(const Vec3 & rhs) const {
		return { x + rhs.x,y + rhs.y,z + rhs.z };
	}
	Vec3 & operator+=(const Vec3 & rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	const Vec3 operator-() const {
		return { -x,-y,-z };
	}

	const Vec3 operator-(const Vec3 & rhs) const {
		return { x - rhs.x,y - rhs.y,z - rhs.z };
	}
	Vec3 & operator-=(const Vec3 & rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	const Vec3 operator*(T k) const {
		return { x*k,y*k,z*k };
	}
	Vec3 & operator*=(T k) {
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}
	friend const Vec3 operator*(T k, Vec3 v) {
		return v * k;
	}

	const Vec3 operator/(T k) const {
		assert(k != 0);
		float invK = 1 / k;
		return { x*invK,y*invK,z*invK };
	}
	Vec3 & operator/=(T k) {
		assert(k != 0);
		float invK = 1 / k;
		x *= invK;
		y *= invK;
		z *= invK;
		return *this;
	}

	// 内积
	T Dot(const Vec3 & rhs) const {
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	// 内积范数
	T Norm2() const {
		return this->Dot(*this);
	}
	T Norm() const {
		return std::sqrt(Norm2());
	}
	const Vec3 Normalize() const {
		T norm = Norm();
		assert(norm != static_cast<T>(0));
		return *this / norm;
	}
	
	// 逐元素相乘
	const Vec3 operator*(const Vec3 & rhs) const {
		return { x*rhs.x,y*rhs.y,z*rhs.z };
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

#endif // !_VEC3_H_
