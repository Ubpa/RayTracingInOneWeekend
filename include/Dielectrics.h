#ifndef _DIELECTRICS_H_
#define _DIELECTRICS_H_

#include <Material.h>

#include <Util.h>

class Dielectrics :public Material {
public:
	Dielectrics(float ior) :ior(ior) {}

public:
	static Ptr<Dielectrics> New(float ior) {
		return std::make_shared<Dielectrics>(ior);
	}

public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const override;

public:
	float ior;
};

// ------------------ 实现

const ScatterRst Dielectrics::Scatter(const Ray & ray, const HitRecord & rec) const {
	Vec3f I = ray.d.Normalize();

	Vec3f T;
	if (!Util::Refract(I, rec.n, ior, T)) {
		// 全反射
		Vec3f dir = Util::Reflect(I, rec.n);
		return ScatterRst(true, { rec.p, dir }, Vec3f(1.f));
	}

	float IoN = I.Dot(rec.n);
	bool isEntering = IoN < 0;
	// 必须是空气中的方向与法向的夹角的余弦
	float cosTheta = isEntering ? -I.Dot(rec.n) : T.Dot(rec.n);
	float F = Util::Fresnel_Schlick(ior, cosTheta); // 菲涅尔系数，即反射概率

	if (Util::RandF() < F) {
		// 反射
		Vec3f dir = Util::Reflect(I, rec.n);
		return ScatterRst(true, { rec.p, dir }, Vec3f(1.f));
	}

	// 折射
	return ScatterRst(true, { rec.p, T }, Vec3f(1.f));
}

#endif // !_DIELECTRICS_H_
