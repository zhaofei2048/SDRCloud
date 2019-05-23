#pragma once
#ifndef IIR_H 
#define IIR_H
#include <QQueue>

void iir(QQueue<qreal> &data_in, qreal data_out[], quint32 &len);
void iir_downsample(const unsigned char *data_in, const quint32 len_in, qreal data_out[], quint32 &len_out);

#endif // !IIR_H 
