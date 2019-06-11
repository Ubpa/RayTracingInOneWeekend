#include <Sphere.h>
#include <Volume.h>
#include <HitableList.h>

#include <Lambertian.h>
#include <Metal.h>
#include <Dielectric.h>
#include <Isotropy.h>

#include <Util.h>

#include <Camera.h>

#include <ROOT_PATH.h>

#include <fstream>
#include <thread>
#include <array>

using namespace std;

const Vec3f Sky(const Ray & ray);
const Vec3f Trace(Ptr<Hitable> balls, Ray & ray, int depth);
Ptr<Hitable> GenScene();
void SaveImg(const vector<vector<Vec3f>> & img);

int main() {
	int width = 200;
	int height = 100;
	int sampleNum = 100;
#ifdef NDEBUG // release
	int cpuNum = Util::NumCPU();
#else
	int cpuNum = 1;
#endif // NDEBUG
	printf("CPU : %d\n", cpuNum);

	

	// 相机参数
	Vec3f pos(0);
	Vec3f target(0, 0, -1);
	float focusDist = 1.0;
	float vfov = 90.f;
	float aspect = float(width) / float(height);
	float aperture = 0.01f;
	Camera camera(pos, target, vfov, aspect, aperture, focusDist);

	// 场景
	auto balls = GenScene();

	vector<vector<Vec3f>> img(height, vector<Vec3f>(width)); // 行主序

	vector<thread> workers;
	vector<int> pixelNums(cpuNum, 0); // 用于记录每个 worker 计算完成的 pixel 数
	for (int id = 0; id < cpuNum; id++) {
		workers.push_back(thread([=, &img, &pixelNums]() { // 使用 lambda 函数，灵活
			for (int idx = id; idx < width*height; idx += cpuNum) {
				int y = idx / width;
				int x = idx - y * width;
				
				Vec3f color(0);
				for (int k = 0; k < sampleNum; k++) {
					float u = (x + Util::RandF()) / width;
					float v = (height - y + Util::RandF()) / height;

					Ray ray = camera.GenRay(u, v);

					color += Trace(balls, ray, 0);
				}
				img[y][x] = color / (float)sampleNum; // 图像是行主序的，因此不是 img[x][y]

				pixelNums[id]++;

				int sum = 0;
				for (auto pixelNum : pixelNums)
					sum += pixelNum;
				float rate = sum / float(width*height);
				printf("\r%.2f%% ...", rate*100.f);
			}
		}));
	}
	for (auto & worker : workers) // 主线程等待所有子线程完成任务
		worker.join();

	printf("\n"
		"Evaluate done\n"
		"Save image...\n");

	SaveImg(img);

	return 0;
}

const Vec3f Sky(const Ray & ray) {
	auto normDir = ray.d.Normalize();
	float t = 0.5f * (normDir.y + 1.0f);

	const Vec3f white(1.f);
	const Vec3f blue(0.5, 0.7, 1);

	return Vec3f::Lerp(white, blue, t); // 线性插值
}

const Vec3f Trace(Ptr<Hitable> scene, Ray & ray, int depth) {
	HitRecord rec;
	if (scene->Hit(ray, rec)) {
		if (depth >= 50)
			return Vec3f(0.f);

		auto scatterRst = rec.material->Scatter(ray, rec);
		if (!scatterRst.isScatter)
			return Vec3f(0.f);

		return scatterRst.attenuation * Trace(scene, scatterRst.ray, depth + 1);
	}

	return Sky(ray);
}

Ptr<Hitable> GenScene() {
	auto sphereBoundary = Sphere::New({ 0, 0, -1 }, 0.5f, nullptr);
	auto sphere = Volume::New(sphereBoundary, 2.5f, Isotropy::New({ 0.8,0.6,0.2 }));
	auto ground = Sphere::New({ 0,-100,-1 }, 99.5f, Lambertian::New(0.5f));
	auto scene = HitableList::New({ sphere, ground });
	return scene;
}

void SaveImg(const vector<vector<Vec3f>> & img) {
	int width = img.front().size();
	int height = img.size();

	ofstream rst(ROOT_PATH + "data/15.ppm"); // ppm 是一种简单的图片格式

	rst << "P3\n" << width << " " << height << "\n255\n";

	for (int j = 0; j < height; j++) { // 从上至下
		for (int i = 0; i < width; i++) { // 从左至右

			Vec3f gammaColor = Util::Gamma(img[j][i]);

			Vec3i iGammaColor = 255.99f * gammaColor;
			rst << iGammaColor.r << " " << iGammaColor.g << " " << iGammaColor.b << endl;
		}
	}

	rst.close();
}
