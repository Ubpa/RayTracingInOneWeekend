#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_

#include <Material.h>

#include <Util.h>

// 漫反射，朗伯模型
class Lambertian : public Material {
public:
	Lambertian(const Vec3f & albedo) : albedo(albedo) { }

public:
	// 生成 Ptr 的便利接口
	static Ptr<Lambertian> New(const Vec3f & albedo) {
		return std::make_shared<Lambertian>(albedo);
	}

public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const override;

public:
	Vec3f albedo; // 反射率，即颜色
};

const ScatterRst Lambertian::Scatter(const Ray & ray, const HitRecord & rec) const {
	Vec3f dir = rec.n + Util::RandInSphere();
	Ray scatterRay(rec.p, dir.Normalize());
	return ScatterRst(true, scatterRay, albedo);
}

#endif // !_LAMBERTIAN_H_
