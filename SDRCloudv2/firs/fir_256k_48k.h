/*
 * Filter Coefficients (C Source) generated by the Filter Design and Analysis Tool
 * Generated by MATLAB(R) 9.0 and the Signal Processing Toolbox 7.2.
 * Generated on: 27-May-2019 19:26:44
 */

/*
 * Discrete-Time FIR Filter (real)
 * -------------------------------
 * Filter Structure  : Direct-Form FIR
 * Filter Length     : 41
 * Stable            : Yes
 * Linear Phase      : Yes (Type 1)
 * Fs = 768KHz
 * Fpass = 128KHz
 * Fstop = 150KHz
 */

/* General type conversion for MATLAB generated C-code  */

/* 
 * Expected path to tmwtypes.h 
 * D:\MATLAB\extern\include\tmwtypes.h 
 */
// ===============================Perfect========================================
 // * Fs = 768KHz
 // * Fpass = 128KHz
 // * Fstop = 150KHz
const int FIR_256K_TO_48K_LEN = 147;
const double FIR_256K_TO_48K[147] = {
   -0.01460375118539, 0.006272274388774, 0.005516257725988, 0.005111036246682,
   0.004939218739351,  0.00489951299044, 0.004915689221095, 0.004921015544504,
   0.004870805277628, 0.004722877145076, 0.004453332105837,  0.00404764466204,
    0.00350221277874, 0.002817164588764, 0.002015651910655, 0.001113322487238,
  0.0001498525490125,-0.0008439530951675,-0.001819659936042,-0.002738432458997,
  -0.003550380527574, -0.00421717747376,-0.004695134229558,-0.004958492446512,
  -0.004976959741481,-0.004743565016802,-0.004249909552187,-0.003510891888758,
  -0.002543565237834,-0.001387900281068,-8.404529940151e-05, 0.001305415610429,
   0.002724055913962, 0.004095032764894, 0.005356915132942, 0.006431849735377,
   0.007260942400297, 0.007779979792611, 0.007946910249411, 0.007719575268156,
    0.00709149954925, 0.006052008164816,  0.00463347615248, 0.002858527348249,
  0.0007987877721912,-0.001465819863498,-0.003877682657988, -0.00627024932163,
  -0.008577958906311, -0.01067503205085, -0.01243005080848, -0.01371528251274,
   -0.01443791470015, -0.01450679135415, -0.01384269712088, -0.01237998032471,
   -0.01009959083783,-0.006987760934469,-0.003077117869519, 0.001572005421759,
   0.006885216162559,  0.01274879655905,  0.01903891606594,  0.02560009367201,
    0.03227126193594,  0.03887395116604,  0.04523153584069,  0.05116536236152,
    0.05651162338976,  0.06111275696155,  0.06484065465799,  0.06758278194489,
    0.06926215315067,  0.06982663132857,  0.06926215315067,  0.06758278194489,
    0.06484065465799,  0.06111275696155,  0.05651162338976,  0.05116536236152,
    0.04523153584069,  0.03887395116604,  0.03227126193594,  0.02560009367201,
    0.01903891606594,  0.01274879655905, 0.006885216162559, 0.001572005421759,
  -0.003077117869519,-0.006987760934469, -0.01009959083783, -0.01237998032471,
   -0.01384269712088, -0.01450679135415, -0.01443791470015, -0.01371528251274,
   -0.01243005080848, -0.01067503205085,-0.008577958906311, -0.00627024932163,
  -0.003877682657988,-0.001465819863498,0.0007987877721912, 0.002858527348249,
    0.00463347615248, 0.006052008164816,  0.00709149954925, 0.007719575268156,
   0.007946910249411, 0.007779979792611, 0.007260942400297, 0.006431849735377,
   0.005356915132942, 0.004095032764894, 0.002724055913962, 0.001305415610429,
  -8.404529940151e-05,-0.001387900281068,-0.002543565237834,-0.003510891888758,
  -0.004249909552187,-0.004743565016802,-0.004976959741481,-0.004958492446512,
  -0.004695134229558, -0.00421717747376,-0.003550380527574,-0.002738432458997,
  -0.001819659936042,-0.0008439530951675,0.0001498525490125, 0.001113322487238,
   0.002015651910655, 0.002817164588764,  0.00350221277874,  0.00404764466204,
   0.004453332105837, 0.004722877145076, 0.004870805277628, 0.004921015544504,
   0.004915689221095,  0.00489951299044, 0.004939218739351, 0.005111036246682,
   0.005516257725988, 0.006272274388774, -0.01460375118539
};
//=================Perfect end=======================================================

