#include "dsp.h"
#include <QDebug>


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
				temp += x[len1 - 1];
			}

		}
		temp /= gapn;
		y[i] = temp;
		//y[i] = 0;
	}
	delete index;
	index = nullptr;
}

// 将数据从sampleRate1重采样到sampleRate2，data_out不需要预先分配空间
void resample(double *data_in, const unsigned int len_in, const int sampleRate1, const int sampleRate2, double *&data_out, unsigned int &len_out)
{
	// 暂时先这样简单处理
	data_out = data_in;
	len_out = len_in;
}