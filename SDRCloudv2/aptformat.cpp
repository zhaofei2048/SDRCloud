#include "aptformat.h"
#include "dsp.h"
#include "wavfile.h"
#include "demod_core.h"
#include <QAudioFormat>
#include <QDebug>
#include "fir_12480_4160.h"


// 1.loadWave：此函数通过指定文件名加载对应的数据到data里，并给出数据的采样频率inputSampleRate
// 2.resample：此函数对信号data进行重采样，使其能够被4160整除，从而使得一个word对应整数个采样点数
// 3.demodAM： 此函数负责对信号进行包络解调得到数据signal，长度，采样频率保持不变
// 4.generateSyncWave： 此函数负责生成sync A的同步波形
// 5.findSync：此函数根据给定的波形hn，从signal中寻找peak点
// 6.s_interp1：此函数对一定长度的数据进行重新内插
// 7.aptDecode：此函数总览全局，负责整个解调流程

bool arrToImage(const unsigned char *imgarr, const int width, const int height, QImage &image)
{
	QImage img(width, height, QImage::Format_Grayscale8);
	//for (int i = 0; i < 10; i++) {
	//	qDebug() << data[i];
	//}
	//RGB分量值
	int b = 0;
	int g = 0;
	int r = 0;

	//设置像素
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			b = (int)imgarr[i * width + j];
			g = b;
			r = g;
			img.setPixel(j, i, qRgb(r, g, b));
		}
	}
	image = img;
	return true;
}

const double* fir_12480_4160 = FIR_12480_TO_4160;
const int fir_12480_4160_len = FIR_12480_TO_4160_LEN;
bool aptDecode(ImageViewer *viewer, const QString fileName, unsigned char *&img, 
	unsigned int &imgSize, int &width, int &height, bool isSync)
{
	qint64 len = 0;
	unsigned int signalLen = 0;
	qreal *data;
	qreal *signal;
	int inputSampleRate;
	int work_rate = 12480;	// 实际处理时采用的采样频率，期望它是4160Hz的整数倍

	// 加载波形数据
	viewer->updateStatusInfo("加载wav数据中", 0);
	loadWave(fileName, data, len, inputSampleRate);
	// 重采样
	viewer->updateStatusInfo("重采样中", 1);
	resample(data, len, inputSampleRate, work_rate, signal, signalLen);
	qDebug() << "before AM demod signalLen=" << signalLen;
	for (int i = 0; i < 100; i++) {
		qDebug() << signal[i];
	}

	// AM解调
	viewer->updateStatusInfo("AM解调中", 2);
	demodAM(work_rate, signal, signalLen, signal, signalLen);

	qDebug() << "before conv filter";
	for (int i = 0; i < 10; i++) {
		qDebug() << signal[i];
	}
	// 再次滤波，后面再抽取也就不用滤波了
	conv_filter(signal, signalLen, fir_12480_4160, fir_12480_4160_len);

	qDebug() << "after conv filter";
	for (int i = 1000; i < 1010; i++) {
		qDebug() << signal[i];
	}
	bool ret = false;
	unsigned int *index = nullptr;
	int indexLen = 0;
	if (isSync) {
		// 生成待检测信号波形
		qreal *hn;
		int hLen = 0;
		viewer->updateStatusInfo("生成待检测信号波形中", 3);
		generateSyncWave(hn, hLen, work_rate);
		// 寻找同步点
		
		viewer->updateStatusInfo("sync A同步位置寻找中", 4);
		ret = findSync(signal, signalLen, hn, hLen, work_rate, index, indexLen);
		if (ret == false) {
			viewer->updateStatusInfo("信号太短导致同步失败", 4);
		}
		else {
			// 对信号进行对齐
			align_signal(signal, signalLen, index, indexLen);
		}
	}



	//int samplesPerLine = round(double(work_rate) / Fw * lineLength);
	width = lineLength;
	height = 0;
	//double *imgData = nullptr;
	//int imgLen = 0;

	for (int i = 0; i < 10; i++) {
		qDebug() << signal[i];
	}
	qDebug() << "Fs=" << inputSampleRate;

	viewer->updateStatusInfo("对图像下采样中", 5);

	// 对信号进行抽取
	decimate(signal, signalLen, 3);

	//if (ret == false || isSync == false) {
	//	// 图片没有进行同步
	//	imgLen = round(double(signalLen) / Fs * Fw);
	//	height = ceil(double(imgLen) / width); // imgSize = width*height; width = 2080; height = floor(imgLen/width)
	//	imgData = new double[imgLen];
	//	s_interp1(signal, signalLen, imgData, imgLen);
	//	qDebug() << "==================";
	//	for (int i = 0; i < 10; i++) {
	//		qDebug() << imgData[i];
	//	}
	//}
	//else {

	//	int i = 0;
	//	qreal *x;
	//	height = indexLen;
	//	imgLen = width * height;
	//	imgData = new double[imgLen];
	//	for (i = 0; i < height; i++) {
	//		x = signal+index[i];

	//		s_interp1(x, samplesPerLine, imgData+i*width, width);
	//	}
	//}


	// 图像归一化，contrast调整
	height = ceil(double(signalLen) / width);
	imgSize = width * height;
	img = (unsigned char*)new char[imgSize];

	normalize_contrast(signal, signalLen, img, imgSize);

	//int i = 0;
	//for (i = 0; i < imgLen; i++) {
	//	if (imgData[i] > 1) {
	//		imgData[i] = 1;
	//	}
	//	else if (imgData[i] < 0) {
	//		imgData[i] = 0;
	//	}
	//	else {
	//		;
	//	}
	//	img[i] = (unsigned char)(imgData[i] * 255);
	//	//img[i] = 0;
	//}
	//for (; i < imgSize; i++) {
	//	img[i] = 0;
	//}
		// 及时清理空间
	delete signal;
	signal = nullptr;

	return true;
	
}

