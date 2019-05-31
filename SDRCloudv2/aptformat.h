#pragma once
#ifndef APTFORMAT_H
#define APTFORMAT_H
#define MAX_PEAKS_NUM 6000
#include <QString>
#include <QImage>
#include <imageviewer.h>
const int Fc = 2400;	// 调幅载波频率2400Hz;
const int Fw = 4160;	// 每个字的频率是4160Hz;
const int lineLength = 2080;	// 每行有2080个words
const int Fpulse = 1040;	// sync A每个脉冲波形频率是1040Hz
const int numPulse = 7;	// sync A一次同步有7个脉冲波形
const int syncLength = 39;	// sync A同步信号有39个words
const double pi = 3.1415926;

typedef struct Peak {
	unsigned int index;
	double coe;
}Peak;

void loadWave(const QString fileName, qreal *&data, qint64 &len, int &Fs);
void aptToImage(int Fs, double *data, unsigned int len_in, unsigned char *&img, unsigned int &imgSize, unsigned int &width);
void generateSyncWave(qreal *&hn, int &len, const int Fs);
bool findSync(qreal *signal, const unsigned int signalLen, const qreal *hn, const int hLen, const int Fs, unsigned int *&index, int &indexLen);
bool aptDecode(ImageViewer *viewer, const QString fileName, unsigned char *&img, unsigned int &imgSize, int &width, int &height, bool isSync);
bool arrToImage(const unsigned char *imgarr, const int width, const int height, QImage &image);
void align_signal(double *signal, unsigned int &signal_len, unsigned int *index, int indexLen);
void normalize_contrast(double *signal, const unsigned int signalLen, unsigned char *img, const unsigned int imgSize);


#endif // !APTFORMAT_H
