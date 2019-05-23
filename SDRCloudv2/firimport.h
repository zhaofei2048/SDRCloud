#pragma once
#ifndef FIRIMPORT_H
#define FIRIMPORT_H

typedef double real64_T;
#include "fir2.h"


const quint32 hFirLen = BL % 2 == 0 ? BL / 2 : BL / 2 + 1;
const double *hFir = B;


const quint32 HALF_FIR_LEN = hFirLen % 2 == 0 ? hFirLen : hFirLen - 1;
const quint32 FIR_LEN = quint32(HALF_FIR_LEN * 2);

#endif // !FIRIMPORT_H
