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

int main() {
	int width = 200;
	int height = 100;

	// 相机参数
	Vec3f pos(0.f);
	Vec3f lowerLeft(-2, -1, -1);
	Vec3f horizontal(4, 0, 0);
	Vec3f vertical(0, 2, 0);

	ofstream rst(ROOT_PATH + "data/03.ppm"); // ppm 是一种简单的图片格式

	rst << "P3\n" << width << " " << height << "\n255\n";

	for (int j = 0; j < height; j++) { // 从上至下
		for (int i = 0; i < width; i++) { // 从左至右
			float u = float(i) / float(width);
			float v = float(height - j) / float(height);

			Vec3f dir = lowerLeft + u * horizontal + v * vertical - pos;
			Ray ray(pos, dir);

			auto skyColor = Sky(ray);

			Vec3i iSkyColor = 255.99f * skyColor;
			rst << iSkyColor.r << " " << iSkyColor.g << " " << iSkyColor.b << endl;
		}
	}

	rst.close();

	return 0;
}
