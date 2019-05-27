/*
 * Filter Coefficients (C Source) generated by the Filter Design and Analysis Tool
 * Generated by MATLAB(R) 9.0 and the Signal Processing Toolbox 7.2.
 * Generated on: 27-May-2019 23:08:02
 */

/*
 * Discrete-Time FIR Filter (real)
 * -------------------------------
 * Filter Structure  : Direct-Form FIR
 * Filter Length     : 196
 * Stable            : Yes
 * Linear Phase      : Yes (Type 2)
 */

/* General type conversion for MATLAB generated C-code  */
#include "tmwtypes.h"
/* 
 * Expected path to tmwtypes.h 
 * D:\MATLAB\extern\include\tmwtypes.h 
 */
const int Len = 196;
const real64_T Num[196] = {
  -0.0001058667805159,-0.0001187556193866,-0.0001803380856738,-0.0002580628804512,
  -0.0003527325828266,-0.0004644842634932,-0.0005923290494668,-0.0007342665884347,
  -0.000887280338924,-0.001046802498289,-0.001207321974329,-0.001361944257692,
  -0.001502921830072,-0.001621725781019,-0.001709348906543,-0.001756723002243,
  -0.001755204554197,-0.001696980250955,-0.001575707222486,-0.001386850278302,
  -0.001128270545483,-0.0008005983586131,-0.0004075668816468,4.378385576975e-05,
  0.0005430654059635, 0.001076678456715, 0.001628024804973, 0.002178028064481,
   0.002705713581157, 0.003189044247363, 0.003605846592703, 0.003934827369681,
   0.004156656299262, 0.004255067425984, 0.004217874724516, 0.004037951023453,
   0.003713964288482,  0.00325095963427, 0.002660644317469, 0.001961376531623,
   0.001177830310536,0.0003403267617515,-0.0005161847811697,-0.001353406277114,
  -0.002131294210332,-0.002809829032022,-0.003350954540698,-0.003720555106948,
  -0.003890384177737,-0.003839851082705,-0.003557583097909,-0.003042630910405,
  -0.002305294911915,-0.001367427476684,-0.0002622215379889, 0.000966574205861,
   0.002266008957292, 0.003575817336324, 0.004830791612643, 0.005963569543691,
   0.006907698859493, 0.007600899681715, 0.007988344422075, 0.008025825654825,
   0.007682656429311, 0.006944135075553, 0.005813453519067, 0.004312924403388,
   0.002484418460168,0.0003889742878175,-0.001894477306167,-0.004271245497371,
  -0.006633818821578,-0.008865538320143, -0.01084486355618, -0.01245008500637,
   -0.01356430529901, -0.01408046310721, -0.01390621479298, -0.01296843625223,
   -0.01121715122024,-0.008628702489723,-0.005207995939866,-0.0009896973472347,
   0.003961695577981, 0.009552967058363,  0.01566444145903,  0.02215335516586,
    0.02885821283978,  0.03560397915966,  0.04220791900084,   0.0484858585771,
    0.05425862181642,  0.05935840308693,   0.0636347957633,  0.06696025937413,
    0.06923478499287,  0.07038957161692,  0.07038957161692,  0.06923478499287,
    0.06696025937413,   0.0636347957633,  0.05935840308693,  0.05425862181642,
     0.0484858585771,  0.04220791900084,  0.03560397915966,  0.02885821283978,
    0.02215335516586,  0.01566444145903, 0.009552967058363, 0.003961695577981,
  -0.0009896973472347,-0.005207995939866,-0.008628702489723, -0.01121715122024,
   -0.01296843625223, -0.01390621479298, -0.01408046310721, -0.01356430529901,
   -0.01245008500637, -0.01084486355618,-0.008865538320143,-0.006633818821578,
  -0.004271245497371,-0.001894477306167,0.0003889742878175, 0.002484418460168,
   0.004312924403388, 0.005813453519067, 0.006944135075553, 0.007682656429311,
   0.008025825654825, 0.007988344422075, 0.007600899681715, 0.006907698859493,
   0.005963569543691, 0.004830791612643, 0.003575817336324, 0.002266008957292,
   0.000966574205861,-0.0002622215379889,-0.001367427476684,-0.002305294911915,
  -0.003042630910405,-0.003557583097909,-0.003839851082705,-0.003890384177737,
  -0.003720555106948,-0.003350954540698,-0.002809829032022,-0.002131294210332,
  -0.001353406277114,-0.0005161847811697,0.0003403267617515, 0.001177830310536,
   0.001961376531623, 0.002660644317469,  0.00325095963427, 0.003713964288482,
   0.004037951023453, 0.004217874724516, 0.004255067425984, 0.004156656299262,
   0.003934827369681, 0.003605846592703, 0.003189044247363, 0.002705713581157,
   0.002178028064481, 0.001628024804973, 0.001076678456715,0.0005430654059635,
  4.378385576975e-05,-0.0004075668816468,-0.0008005983586131,-0.001128270545483,
  -0.001386850278302,-0.001575707222486,-0.001696980250955,-0.001755204554197,
  -0.001756723002243,-0.001709348906543,-0.001621725781019,-0.001502921830072,
  -0.001361944257692,-0.001207321974329,-0.001046802498289,-0.000887280338924,
  -0.0007342665884347,-0.0005923290494668,-0.0004644842634932,-0.0003527325828266,
  -0.0002580628804512,-0.0001803380856738,-0.0001187556193866,-0.0001058667805159
};
