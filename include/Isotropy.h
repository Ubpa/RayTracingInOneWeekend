#ifndef _ISOTROPY_H_
#define _ISOTROPY_H_

#include <Material.h>

class Isotropy : public Material {
public:
	Isotropy(const Vec3f & albedo)
		: albedo(albedo) { }

public:
	static const Ptr<Isotropy> New(const Vec3f & albedo) {
		return std::make_shared<Isotropy>(albedo);
	}

public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const override {
		return { true, {rec.p, Util::RandOnSphere()}, albedo };
	}

public:
	Vec3f albedo;
};

#endif // !_ISOTROPY_H_