// const int FIR_256K_TO_48K_LEN = 196;
// const double FIR_256K_TO_48K[196] = {
//   -0.0001058667805159,-0.0001187556193866,-0.0001803380856738,-0.0002580628804512,
//   -0.0003527325828266,-0.0004644842634932,-0.0005923290494668,-0.0007342665884347,
//   -0.000887280338924,-0.001046802498289,-0.001207321974329,-0.001361944257692,
//   -0.001502921830072,-0.001621725781019,-0.001709348906543,-0.001756723002243,
//   -0.001755204554197,-0.001696980250955,-0.001575707222486,-0.001386850278302,
//   -0.001128270545483,-0.0008005983586131,-0.0004075668816468,4.378385576975e-05,
//   0.0005430654059635, 0.001076678456715, 0.001628024804973, 0.002178028064481,
//    0.002705713581157, 0.003189044247363, 0.003605846592703, 0.003934827369681,
//    0.004156656299262, 0.004255067425984, 0.004217874724516, 0.004037951023453,
//    0.003713964288482,  0.00325095963427, 0.002660644317469, 0.001961376531623,
//    0.001177830310536,0.0003403267617515,-0.0005161847811697,-0.001353406277114,
//   -0.002131294210332,-0.002809829032022,-0.003350954540698,-0.003720555106948,
//   -0.003890384177737,-0.003839851082705,-0.003557583097909,-0.003042630910405,
//   -0.002305294911915,-0.001367427476684,-0.0002622215379889, 0.000966574205861,
//    0.002266008957292, 0.003575817336324, 0.004830791612643, 0.005963569543691,
//    0.006907698859493, 0.007600899681715, 0.007988344422075, 0.008025825654825,
//    0.007682656429311, 0.006944135075553, 0.005813453519067, 0.004312924403388,
//    0.002484418460168,0.0003889742878175,-0.001894477306167,-0.004271245497371,
//   -0.006633818821578,-0.008865538320143, -0.01084486355618, -0.01245008500637,
//    -0.01356430529901, -0.01408046310721, -0.01390621479298, -0.01296843625223,
//    -0.01121715122024,-0.008628702489723,-0.005207995939866,-0.0009896973472347,
//    0.003961695577981, 0.009552967058363,  0.01566444145903,  0.02215335516586,
//     0.02885821283978,  0.03560397915966,  0.04220791900084,   0.0484858585771,
//     0.05425862181642,  0.05935840308693,   0.0636347957633,  0.06696025937413,
//     0.06923478499287,  0.07038957161692,  0.07038957161692,  0.06923478499287,
//     0.06696025937413,   0.0636347957633,  0.05935840308693,  0.05425862181642,
//      0.0484858585771,  0.04220791900084,  0.03560397915966,  0.02885821283978,
//     0.02215335516586,  0.01566444145903, 0.009552967058363, 0.003961695577981,
//   -0.0009896973472347,-0.005207995939866,-0.008628702489723, -0.01121715122024,
//    -0.01296843625223, -0.01390621479298, -0.01408046310721, -0.01356430529901,
//    -0.01245008500637, -0.01084486355618,-0.008865538320143,-0.006633818821578,
//   -0.004271245497371,-0.001894477306167,0.0003889742878175, 0.002484418460168,
//    0.004312924403388, 0.005813453519067, 0.006944135075553, 0.007682656429311,
//    0.008025825654825, 0.007988344422075, 0.007600899681715, 0.006907698859493,
//    0.005963569543691, 0.004830791612643, 0.003575817336324, 0.002266008957292,
//    0.000966574205861,-0.0002622215379889,-0.001367427476684,-0.002305294911915,
//   -0.003042630910405,-0.003557583097909,-0.003839851082705,-0.003890384177737,
//   -0.003720555106948,-0.003350954540698,-0.002809829032022,-0.002131294210332,
//   -0.001353406277114,-0.0005161847811697,0.0003403267617515, 0.001177830310536,
//    0.001961376531623, 0.002660644317469,  0.00325095963427, 0.003713964288482,
//    0.004037951023453, 0.004217874724516, 0.004255067425984, 0.004156656299262,
//    0.003934827369681, 0.003605846592703, 0.003189044247363, 0.002705713581157,
//    0.002178028064481, 0.001628024804973, 0.001076678456715,0.0005430654059635,
//   4.378385576975e-05,-0.0004075668816468,-0.0008005983586131,-0.001128270545483,
//   -0.001386850278302,-0.001575707222486,-0.001696980250955,-0.001755204554197,
//   -0.001756723002243,-0.001709348906543,-0.001621725781019,-0.001502921830072,
//   -0.001361944257692,-0.001207321974329,-0.001046802498289,-0.000887280338924,
//   -0.0007342665884347,-0.0005923290494668,-0.0004644842634932,-0.0003527325828266,
//   -0.0002580628804512,-0.0001803380856738,-0.0001187556193866,-0.0001058667805159
// };
