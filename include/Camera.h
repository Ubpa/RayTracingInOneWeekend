#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <Vec3.h>
#include <Ray.h>
#include <Util.h>

class Camera {
public:
	// vfov: verticle field of view，纵向的角度
	// aspect: width / height，宽高比
	// aperture: 透镜直径
	// focusDis: 对焦距离，就是最清晰处离相机的距离
	Camera(const Vec3f & pos, const Vec3f & target, float vfov, float aspect,
		// 第 11 节引入
		float aperture = 0.f, float focusDis = 1.f)
	{ Init(pos, target, vfov, aspect, aperture, focusDis); }

public:
	void Init(const Vec3f & pos, const Vec3f & target, float vfov, float aspect,
		// 第 11 节引入
		float aperture = 0.f, float focusDis = 1.f);

public:
	const Ray GenRay(float u, float v) const;

private:
	Vec3f pos; // 相机位置
	Vec3f lowerLeft; // 成像平面的左下角

	Vec3f right; // 单位向量
	Vec3f horizontal; // 长度为成像平面宽度

	Vec3f up; // 单位向量
	Vec3f vertical; // 长度为成像平面高度

	// 第 11 节引入
	float lensRadius; // 透镜半径
};

// -------------- 实现
void Camera::Init(const Vec3f & pos, const Vec3f & target, float vfov, float aspect,
	// 第 11 节引入
	float aperture, float focusDis)
{
	this->pos = pos;

	float theta = vfov / 180.f * Util::PI; // 角度转弧度
	float halfHeight = focusDis * tan(theta / 2.f); // 成像平面高度的一半
	float halfWidth = aspect * halfHeight; // 成像平面宽度的一半

	lensRadius = aperture / 2.f;
	Vec3f front = (target - pos).Normalize();
	const Vec3f worldUp(0, 1, 0);
	right = front.Cross(worldUp);
	up = right.Cross(front);

	lowerLeft = pos + focusDis * front - halfWidth * right - halfHeight * up;
	horizontal = 2.f * halfWidth * right;
	vertical = 2.f * halfHeight * up;
}

const Ray Camera::GenRay(float u, float v) const {
	// 当 lensRadius == 0 时退化为
	// {pos, lowerLeft + u * horizontal + v * vertical - pos}

	Vec3f target = lowerLeft + u * horizontal + v * vertical;

	Vec3f rd = lensRadius * Util::RandInDisk();
	Vec3f offset = right * rd.x + up * rd.y;
	Vec3f origin = pos + offset;

	return { origin,  target - origin };
}

#endif // !_CAMERA_H_
