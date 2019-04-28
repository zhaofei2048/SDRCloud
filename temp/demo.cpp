#define BOUNDS_CHECK

#include <iostream>
#include <iomanip>
#include <inverse.h>
#include <complex>
#include <vector.h>
#include <vectormath.h>

using namespace std;
using namespace splab;


Vector<double> demo_am(int length,double s_iq[]) {
	int n = length;
	int i = 0;
	double low = 0;
	double up = n - 1;
	double *si = new double[length / 2] ;
	double* sq = new double[length / 2];

	for (i = 0; i < n / 2; i++)
	{
		si[i] = s_iq[2 * i];
		sq[i] = s_iq[2 * i + 1];
	};

	Vector<double>v_sig(n / 2, low);
	Vector<double>v_si(n / 2,si);
	Vector<double>v_sq(n / 2,sq);
	
	v_sig = v_si * v_si + v_sq * v_sq;

	return v_sig;

}

Vector<double> demo_fm(int length, double s_iq[]) {
	int n = length;
	int i = 0;
	double low = 0;
	double up = n - 1;
	double* si = new double[length / 2];
	double* sq = new double[length / 2];

	for (i = 0; i < n / 2; i++)
	{
		si[i] = s_iq[2 * i];
		sq[i] = s_iq[2 * i + 1];
	};

	Vector<double>v_sig(n / 2, low);
	Vector<double>v_si(n / 2, si);
	Vector<double>v_sq(n / 2, sq);

	v_sig = atan(v_si / v_sq);

	return v_sig;

}

Vector<double> fft_am(int length, double s_iq[]) {
	int n = length;
	int i = 0;
	double low = 0;
	double up = n - 1;
	double* si = new double[length / 2];
	double* sq = new double[length / 2];

	for (i = 0; i < n / 2; i++)
	{
		si[i] = s_iq[2 * i];
		sq[i] = s_iq[2 * i + 1];
	};

	Vector<double>v_sig(n / 2, low);
	Vector<double>v_si(n / 2, si);
	Vector<double>v_sq(n / 2, sq);

	v_sig = atan(v_si / v_sq);
}
int main() {
	int i = 0;
	Vector<double>v;
	double s_iq[10] = {1,2,3,4,5,6,7,8,9,10};
	for (i = 0; i < 100; i++) {
		v = demo_fm(10, s_iq);
		cout << v << endl;
	}
	
}