// 将fileName文件里的数据读到data里，不需要预先分配空间
void loadWave(const QString fileName, qreal *&data, qint64 &len, int &Fs)
{
	WavFile wav;
	int sampleSize;
	quint64 len2;
	QAudioFormat format;
	
	// 读取文件的长度和样本点大小
	wav.getWaveDataSize(fileName, len2, sampleSize);	// len / sampleSize = 样本点数
	data = (qreal*)(new char[ceil(double(len2) / (sampleSize / 8.0)) * sizeof(qreal)]);

	// 填充数据
	wav.readWave(fileName, data, len, format);
	qDebug() << "wav datasize = " << QString::number(len);
	Fs = format.sampleRate();
}

// 生成一个待匹配信号的波形，无需预先分配空间
void generateSyncWave(qreal *&hn, int &len, const int Fs)
{
	// 为了性能，在此缓存hn
	static int _Fs = 0;
	static int _len = 0;

	if (Fs == _Fs) {	// 请确保hn不要被外界修改
		len = _len;
		return;
	}
	else {
		_Fs = Fs;
	}
	
	int samplesPerWord = round(double(Fs) / Fw);
	int samplesPerLine = round(double(Fs) / Fw * lineLength);

	int samplesPerPulse = 4 * samplesPerWord;

	len = syncLength * samplesPerWord;
	_len = len;	// 缓存
	hn = new qreal[len];
	for (int i = 0; i < len; i++) {
		hn[i] = -1;
	}
	
	int startPos = samplesPerPulse;
	int endPos = startPos + (numPulse - 1)*samplesPerPulse;

	int i = 0;
	for (i = startPos; i <= endPos; i=i+samplesPerPulse) {
		for (int j = i; j <= (i + samplesPerPulse / 2); j++) {
			hn[j] = 1;
		}
	}
}

