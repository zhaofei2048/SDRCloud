#pragma once
/*
	宏常量头文件，主要用于方便地统一更改参数设置，便于进行程序的测试
*/
#ifndef CONSTANT_H
#define CONSTANT_H
/** You should not change this below*/
#define DEFAULT_MAX_BUFFER_SIZE 16*16384
#define DEFAULT_MAX_AUDIO_BUFFER_SIZE (16*16384)
#define DEFAULT_SAMPLE_RATE 2048000
#define DEFAULT_AUDIO_RATE 44100



/** Here you can config freely if you known what you are doning. Up to you, but pay attention*/
//==================================CONFIG BEGIN=====================================================================================\\

#define CONFIG_DEFAULT_AUDIO_RATE DEFAULT_AUDIO_RATE	// (Hz)the audio rate your loudspeaker of computer will use to play the sound
#define CONFIG_DEFAULT_TUNNER_FREQ 97399092	//<  97399092Hz; range = 24MHz-1766MHz, f > 50MHz is recommended.
#define CONFIG_DEFAULT_TUNNER_GAIN 500	//< 500 = 50dB; range = 0-50dB, which means 0-500.
#define CONFIG_DEFAULT_SAMPLE_RATE_INDEX 4	//< range = 0-10.
/** here we can get the sample rate:
*	index		sample rate(Hz)
*	0			3200e3,
*	1			2800e3,
*	2			2560e3,
*	3			2400e3,
*	4			2048e3,
*	5			1920e3,
*	6			1800e3,
*	7			1400e3,
*	8			1024e3,
*	9			900001,
*	10			250e3
*/
#define CONFIG_DEFAULT_XTAL_FREQ_CORRECTION 0	// the correction of the clock frequency(ppm)
#define CONFIG_DEFAULT_IS_RTL_AGC_ON true	// enable or disable the AGC of RTL2832u
/// The down sample rate before you demodulate..., you should promise: 
///	CONFIG_DOWNSAMPLE_RATE_BEFORE_DEMOD <= DEFAULT_SAMPLERATES[CONFIG_DEFAULT_SAMPLE_RATE_INDEX]/DEFAULT_AUDIO_RATE
#define CONFIG_DOWNSAMPLE_RATE_BEFORE_DEMOD (_DEFAULT_SAMPLERATES[CONFIG_DEFAULT_SAMPLE_RATE_INDEX]/DEFAULT_AUDIO_RATE)

//==================================CONFIG END=====================================================================================//






#endif // !CONSTANT_H
