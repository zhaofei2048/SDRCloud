#pragma once
#ifndef DEMOD_CORE_H
#define DEMOD_CORE_H
#include <QQueue>
// don not edit this file!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// don not edit this file!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// don not edit this file!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void demod_core(double data_in[], unsigned int len_in, double data_out[], unsigned int *len_out);
void demodAM(int Fs, double data_in[], unsigned int len_in, double data_out[], unsigned int &len_out);
void resample_fast(QQueue<qreal> &x, const quint32 len_in, const int L,
	const int M, qreal *coeff, const quint32 coeff_len, qreal *y, quint32 &len_out);

// don not edit this file!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// don not edit this file!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// don not edit this file!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#endif // !DEMOD_CORE_H

