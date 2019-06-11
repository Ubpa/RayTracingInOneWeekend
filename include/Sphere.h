#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <Hitable.h>

class Sphere : public Hitable {
public:
	Sphere(const Vec3f & center, float radius, Ptr<Material> material)
		: center(center), radius(radius), material(material) { }

public:
	// 生成 Ptr 的便利接口
	static Ptr<Sphere> New(const Vec3f & center, float radius, Ptr<Material> material = nullptr) {
		return std::make_shared<Sphere>(center, radius, material);
	}

public:
	virtual bool Hit(Ray & ray, HitRecord & rec) const override;

public:
	Vec3f center;
	float radius;

	Ptr<Material> material;// 第 08 节引入
};

bool Sphere::Hit(Ray & ray, HitRecord & rec) const {
	// o + t * d == p
	// (p-c)^2 == r^2
	// (d * t + o - c)^2 == r^2
	// d*d * t^2 + 2*d*(o-c) * t + (o-c)^2 - r^2 = 0

	auto oc = ray.o - center;

	float a = ray.d.Dot(ray.d);
	float b = ray.d.Dot(oc); // 化简
	float c = oc.Dot(oc) - radius * radius;

	float discriminant = b * b - a * c; // 化简
	if (discriminant < 0.f)
		return false;

	float sqrtDiscriminant = sqrt(discriminant);

	float t0 = (-b - sqrtDiscriminant) / a; // 较小根
	if (t0 <= ray.tMin || t0 >= ray.tMax) {
		float t1 = (-b + sqrtDiscriminant) / a; // 较大根
		if (t1 <= ray.tMin || t1 >= ray.tMax)
			return false; // 根不再 tMin 和 tMax 间

		ray.tMax = t1; // 更新 tMax
	}
	else
		ray.tMax = t0; // 更新 tMax

	rec.p = ray.EndPos();
	rec.n = (rec.p - center )/ radius;

	rec.material = material; // 第 08 节引入

	return true;
}

#endif // !_SPHERE_H_
