#ifndef _RAY_H_
#define _RAY_H_

#include <Vec3.h>

constexpr float LARGE_FLT = 98e10f;

class Ray {
public:
	Ray(const Vec3f & origin, const Vec3f & dir, float tMin = 0.001f, float tMax = LARGE_FLT)
		: o(origin), d(dir), tMin(tMin), tMax(tMax) { }

public:
	void Init(const Vec3f & origin, const Vec3f & dir, float tMin = 0.001f, float tMax = LARGE_FLT);

	const Vec3f At(float t) const { return o + t * d; }
	const Vec3f StartPos() const { return this->At(tMin); }
	const Vec3f EndPos() const { return this->At(tMax); }

public:
	Vec3f o;
	Vec3f d;
	float tMin; // 将 tMin 和 tMax 直接放在 ray 中，简化设计
	float tMax;
};

// ------------------ 实现
void Ray::Init(const Vec3f & origin, const Vec3f & dir, float tMin, float tMax) {
	o = origin;
	d = dir;
	this->tMin = tMin;
	this->tMax = tMax;
}

#endif // !_RAY_H_
