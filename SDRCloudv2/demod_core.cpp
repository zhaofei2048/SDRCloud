#include "demod_core.h"
#include "constant.h"
#include "math.h"

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