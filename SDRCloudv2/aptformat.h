#pragma once
#ifndef APTFORMAT_H
#define APTFORMAT_H
const int Fc = 2400;	// 调幅载波频率2400Hz;
const int Fw = 4160;	// 每个字的频率是4160Hz;
const int lineLength = 2080;	// 每行有2080个words
const int Fpulse = 1040;	// sync A每个脉冲波形频率是1040Hz
const int numPulse = 7;	// sync A一次同步有7个脉冲波形
const int syncLength = 39;	// sync A同步信号有39个words
const double pi = 3.1415926;



#endif // !APTFORMAT_H
