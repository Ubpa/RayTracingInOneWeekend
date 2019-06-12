#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <Material.h>

// นโิด
class Light : public Material {
public:
	Light(const Vec3f & L) : L(L) { }

public:
	static const Ptr<Light> New(const Vec3f & L) {
		return std::make_shared<Light>(L);
	}

public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const override { return ScatterRst::InValid(); }
	virtual const Vec3f Emit() const override { return L; }

public:
	Vec3f L;
};

#endif // !_LIGHT_H_
