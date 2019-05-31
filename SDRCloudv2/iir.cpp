#include "iir.h"
#include <qmath.h>
#include "fir_2048k_256k.h"
#include "fir_2048k_128k.h"
#include <QDebug>



const int IIR_A_LEN = 3;
const int IIR_B_LEN = 3;
// 50Hz陷波滤波器
//const double IIR_A[] = { 1, -1.99884767026, 0.9988904831675 };
//const double IIR_B[] = { 0.9994452415838, -1.99884767026, 0.9994452415838 };

const double IIR_A[] = { 1,  -0.3075663597922,    0.188345160884 };
const double IIR_B[] = { 0.220194700273,   0.4403894005459,    0.220194700273 };

//********************* FM || APT 信号接收参数switch***************************\\

//======================= 调频广播接收============================================
//const int FIR_LEN = FIR_2048K_TO_256K_LEN;
//qreal *coeff = (qreal *)FIR_2048K_TO_256K;
//int D = 8;	// 下采样倍数2048K->256K

//========================APT信号接收=============================================
int D = 16;	// 下采样倍数2048K->128K
const int FIR_LEN = FIR_2048K_TO_128K_LEN;
qreal *coeff = (qreal *)FIR_2048K_TO_128K;


//****************************************************************************//

#define signalI(i) ((2*i+last < 2*FIR_LEN) ? pre[2*i+last]:(qint16(data_in[2*(i-FIR_LEN)+last])-128))
#define signalQ(i) ((2 * i+1+last < 2*FIR_LEN) ? pre[2 * i+1+last] : (qint16(data_in[2*(i-FIR_LEN)+1+last])-128))
#define len_I ceil((len_pre+len_in)/2.0)
#define len_Q (len_pre+len_in)/2

// 关于取signalQ(i)还存在不确定性
// FM版===========================================================================================================
void downsampleWithFir(const unsigned char *data_in, const quint32 len_in, qreal data_out[], quint32 &len_out)
{
	// 这里对信号进行D倍的下采样，并同时使用抗混叠滤波器
	// 使用精度为int16的FIR滤波器对-128->127范围内的信号进行滤波，输出会出现对应的最大值，
	// 这个最大值和具体的FIR滤波器系数
	// 算了还是先用double型的系数吧

	static qreal pre[FIR_LEN*2];	// 最需要缓存2*FIR_LEN个数据
	static quint32 len_pre = 0;	// 上一次剩余数据的长度
	static quint32 last = 2*FIR_LEN;	// 辅助记住信号的开头
	//static int I = 0;
	//static int Q = 0;	// I， Q信号滤波计算当前的起始位置
	quint32 leni = len_I;
	quint32 lenq = len_Q;

	qint64 i = 0;
	qint64 jI = 0;
	qint64 jQ = 0;
	quint32 r = 0;
	quint32 s = 0;
	qreal sum = 0;

	//qDebug() << "downsampling with filter:" << len_in;
	//qDebug() << "len_pre=" << len_pre;
	//qDebug() << "leni=" << leni;
	//qDebug() << "lenq=" << lenq;
	// 重置len_pre为0
	len_pre = 0;

	// 先对I路信号抗混叠滤波并完成下采样
	qint64 h = 0;	// 为相乘求和的头位置
	qint64 t = 0;	// 为当前计算位置，也是相乘求和的尾
	for (t = ceil(FIR_LEN / double(D))*D; t < leni; t = t + D) {
		if (t > FIR_LEN) {					//signal:	x(0) x(1) x(2) x(3)...
			// 如果滤波器的尾部已经进入到信号里//coeff:			 h(0) h(1) h(2)...
			h = t - FIR_LEN;
		}
		else {								//signal:		 x(0) x(1) x(2) x(3) ...
			// 滤波器尾部还没有进入到信号里	//coeff:	h(0) h(1) h(2) h(3)...
			h = 0;
		}

		sum = 0;
		for (i = h; i < t; i++) {	// 卷积求和
			sum += signalI(i)*coeff[i-h];
		}
		data_out[r] = sum;
		r = r + 2;
	}


	h = t - FIR_LEN;
	if (h < leni && h >= 0) {
		// data_in中有剩余的I路数据需要搬移到pre中缓存
		//if (len_in % 2 == 0) {	// 说明末尾的是I路信号

		//}
		jI = 2*FIR_LEN - 2;	 // 因为缓冲区的长度是2*FIR_LEN
		for (i = leni - 1; i >= h; i--) {
			pre[jI] = signalI(i);
			jI = jI - 2;
			len_pre++;
		}
	}
	
	// 再对Q路信号抗混叠滤波并完成下采样
	s = 1;
	for (t = ceil(FIR_LEN/double(D))*D; t < lenq; t = t + D) {
		if (t > FIR_LEN) {					//signal:	x(0) x(1) x(2) x(3)...
			// 如果滤波器的尾部已经进入到信号里//coeff:			 h(0) h(1) h(2)...
			h = t - FIR_LEN;
		}
		else {								//signal:		 x(0) x(1) x(2) x(3) ...
			// 滤波器尾部还没有进入到信号里	//coeff:	h(0) h(1) h(2) h(3)...
			h = 0;
		}

		sum = 0;
		for (i = h; i < t; i++) {	// 卷积求和
			sum += signalQ(i)*coeff[i - h];
		}
		data_out[s] = sum;
		s = s + 2;
	}

	h = t - FIR_LEN;
	if (h < lenq && h >= 0) {
		// data_in中有剩余的Q路数据需要搬移到pre中缓存
		//if (len_in % 2 == 0) {	// 说明末尾的是I路信号

		//}
		jQ = 2*FIR_LEN - 1;
		for (i = lenq - 1; i >= h; i--) {
			pre[jQ] = signalQ(i);
			jQ = jQ - 2;
			len_pre++;
		}
	}
	if (jI > jQ) {
		// 总是保证第一个是I路的
		pre[jI] = pre[jI + 2];
		jI = jI - 2;
		len_pre++;
	}
	Q_ASSERT(r - s == 1 || r - s == -1);
	Q_ASSERT(jI < jQ);
	len_out = r > s ? s : r;	// len_out = min(r,s)
	//qDebug() << "len_out=" << len_out;
	last = jQ + 1;	// 记录pre中数据的开始位置
	Q_ASSERT(len_pre == 2 * FIR_LEN - last);
	
	
	
}
//============================================================================================================

