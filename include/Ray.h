#ifndef _RAY_H_
#define _RAY_H_

#include <Vec3.h>
#include <Util.h>

class Ray {
public:
	// tMin = 0.001f，以避免重复自交的情况
	Ray(const Vec3f & origin, const Vec3f & dir, float tMin = Util::DEFAULT_TMIN, float tMax = Util::LARGE_FLT)
		{ Init(origin, dir, tMin, tMax); }

public:
	void Init(const Vec3f & origin, const Vec3f & dir, float tMin = Util::DEFAULT_TMIN, float tMax = Util::LARGE_FLT);

	const Vec3f At(float t) const { return o + t * d; }
	const Vec3f StartPos() const { return this->At(tMin); }
	const Vec3f EndPos() const { return this->At(tMax); }
	const Vec3f InvDir() const { return { 1.f / d.x,1.f / d.y,1.f / d.z }; }

public:
	Vec3f o; // 起点
	Vec3f d; // 方向

	 // 将 tMin 和 tMax 直接放在 ray 中，简化设计
	float tMin; // 决定射线起点
	float tMax; // 决定射线终点
};

void Ray::Init(const Vec3f & origin, const Vec3f & dir, float tMin, float tMax) {
	o = origin;
	d = dir;
	this->tMin = tMin;
	this->tMax = tMax;
}

#endif // !_RAY_H_
