#ifndef _BOX_H_
#define _BOX_H_

#include <Vec3.h>
#include <Ray.h>

class Box {
public:
	Box(const Vec3f & minP = Vec3f(Util::LARGE_FLT), const Vec3f & maxP = Vec3f(-Util::LARGE_FLT))
		: minP(minP), maxP(maxP) { }

public:
	struct HitRst {
		bool hit;
		float t0;
		float t1;
	};
	const HitRst Hit(const Ray & ray) const;

	// 合并两个盒子
	static const Box Union(const Box & lhs, const Box & rhs);
	const Box UnionWith(const Box & rhs) const {
		return Union(*this, rhs);
	}

	const Vec3f Center() const { return (minP + maxP) / 2.f; }

public:
	Vec3f minP;
	Vec3f maxP;
};

const Box::HitRst Box::Hit(const Ray & ray) const {
	float tMin = ray.tMin;
	float tMax = ray.tMax;
	auto invDir = ray.InvDir();

	HitRst rst;

	for (int axis = 0; axis < 3; axis++) {
		float invD = invDir[axis];
		float t0 = (minP[axis] - ray.o[axis]) * invD;
		float t1 = (maxP[axis] - ray.o[axis]) * invD;
		if (invD < 0.0f)
			std::swap(t0, t1);

		// 范围求交
		tMin = (std::max)(t0, tMin);
		tMax = (std::min)(t1, tMax);
		if (tMax < tMin) {
			rst.hit = false;
			return rst;
		}
	}

	rst.hit = true;
	rst.t0 = tMin;
	rst.t1 = tMax;
	return rst;
}

const Box Box::Union(const Box & lhs, const Box & rhs) {
	auto minP = Vec3f::Min(lhs.minP, rhs.minP);
	auto maxP = Vec3f::Max(lhs.maxP, rhs.maxP);
	return { minP, maxP };
}

#endif // !_BOX_H_