//=========================================APT版==============================================================
//const int FIR_LEN_APT = FIR_2048K_TO_128K_LEN;
//qreal *coeff_APT = (qreal *)FIR_2048K_TO_128K;
//#define signalI_APT(i) ((2*i+last < 2*FIR_LEN) ? pre[2*i+last]:(qint16(data_in[2*i-2*FIR_LEN])-128))
//#define signalQ(i) ((2 * i+1+last < 2*FIR_LEN) ? pre[2 * i+1+last] : (qint16(data_in[2*i-2*FIR_LEN+1]-128)))
//#define len_I ceil((len_pre+len_in)/2.0)
//#define len_Q (len_pre+len_in)/2
//
//void downsampleWithFir_APT(const unsigned char *data_in, const quint32 len_in, qreal data_out[], quint32 &len_out)
//{
//	// 这里对信号进行D倍的下采样，并同时使用抗混叠滤波器
//	// 使用精度为int16的FIR滤波器对-128->127范围内的信号进行滤波，输出会出现对应的最大值，
//	// 这个最大值和具体的FIR滤波器系数
//	// 算了还是先用double型的系数吧
//
//	int D = 16;	// 下采样倍数2048K->256K
//	static qreal pre[FIR_LEN * 2];	// 最需要缓存2*FIR_LEN个数据
//	static int len_pre = 0;
//	static int last = 2 * FIR_LEN;	// 辅助记住信号的开头
//	//static int I = 0;
//	//static int Q = 0;	// I， Q信号滤波计算当前的起始位置
//	quint32 leni = len_I;
//	quint32 lenq = len_Q;
//
//	int i = 0;
//	int jI = 0;
//	int jQ = 0;
//	int r = 0;
//	int s = 0;
//	qreal sum = 0;
//
//	//qDebug() << "downsampling with filter:" << len_in;
//
//	// 先对I路信号抗混叠滤波并完成下采样
//	int h = 0;	// 为相乘求和的头位置
//	int t = 0;	// 为当前计算位置，也是相乘求和的尾
//	for (t = 0; t < leni; t = t + D) {
//		if (t - FIR_LEN > 0) {					//signal:	x(0) x(1) x(2) x(3)...
//			// 如果滤波器的尾部已经进入到信号里//coeff:			 h(0) h(1) h(2)...
//			h = t - FIR_LEN;
//		}
//		else {								//signal:		 x(0) x(1) x(2) x(3) ...
//			// 滤波器尾部还没有进入到信号里	//coeff:	h(0) h(1) h(2) h(3)...
//			h = 0;
//		}
//
//		sum = 0;
//		for (i = h; i < t; i++) {	// 卷积求和
//			sum += signalI(i)*coeff[i - h];
//		}
//		data_out[r] = sum;
//		r = r + 2;
//	}
//
//	h = t - FIR_LEN;
//	if (h < leni && h >= 0) {
//		// data_in中有剩余的I路数据需要搬移到pre中缓存
//		//if (len_in % 2 == 0) {	// 说明末尾的是I路信号
//
//		//}
//		jI = 2*FIR_LEN - 2;
//		for (i = leni - 1; i >= h; i--) {
//			pre[jI] = signalI(i);
//			jI = jI - 2;
//		}
//	}
//
//	// 再对Q路信号抗混叠滤波并完成下采样
//	s = 1;
//	for (t = 0; t < lenq; t = t + D) {
//		if (t - FIR_LEN > 0) {					//signal:	x(0) x(1) x(2) x(3)...
//			// 如果滤波器的尾部已经进入到信号里//coeff:			 h(0) h(1) h(2)...
//			h = t - FIR_LEN;
//		}
//		else {								//signal:		 x(0) x(1) x(2) x(3) ...
//			// 滤波器尾部还没有进入到信号里	//coeff:	h(0) h(1) h(2) h(3)...
//			h = 0;
//		}
//
//		sum = 0;
//		for (i = h; i < t; i++) {	// 卷积求和
//			sum += signalQ(i)*coeff[i - h];
//		}
//		data_out[s] = sum;
//		s = s + 2;
//	}
//
//	h = t - FIR_LEN;
//	if (h < leni && h >= 0) {
//		// data_in中有剩余的Q路数据需要搬移到pre中缓存
//		//if (len_in % 2 == 0) {	// 说明末尾的是I路信号
//
//		//}
//		jQ = 2*FIR_LEN - 1;
//		for (i = leni - 1; i >= h; i--) {
//			pre[jI] = signalQ(i);
//			jQ = jQ - 2;
//		}
//	}
//	Q_ASSERT(r - s == 1 || r - s == -1);
//	Q_ASSERT(jI - jQ == 1 || jI - jQ == -1);
//	len_out = r > s ? s : r;	// len_out = min(r,s)
//	last = ((jI > jQ ? jI : jQ) - 1);	// 记录pre中数据的开始位置
//}
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
		ix_n = qreal(qint16(data_in[i])-128.0);
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
		qx_n = qreal(qint16(data_in[i]) - 128.0);
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