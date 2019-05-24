#include <QImage>

typedef qint8 Gray;
bool histeq(QImage &img)
{
	int *rs = new int[256];
	Gray *T = new Gray[256];
	// 初始化
	for (int i = 0; i < 256; i++) {
		rs[i] = 0;
		T[i] = 0;
	}
	int width = img.width();
	int height = img.height();
	// 统计图像中的灰度值
	int x = 0;
	int y = 0;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			rs[qGray(img.pixel(x, y))]++;
		}
	}

	// 计算灰度值映射表s = T(r)
	int imgSize = width * height;
	for (int i = 0; i < 256; i++) {
		double temp = 0;
		for (int j = 0; j <= i; j++) {
			temp += rs[j];
		}
		T[i] = round(255 * temp / imgSize);
	}

	// 实现映射r->s
	int s = 0;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			s = qGray(img.pixel(x, y));
			img.setPixel(x, y, qRgb(T[s], T[s], T[s]));
		}
	}
	// 释放内存
	delete rs;
	rs = nullptr;
	delete T;
	T = nullptr;
	return true;
}