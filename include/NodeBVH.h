#ifndef _NODE_BVH_H_
#define _NODE_BVH_H_

#include <Box.h>
#include <Hitable.h>
#include <vector>

class NodeBVH : public Hitable{
public:
	NodeBVH(Ptr<Hitable> left, Ptr<Hitable> right)
		: left(left), right(right), box(Box::Union(left->GetBox(), right->GetBox())) { }
	NodeBVH(Ptr<Hitable> hitable) : NodeBVH(hitable, hitable) { }

public:
	static const Ptr<NodeBVH> New(Ptr<Hitable> left, Ptr<Hitable> right) {
		return std::make_shared<NodeBVH>(left, right);
	}
	static const Ptr<NodeBVH> New(Ptr<Hitable> hitable) {
		return std::make_shared<NodeBVH>(hitable);
	}

	static const Ptr<NodeBVH> Build(const std::vector<Ptr<Hitable>> & hitables);

public:
	virtual bool Hit(Ray & ray, HitRecord & rec) const override;

	virtual const Box GetBox() const override { return box; }

private:
	bool IsLeaf() const { return left == right; }

public:
	Ptr<Hitable> left;
	Ptr<Hitable> right;
	Box box;
};

bool NodeBVH::Hit(Ray & ray, HitRecord & rec) const {
	if (IsLeaf())
		return left->Hit(ray, rec);

	auto leftRst = left->GetBox().Hit(ray);
	auto rightRst = right->GetBox().Hit(ray);

	if (leftRst.hit) { // 击中左侧盒子
		if (rightRst.hit) { // 击中右侧盒子
			if (leftRst.t0 < rightRst.t0) { // 左节点靠前
				bool hit = left->Hit(ray, rec);
				if (!hit) // 左节点未击中，则相交只取决于右节点
					return right->Hit(ray, rec);

				// 已击中左结点，如果射线终点能够到右侧盒子，则继续对右节点进行判定
				if (ray.tMax > rightRst.t0)
					right->Hit(ray, rec);

				// 前边已确定了有节点被击中了
				return true;
			}
			else {
				// 与上边的情况左右相反
				bool hit = right->Hit(ray, rec);
				if (!hit)
					return left->Hit(ray, rec);

				if (ray.tMax > leftRst.t0)
					left->Hit(ray, rec);

				return true;
			}
		}
		else // 未击中右侧盒子，则相交只取决于左节点
			return left->Hit(ray, rec);
	}
	else if (rightRst.hit) // 未击中右侧盒子，则相交只取决于右节点
		return right->Hit(ray, rec);
	else
		return false;
}

const Ptr<NodeBVH> NodeBVH::Build(const std::vector<Ptr<Hitable>> & hitables) {
	if (hitables.size() == 1) // 递归终止条件
		return NodeBVH::New(hitables.front());

	// 获取包围盒中心，按轴记录
	std::vector<float> centers[3];
	for (auto hitable : hitables){
		auto center = hitable->GetBox().Center();
		for (int axis = 0; axis < 3; axis++)
			centers[axis].push_back(center[axis]);
	}
	
	// 计算中心的方差
	float vars[3];
	for (int axis = 0; axis < 3; axis++)
		vars[axis] = Util::Var(centers[axis]);

	// 方差最大的轴即为划分轴，划分值为均值
	int spiltAxis = (vars[0] > vars[1] && vars[0] > vars[2]) ? 0 : (vars[1] > vars[2] ? 1 : 2);
	float spiltVal = Util::Mean(centers[spiltAxis]);

	// 根据划分轴和划分值进行划分
	std::vector<Ptr<Hitable>> leftHitables;
	std::vector<Ptr<Hitable>> rightHitables;
	for (auto hitable : hitables) {
		auto center = hitable->GetBox().Center();
		if (center[spiltAxis] < spiltVal)
			leftHitables.push_back(hitable);
		else
			rightHitables.push_back(hitable);
	}
	
	// 有时会划分失败（中心重合），此时就强行均分
	if (leftHitables.size() == 0 || rightHitables.size() == 0) {
		leftHitables.clear();
		rightHitables.clear();

		auto halfNum = hitables.size() / 2;
		for (size_t i = 0; i < halfNum; i++)
			leftHitables.push_back(hitables[i]);
		for (size_t i = halfNum; i < hitables.size(); i++)
			rightHitables.push_back(hitables[i]);
	}
	
	// 递归
	auto leftNode = NodeBVH::Build(leftHitables);
	auto rightNode = NodeBVH::Build(rightHitables);
	return NodeBVH::New(leftNode, rightNode);
}

#endif // !_NODE_BVH_H_
