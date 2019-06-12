#ifndef _HITABLE_LIST_H_
#define _HITABLE_LIST_H_

#include <Hitable.h>
#include <vector>

class HitableList : public Hitable, public std::vector<Ptr<Hitable>> {
public:
	HitableList(const std::vector<Ptr<Hitable>> & hitables = std::vector<Ptr<Hitable>>{})
		: std::vector<Ptr<Hitable>>(hitables) {}

public:
	// 生成 Ptr 的便利接口
	static Ptr<HitableList> New(const std::vector<Ptr<Hitable>> & hitables = std::vector<Ptr<Hitable>>{}) {
		return std::make_shared<HitableList>(hitables);
	}

public:
	virtual bool Hit(Ray & ray, HitRecord & rec) const override;

	virtual const Box GetBox() const override; // 第 14 节引入
};

// ------------- 实现

bool HitableList::Hit(Ray & ray, HitRecord & rec) const {
	bool isHit = false;
	for (auto hitable : *this) {
		if (hitable->Hit(ray, rec))
			isHit = true;
	}
	return isHit;
}

const Box HitableList::GetBox() const {
	Box box;
	for (auto hitable : *this)
		box = box.UnionWith(hitable->GetBox());
	return box;
}

#endif // !_HITABLE_LIST_H_
