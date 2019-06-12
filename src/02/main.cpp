#include <Vec3.h>

#include <ROOT_PATH.h>

#include <fstream>

using namespace std;

int main() {
	int width = 200;
	int height = 100;

	ofstream rst(ROOT_PATH + "data/02.ppm"); // ppm 是一种简单的图片格式

	rst << "P3\n" << width << " " << height << "\n255\n";

	for (int j = 0; j < height; j++) { // 从上至下
		for (int i = 0; i < width; i++) { // 从左至右
			float r = float(i) / float(width);
			float g = float(height - j) / float(height);
			float b = 0.2f;

			Vec3f color(r, g, b);

			Vec3i icolor = 255.99f * color;
			rst << icolor.r << " " << icolor.g << " " << icolor.b << endl;
		}
	}

	rst.close();

	return 0;
}
