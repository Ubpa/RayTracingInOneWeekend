#ifndef _UTIL_H_
#define _UTIL_H_

#include <random>

// 用一个命名空间来方便查找接口
namespace Util {
	float RandF() {
		static std::default_random_engine engine;
		static std::uniform_real_distribution<float> fMap(0.0f, 1.0f);
		return fMap(engine);
	}
}

#endif // !_UTIL_H_
