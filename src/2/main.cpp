#include <common/vec3.h>

#include <ROOT_PATH.h>

#include <fstream>

using namespace std;

int main() {
	int nx = 200;
	int ny = 100;

	ofstream rst(ROOT_PATH + "data/2.ppm");
	rst << "P3\n" << nx << " " << ny << "\n255\n";

	for (int j = 0; j < ny; j++) {
		for (int i = 0; i < nx; i++) {
			float r = float(i) / float(nx);
			float g = float(ny - j) / float(ny);
			float b = 0.2f;
			vec3f color(r, g, b);
			vec3i icolor = 255.99f * color;

			rst << icolor.r << " " << icolor.g << " " << icolor.b << endl;
		}
	}

	rst.close();

	return 0;
}
