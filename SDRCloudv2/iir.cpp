#include "iir.h"
#include <qmath.h>

const int IIR_A_LEN = 3;
const int IIR_B_LEN = 3;
// 50HzÏÝ²¨ÂË²¨Æ÷
//const double IIR_A[] = { 1, -1.99884767026, 0.9988904831675 };
//const double IIR_B[] = { 0.9994452415838, -1.99884767026, 0.9994452415838 };

const double IIR_A[] = { 1,  -0.3075663597922,    0.188345160884 };
const double IIR_B[] = { 0.220194700273,   0.4403894005459,    0.220194700273 };

void iir_downsample(const unsigned char *data_in, const quint32 len_in, qreal data_out[], quint32 &len_out)
{
	static const qreal IIR_A[3] = { 1,   -1.895957017836,    0.901108152734 };
	static const qreal IIR_B[3] = { 0.001287783724582, 0.002575567449164, 0.001287783724582 };
	static qreal ix_n_1 = 0;
	static qreal ix_n_2 = 0;
	static qreal iy_n_1 = 0;
	static qreal iy_n_2 = 0;

	qreal ix_n = 0;
	qreal iy_n = 0;
	// y(n) = 
	int i = 0;
	int count = len_in;
	for (i = 0; i < count; i=i+2)
	{
		ix_n = qreal(qint16(data_in[i])-127.0);
		iy_n = IIR_B[0] * ix_n + IIR_B[1] * ix_n_1 + IIR_B[2] * ix_n_2 - IIR_A[1] * iy_n_1 - IIR_A[2] * iy_n_2;

		iy_n_2 = iy_n_1;
		iy_n_1 = iy_n;

		ix_n_2 = ix_n_1;
		ix_n_1 = ix_n;

		data_out[i] = iy_n;
		//if (qAbs(data_out[i]) < 5)
		//{
		//	data_out[i] = 0.0;
		//}

	}

	static qreal qx_n_1 = 0;
	static qreal qx_n_2 = 0;
	static qreal qy_n_1 = 0;
	static qreal qy_n_2 = 0;

	qreal qx_n = 0;
	qreal qy_n = 0;
	for (i = 1; i < count; i = i + 2)
	{
		qx_n = qreal(qint16(data_in[i]) - 127.0);
		qy_n = IIR_B[0] * qx_n + IIR_B[1] * qx_n_1 + IIR_B[2] * qx_n_2 - IIR_A[1] * qy_n_1 - IIR_A[2] * qy_n_2;

		qy_n_2 = qy_n_1;
		qy_n_1 = qy_n;

		qx_n_2 = qx_n_1;
		qx_n_1 = qx_n;

		data_out[i] = qy_n;
		//if (qAbs(data_out[i]) < 5)
		//{
		//	data_out[i] = 0.0;
		//}
	}
	len_out = len_in;
}

void iir(QQueue<qreal> &data_in, qreal data_out[], quint32 &len)
{
	static qreal x_n_1 = 0;
	static qreal x_n_2 = 0;
	static qreal y_n_1 = 0;
	static qreal y_n_2 = 0;

	qreal x_n = 0;
	qreal y_n = 0;
	// y(n) = 
	int i = 0;
	int count = data_in.count();
	for (i = 0; i < count; i++)
	{
		x_n = data_in.dequeue();
		y_n = IIR_B[0] * x_n + IIR_B[1] * x_n_1 + IIR_B[2] * x_n_2 - IIR_A[1] * y_n_1 - IIR_A[2] * y_n_2;

		y_n_2 = y_n_1;
		y_n_1 = y_n;

		x_n_2 = x_n_1;
		x_n_1 = x_n;

		data_out[i] = 30.0*y_n;
		//if (qAbs(data_out[i]) < 5)
		//{
		//	data_out[i] = 0.0;
		//}

	}

	len = quint32(i);
}