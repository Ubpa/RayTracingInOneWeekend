#include <fstream>

#include <ROOT_PATH.h>

using namespace std;

int main(){
	int nx = 200;
	int ny = 100;

	ofstream rst(ROOT_PATH + "data/1.ppm");
	rst << "P3\n" << nx << " " << ny << "\n255\n";

	for (int j = 0; j < ny; j++) {
		for(int i=0; i <nx; i++) {
			float r = float(i) / float(nx);
			float g = float(ny - j) / float(ny);
			float b = 0.2f;

			int ir = int(255.99f * r);
			int ig = int(255.99f * g);
			int ib = int(255.99f * b);

			rst << ir << " " << ig << " " << ib << endl;
		}
	}

	rst.close();

	return 0;
}
