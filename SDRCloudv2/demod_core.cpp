#include "demod_core.h"
#include "constant.h"
#include "math.h"
#include "aptformat.h"
#include <QDebug>

#include <spline3interp.h>  
using namespace splab;

// here you can build and add Header files your self if you need.


// demod_core: do what you want
// data_in: the array data is provided by RTL USB
// len_in: tell you the length of data input
// data_out: the data you want to display on screen or play in your soundspeaker
// len_out: the length of data_out.
// Note: Be friendly to your computer, please keep data_out  is not a empty one!
void demod_core(double data_in[], unsigned int len_in, double data_out[], unsigned int *len_out)
{
	int i = 0;
	for (i = 0; i < len_in/2; i++)
	{
		// example 1:
		data_out[i] = 127.0*cos(2 * 3.14*0.003*i);


		// example 2:
		/*data_out[i] = cos(2 * 3.14*0.003*i) + data_in[i];*/

	}
	*len_out = i;
	return;
}

// 近似包络解调算法,为了节省内存,data_in和data_out可以指向同一片内存并且data_in将会被覆盖
void demodAM(int Fs, double data_in[], unsigned int len_in, double data_out[], unsigned int & len_out)
{
	static double phi = 2 * pi*Fc / Fs;
	static double cosphi2 = 2*cos(phi);
	static double sinphi = sin(phi);

	
	static double xi_pre = data_in[0];
	static double xi2_pre = xi_pre*xi_pre;
	
	double xi = 0.0;
	double xi2 = 0.0;
	int i = 0;
	for (i = 1; i < len_in; i++) {
		xi = data_in[i];
		xi2 = xi * xi;
		data_out[i - 1] = sqrt(xi2 + xi2_pre - cosphi2 * xi*xi_pre) / sinphi;
		xi_pre = xi;
		xi2_pre = xi2;
	}

	len_out = i - 1;
	for (int i = 0; i < 10; i++) {
		qDebug() << data_out[i];
	}
}


void linspace(double *v, const double startNum, const double endNum, const unsigned int N)
{
	if (N <= 0) {
		return;
	}
	else if (N == 1) {
		v[0] = startNum;

	}
	else {
		double gap = (endNum - startNum) / (N - 1);
		for (int i = 0; i < N; i++) {
			v[i] = startNum + i * gap;
		}
	}
}
void s_interp1(double *x, const unsigned int len1, double *y, const unsigned int len2)
{
	if (len1 < len2) {
		return;
	}
	qDebug() << "interp==============";
	qDebug() << "len1 = " << len1 << "len2=" << len2;
	double gap = double(len1) / double(len2);
	unsigned int gapn = ceil(double(len1) / double(len2));
	//qDebug() << "=========================";
	//qDebug() << floor(3.6) << ";" << ceil(3.1) << ";" << round(3.2) << ";" << round(3.7);

	qDebug() << "gapn=" << gapn;
	unsigned int *index = (unsigned int *)new int[len2];
	
	for (int i = 0; i < len2; i++) {
		index[i] = round(i*gap);
	}

	qDebug() << "index[18]=" << index[18];
	int j = 0;
	double temp = 0;

	for (int i = 0; i < len2; i++) {
		temp = 0;
		for (j = index[i]; j < (index[i] + gapn); j++) {
			if (j < len1) {
				temp += x[j];
			}
			else {
				temp += x[len1-1];
			}
			
		}
		temp /= gapn;
		y[i] = temp;
		//y[i] = 0;
	}
}
// img不需要预先分配空间
void aptToImage(int Fs, double *data, unsigned int len_in, unsigned char *&img, unsigned int &imgSize, unsigned int &width)
{
	int samplesPerWord = round(double(Fs) / Fw);
	int samplesPerPulse = round(double(Fs) / Fpulse);
	unsigned int len2 = round(double(len_in) / Fs * Fw);
	double temp = 0;
	double tMax = double(len_in) / double(Fs);
	
	// 初始化图像尺寸
	width = lineLength;
	imgSize = ceil(double(len2) / double(width))*width;	// len*Fw/Fs
	img = (unsigned char*)new char[imgSize];


	// 线性内插
	double *imgData = new double[len2];
	s_interp1(data, len_in, imgData, len2);

	for (int i = 0; i < 10; i++) {
		qDebug() << "=" << imgData[i];
	}

	int i = 0;
	for (i = 0; i < len2; i++) {
		img[i] = (unsigned char)(imgData[i] * 255);
	}

	for (; i < imgSize; i++) {
		img[i] = 0;	// 对剩余部分进行填充
	}

	delete imgData;
	imgData = nullptr;
	//// 初始化内插用到的一些变量
	//double *tApt = new double[len_in];
	//double *tImg = new double[len2];
	//
	//linspace(tApt, 0, tMax, len_in);
	//linspace(tImg, 0, tMax, len2);

	//// 内插计算
	//double Ml = 0.2105;
	//double Mr = 0.2105;
	//Vector<double> x(len_in, tApt);
	//Vector<double> y(len_in, data);
	//Spline3Interp<double> poly(x, y, Ml, Mr);
	//poly.calcCoefs();

	//int i = 0;
	//int k = 0;
	//for (i = 0; i < len2; i++) {
	//	img[i] = (unsigned char)255.0*poly.evaluate(tImg[i]);
	//}

	//for (; i < imgSize; i++) {
	//	img[i] = 0;	// 对剩余部分进行填充
	//}

	//delete tApt;
	//tApt = nullptr;
	//delete tImg;
	//tImg = nullptr;

	//int i = 0;
	//int j = 0;
	//int k = 0;

	//for (i = 0; i < len_in; i += samplesPerWord) {
	//	temp = 0;
	//	for (j = i; j < i + samplesPerWord; j++) {
	//		temp += data[i];
	//	}
	//	temp = temp * 255.0 / double(samplesPerWord);
	//	img[k++] = (unsigned char)temp;
	//}
	//for (; k < imgSize; k++) {
	//	img[k++] = 0;	// 对剩余部分进行填充
	//}
}
