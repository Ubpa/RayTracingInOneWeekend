#ifndef _HITABLE_H_
#define _HITABLE_H_

#include <Ray.h>

#include <memory>

template<typename T>
using Ptr = std::shared_ptr<T>;

struct HitRecord {
	// float t; 我们在 Ray 中记录 tMin 和 tMax
	Vec3f p;
	Vec3f n;
};

class Hitable {
public:
	// 因为 Ray 中含有 tMin 和 tMax，所以这里不需要输入相应参数
	// 如果相交，函数会修改 ray 的 tMax
	virtual bool Hit(Ray & ray, HitRecord & rec) const = 0;
};

#endif // !_HITABLE_H_
