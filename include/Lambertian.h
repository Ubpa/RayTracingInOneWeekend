#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_

#include <Material.h>

#include <Util.h>

class Lambertian : public Material {
public:
	Lambertian(const Vec3f & albedo) : albedo(albedo) { }

public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const override;

public:
	static Ptr<Lambertian> New(const Vec3f & albedo) {
		return std::make_shared<Lambertian>(albedo);
	}

public:
	Vec3f albedo;
};

// ------------- й╣ож

const ScatterRst Lambertian::Scatter(const Ray & ray, const HitRecord & rec) const {
	Vec3f dir = rec.n + Util::RandInSphere();
	Ray scatterRay(rec.p, dir);
	return ScatterRst(true, scatterRay, albedo);
}

#endif // !_LAMBERTIAN_H_
