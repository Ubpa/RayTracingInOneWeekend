#ifndef _UTIL_H_
#define _UTIL_H_

#include <Vec3.h>

#include <random>

// 用一个命名空间来便利查找接口
namespace Util {
	const constexpr float PI = 3.1415926f;

	// [0, 1)
	float RandF() {
		static std::default_random_engine engine;
		static std::uniform_real_distribution<float> fMap(0.0f, 0.999999f);
		return fMap(engine);
	}

	// [0, 1)
	const Vec3f RandInSphere() {
		Vec3f p;
		do {
			p = 2.0*Vec3f(RandF(), RandF(), RandF()) - Vec3f(1.f);
		} while (p.Norm2() >= 1.f);

		return p;
	}

	// [0, 1)
	// z 为 0，是冗余的
	const Vec3f RandInDisk() {
		Vec3f p;
		do {
			p = 2.0*Vec3f(RandF(), RandF(), 0.f) - Vec3f(1.f, 1.f, 0);
		} while (p.Norm2() >= 1.f);

		return p;
	}

	const Vec3f Gamma(const Vec3f & color) {
		float x = std::pow(color.x, 1.0f / 2.2f);
		float y = std::pow(color.y, 1.0f / 2.2f);
		float z = std::pow(color.z, 1.0f / 2.2f);
		return { x,y,z };
	}

	// I 朝向表面，N 朝向表面外侧
	// I 和 N 无需是单位向量
	// 反射方向的长度等于 I
	const Vec3f Reflect(const Vec3f & I, const Vec3f & N) {
		float IoN = I.Dot(N);
		return I - 2.f * IoN * N;
	}

	// I 和 N 是单位向量
	// I 朝向表面，N 朝向表面外侧
	// 发生全反射时，返回 false，否则返回 true
	const bool Refract(const Vec3f & I, Vec3f N, float ior, Vec3f & T) {
		float etai_etat; // 入射侧折射率 除以 折射侧折射率
		float IoN = I.Dot(N);
		bool isEntering = IoN < 0;
		if (!isEntering) {
			// 射出

			// 让 N 在 I 这一侧
			N = -N;
			IoN = -IoN;

			etai_etat = ior; // etai = ior, etat = 1
		}
		else
			etai_etat = 1.f / ior; // etai = 1, etat = ior

		float discriminant = 1.f - etai_etat * etai_etat * (1.f - IoN * IoN);
		if (discriminant <= 0) // 全反射
			return false;
		
		T = etai_etat * (I - IoN * N) - sqrt(discriminant) * N;
		return true;
	}

	// 菲涅尔系数，Schlick 近似公式
	float Fresnel_Schlick(float ior, float cosTheta) {
		float F0 = pow((ior - 1) / (ior + 1), 2);
		return F0 + (1.f - F0)*pow(1 - cosTheta, 5);
	}
}

#endif // !_UTIL_H_
