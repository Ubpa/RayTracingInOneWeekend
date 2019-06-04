#include <Ray.h>
#include <Vec3.h>

#include <ROOT_PATH.h>

#include <fstream>

using namespace std;

const Vec3f Sky(const Ray & ray) {
	auto normDir = ray.d.Normalize();
	float t = 0.5f * (normDir.y + 1.0f);

	const Vec3f white(1.f);
	const Vec3f blue(0.5, 0.7, 1);
	
	return Vec3f::Lerp(white, blue, t);
}

bool Hit_Sphere(const Vec3f & center, float radius, const Ray & ray) {
	// c : center
	// r : radius
	// o : ray.o
	// d : ray.d
	// 
	// o + t * d == p
	// (p-c)^2 == r^2
	// (d * t + o - c)^2 == r^2
	// d*d * t^2 + 2*d*(o-c) * t + (o-c)^2 - r^2 = 0

	auto oc = ray.o - center;

	float a = ray.d.Dot(ray.d);
	float b = 2.f * ray.d.Dot(oc);
	float c = oc.Dot(oc) - radius * radius;

	float delta = b * b - 4.f * a * c; // 判别式
	if (delta <= 0.f)
		return false;

	return true;
}

const Vec3f Trace(const Ray & ray) {
	if (Hit_Sphere({ 0,0,-1 }, 0.5f, ray))
		return { 1,0,0 };

	return Sky(ray);
}

int main() {
	int width = 200;
	int height = 100;

	// 相机参数
	Vec3f pos(0.f);
	Vec3f lowerLeft(-2, -1, -1);
	Vec3f horizontal(4, 0, 0);
	Vec3f vertical(0, 2, 0);

	ofstream rst(ROOT_PATH + "data/04.ppm"); // ppm 是一种简单的图片格式

	rst << "P3\n" << width << " " << height << "\n255\n";

	for (int j = 0; j < height; j++) { // 从上至下
		for (int i = 0; i < width; i++) { // 从左至右
			float u = float(i) / float(width);
			float v = float(height - j) / float(height);

			Vec3f dir = lowerLeft + u * horizontal + v * vertical - pos;
			Ray ray(pos, dir);

			auto color = Trace(ray);

			Vec3i iColor = 255.99f * color;
			rst << iColor.r << " " << iColor.g << " " << iColor.b << endl;
		}
	}

	rst.close();

	return 0;
}