// 生成一个待匹配信号的波形，无需预先分配空间
void generateSyncWave_s(qreal *&hn, int &len, const int Fs)
{
	// 为了性能，在此缓存hn
	static int _Fs = 0;
	static int _len = 0;

	if (Fs == _Fs) {	// 请确保hn不要被外界修改
		len = _len;
		return;
	}
	else {
		_Fs = Fs;
	}

	int samplesPerWord = round(double(Fs) / Fw);
	int samplesPerLine = round(double(Fs) / Fw * lineLength);

	int samplesPerPulse = 4 * samplesPerWord;

	len = syncLength * samplesPerWord;
	_len = len;	// 缓存
	hn = new qreal[len];
	for (int i = 0; i < len; i++) {
		hn[i] = -1;
	}

	int startPos = samplesPerPulse;
	int endPos = startPos + (numPulse - 1)*samplesPerPulse;

	int i = 0;
	for (i = startPos; i <= endPos; i = i + samplesPerPulse) {
		for (int j = i; j < (i + samplesPerPulse / 2); j++) {	// 这里肯定能整除
			hn[j] = 1;
		}
	}
}

// 根据待检测信号hn，去signal中找同步点，最后通过index返回索引，index不需要预先分配空间
bool findSync(qreal *signal, const unsigned int signalLen, const qreal *hn, const int hLen, const int Fs, unsigned int *&index, int &indexLen)
{
	// 两个peaks的最短距离
	unsigned int maxPos = signalLen - hLen;
	int samplesPerLine = round(double(Fs) / Fw * lineLength);
	if (maxPos <= 0) {
		return false;	// 信号太短不可sync
	}
	int minDistance = round(double(Fs) / Fw * lineLength*0.8);
	Peak *peaks;
	Peak peak;
	
	// 为峰点分配数组
	peaks = new Peak[MAX_PEAKS_NUM];
	peak.index = 0;
	peak.coe = 0;
	peaks[0] = peak;

	unsigned int i = 0;
	unsigned int p = 0;
	double coe = 0;
	for (i = 0; i <= maxPos && (i + samplesPerLine) <= signalLen; i++) {
		coe = 0;
		for (unsigned int j = 0; j < hLen; j++) {
			coe += hn[j] * signal[i + j];
		}
		if (i - peaks[p].index > minDistance) {
			// 如果两个峰点间距足够
			p++;
			peaks[p].index = i;
			peaks[p].coe = coe;
		}
		if (coe > peaks[p].coe) {
			peaks[p].index = i;	// 选最高的峰
			peaks[p].coe = coe;
		}
	}

	if (peaks[p].index + samplesPerLine <= signalLen)
	{
		p++;
	}

	index = new unsigned int[p];
	for (i = 0; i < p; i++) {
		index[i] = peaks[i].index;
	}
	indexLen = p;
	delete peaks;
	peaks = nullptr;
	return true;
}



void align_signal(double *signal, unsigned int &signal_len, unsigned int *index, const int indexLen)
{
	int row = 0;
	int work_rate = 12480;
	int samplesPerLine = round(double(work_rate) / Fw * lineLength);
	unsigned int i = 0;
	unsigned int j = 0;
	for (row = 0; row < indexLen; row++) {
		for (i = index[row]; i < samplesPerLine+ index[row] && i < signal_len; i++) {
			signal[j++] = signal[i];
		}
	}
	signal_len = j;
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

void normalize_contrast(double *signal, const unsigned int signalLen, unsigned char *img, const unsigned int imgSize)
{
	double maxValue = 0;
	double minValue = 0;
	double range = 0;
	unsigned int i = 0;
	for (i = 0; i < signalLen; i++) {
		if (signal[i] > maxValue) {
			maxValue = signal[i];
		}

		if (signal[i] < minValue) {
			minValue = signal[i];
		}
	}
	range = maxValue - minValue;
	if (range == 0) {
		return;
	}
	for (i = 0; i < signalLen; i++) {
		img[i] = (unsigned char)((signal[i]-minValue) * 255.0 / range);
	}
	for (; i < imgSize; i++) {
		img[i] = (unsigned char)0;
	}
}