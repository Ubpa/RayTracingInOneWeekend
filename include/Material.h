#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <Hitable.h>

struct ScatterRst {
	ScatterRst(bool isScatter, const Ray & ray, const Vec3f attenuation) :isScatter(isScatter), ray(ray), attenuation(attenuation) { }

	static const ScatterRst & InValid() {
		static ScatterRst invalid(false, Ray(Vec3f(0.f), Vec3f(0.f)), Vec3f(0.f));
		return invalid;
	}

	bool isScatter;
	Ray ray; // 新的光线
	Vec3f attenuation; // 衰减
};

class Material {
public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const = 0;

	virtual const Vec3f Emit() const { return 0; } // 第 16 节引入
};

#endif // !_MATERIAL_H_
