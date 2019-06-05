#include <Sphere.h>
#include <HitableList.h>

#include <Util.h>

#include <ROOT_PATH.h>

#include <fstream>

using namespace std;

const Vec3f Sky(const Ray & ray);

const Vec3f Trace(Ptr<Hitable> scene, Ray & ray) {
	HitRecord rec;
	if (scene->Hit(ray, rec))
		return 0.5f * (rec.n + Vec3f(1.f)); // 映射到 0 - 1

	return Sky(ray);
}

int main() {
	int width = 200;
	int height = 100;
	int sampleNum = 100;

	// 相机参数
	Vec3f pos(0.f);
	Vec3f lowerLeft(-2, -1, -1);
	Vec3f horizontal(4, 0, 0);
	Vec3f vertical(0, 2, 0);

	// 场景
	auto sphere = Sphere::New({ 0, 0, -1 }, 0.5f);
	auto ground = Sphere::New({ 0, -100.5, -1 }, 100.f);
	auto scene = HitableList::New({ sphere,ground });

	ofstream rst(ROOT_PATH + "data/06.ppm"); // ppm 是一种简单的图片格式

	rst << "P3\n" << width << " " << height << "\n255\n";

	for (int j = 0; j < height; j++) { // 从上至下
		for (int i = 0; i < width; i++) { // 从左至右
			Vec3f color(0.f);
			for (int k = 0; k < sampleNum; k++) { // 多重采样
				float u = (i + Util::RandF()) / width;
				float v = (height - j + Util::RandF()) / height;

				Vec3f dir = lowerLeft + u * horizontal + v * vertical - pos;
				Ray ray(pos, dir);

				color += Trace(scene, ray);
			}
			color /= float(sampleNum); // 求均值

			Vec3i iColor = 255.99f * color;
			rst << iColor.r << " " << iColor.g << " " << iColor.b << endl;
		}
	}

	rst.close();

	return 0;
}

const Vec3f Sky(const Ray & ray) {
	auto normDir = ray.d.Normalize();
	float t = 0.5f * (normDir.y + 1.0f);

	const Vec3f white(1.f);
	const Vec3f blue(0.5, 0.7, 1);

	return Vec3f::Lerp(white, blue, t); // 线性插值
}
