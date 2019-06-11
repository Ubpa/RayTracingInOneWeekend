#ifndef _UTIL_H_
#define _UTIL_H_

#include <Vec3.h>

#include <random>

// 用于获取 CPU 核心数
#ifdef WIN32
#include <windows.h>
#elif defined linux
#include <unistd.h>
#else
#error not support system
#endif


// 用一个命名空间来便利查找接口
namespace Util {
	constexpr float PI = 3.1415926f;
	constexpr float LARGE_FLT = 98e8f; // 大浮点数
	constexpr float DELTA_FLT = 98e-8f; // 小浮点数
	constexpr float DEFAULT_TMIN = 0.001f;

	// [0, 1)
	float RandF() {
		static std::default_random_engine engine;
		static std::uniform_real_distribution<float> fMap(0.0f, 1.f - DELTA_FLT);
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

	const Vec3f RandOnSphere() {
		Vec3f p;
		do {
			p = 2.0*Vec3f(RandF(), RandF(), RandF()) - Vec3f(1.f);
		} while (p.Norm2() >= 1.f);

		return p.Normalize();
	}

	// [0, 1)
	// z 为 0，是冗余的
	const Vec3f RandInDisk() {
		Vec3f p;
		do {
			p = 2.f * Vec3f(RandF(), RandF(), 0) - Vec3f(1, 1, 0);
		} while (p.Norm2() >= 1.f);

		return p;
	}

	const Vec3f Gamma(const Vec3f & color) {
		float x = std::pow(color.x, 1.f / 2.2f);
		float y = std::pow(color.y, 1.f / 2.2f);
		float z = std::pow(color.z, 1.f / 2.2f);
		return { x,y,z };
	}

	// I 朝向表面，N 朝向表面外侧
	// I 和 N 无需是单位向量
	// 反射方向的长度等于 I
	const Vec3f Reflect(const Vec3f & I, const Vec3f & N) {
		return I - 2.f * I.Dot(N) * N;
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
		float F0 = pow((ior - 1.f) / (ior + 1.f), 2);
		return F0 + (1.f - F0) * pow(1.f - cosTheta, 5);
	}

	// 获取 CPU 的逻辑核心数
	int NumCPU() {
		// 根据不同的系统选择对应的实现
#ifdef WIN32
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		return static_cast<int>(info.dwNumberOfProcessors);
#elif defined linux
		int cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
		return cpu_num;
#else
#error not support system
#endif
	}

	// 均值
	template<typename T>
	T Mean(const std::vector<T> & vals) {
		auto rst = static_cast<T>(0);
		for (const auto & val : vals)
			rst += val;
		return rst / static_cast<T>(vals.size());
	}

	// 方差
	template<typename T>
	T Var(const std::vector<T> & vals) {
		auto rst = static_cast<T>(0);

		auto mean = Mean(vals);
		for (const auto & val : vals) {
			auto diff = val - mean;
			rst += diff * diff;
		}

		return rst / static_cast<T>(vals.size());
	}
}

#endif // !_UTIL_H_
