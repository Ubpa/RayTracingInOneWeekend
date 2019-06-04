#ifndef _UTIL_H_
#define _UTIL_H_

#include <Vec3.h>

#include <random>

// 用一个命名空间来方便查找接口
namespace Util {
	float RandF() {
		static std::default_random_engine engine;
		static std::uniform_real_distribution<float> fMap(0.0f, 1.0f);
		return fMap(engine);
	}

	const Vec3f RandInSphere() {
		Vec3f p;
		do {
			p = 2.0*Vec3f(RandF(), RandF(), RandF()) - Vec3f(1.f);
		} while (p.Norm2() >= 1.f);

		return p;
	}

	const Vec3f Gamma(const Vec3f & color) {
		float x = std::pow(color.x, 1.0f / 2.2f);
		float y = std::pow(color.y, 1.0f / 2.2f);
		float z = std::pow(color.z, 1.0f / 2.2f);
		return { x,y,z };
	}

	const Vec3f Reflect(const Vec3f & I, const Vec3f & N) {
		float IoN = I.Dot(N);
		return I - 2.f * IoN * N;
	}

	// I 和 N 是单位向量
	const bool Refract(const Vec3f & I, Vec3f N, float ior, Vec3f & T) {
		bool isEntering = I.Dot(N) < 0;

		float eiet;
		if (!isEntering) {
			N = -N;
			eiet = ior;
		}
		else
			eiet = 1.f / ior;

		float IoN = I.Dot(N);
		float discriminant = 1.f - eiet * eiet * (1.f - IoN * IoN);
		if (discriminant <= 0)
			return false;
		
		T = eiet * (I - IoN * N) - sqrt(discriminant)*N;
		return true;
	}

	float Fresnel_Schlick(float ior, float cosTheta) {
		float F0 = pow((ior - 1) / (ior + 1), 2);
		return F0 + (1.f - F0)*pow(1 - cosTheta, 5);
	}
}

#endif // !_UTIL_H_
