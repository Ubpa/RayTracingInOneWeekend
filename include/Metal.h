#ifndef _METAL_H_
#define _METAL_H_

#include <Material.h>

#include <Util.h>

class Metal : public Material {
public:
	Metal(const Vec3f & albedo, float fuzz) : albedo(albedo), fuzz(fuzz) { }

public:
	static Ptr<Metal> New(const Vec3f & albedo, float fuzz = 0.f) {
		return std::make_shared<Metal>(albedo, fuzz);
	}

public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const override;

public:
	Vec3f albedo;
	float fuzz; // 0 -- 1
};

// ------------- й╣ож

const ScatterRst Metal::Scatter(const Ray & ray, const HitRecord & rec) const {
	Vec3f dir = Util::Reflect(ray.d.Normalize(), rec.n) + fuzz * Util::RandInSphere();
	if (dir.Dot(rec.n) <= 0)
		return ScatterRst::InValid();

	Ray newRay(rec.p, dir);
	return ScatterRst(true, newRay, albedo);
}

#endif // !_METAL_H_
