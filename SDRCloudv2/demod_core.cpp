#include "demod_core.h"
#include "constant.h"
#include "math.h"
#include "aptformat.h"
#include <QDebug>

//#include <spline3interp.h>  
//using namespace splab;

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
	static double phi = 2 * pi*Fc / double(Fs);
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
	data_out[i - 1] = data_out[i - 2];
	len_out = i;
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

