#include <fstream>

#include <ROOT_PATH.h>

using namespace std;

int main(){
	int width = 200;
	int height = 100;

	ofstream rst(ROOT_PATH + "data/01.ppm"); // ppm 是一种简单的图片格式

	rst << "P3\n" << width << " " << height << "\n255\n";

	for (int j = 0; j < height; j++) { // 从上至下
		for(int i = 0; i < width; i++) { // 从左至右
			float r = float(i) / float(width);
			float g = float(height - j) / float(height);
			float b = 0.2f;

			// 浮点数颜色 [0, 1] 转化成整数颜色 [0, 255]
			int ir = int(255.99f * r);
			int ig = int(255.99f * g);
			int ib = int(255.99f * b);

			rst << ir << " " << ig << " " << ib << endl;
		}
	}

	rst.close();

	return 0;
}
