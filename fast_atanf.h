#include <stdint.h>
#include <math.h>
#define TAN_MAP_RES 0.003921549f  /* TAN_MAP_RES = 1.0 / 256.0 */
#define TAN_MAP_SIZE 255

/* arctangents from 0 to pi/4 radians. can be generated with the following code:
 *
 * for (i = 0; i < 256; i++) {
 *     t[i] = atanf((float)i / 255.0f);
 * }
 */
static float fast_atan_table[258] = {
    0.000000000, 0.003921549, 0.007842977, 0.011764163, 0.015684988, 0.019605331, 0.023525072, 0.027444089, 
    0.031362265, 0.035279475, 0.039195605, 0.043110527, 0.047024135, 0.050936297, 0.054846898, 0.058755822, 
    0.062662952, 0.066568166, 0.070471346, 0.074372381, 0.078271143, 0.082167529, 0.086061411, 0.089952677, 
    0.093841217, 0.097726911, 0.101609647, 0.105489306, 0.109365791, 0.113238975, 0.117108747, 0.120974995, 
    0.124837629, 0.128696501, 0.132551536, 0.136402607, 0.140249610, 0.144092441, 0.147930995, 0.151765168, 
    0.155594841, 0.159419909, 0.163240284, 0.167055860, 0.170866534, 0.174672201, 0.178472772, 0.182268128, 
    0.186058179, 0.189842835, 0.193621993, 0.197395563, 0.201163441, 0.204925537, 0.208681762, 0.212432027, 
    0.216176227, 0.219914272, 0.223646089, 0.227371588, 0.231090665, 0.234803244, 0.238509253, 0.242208585, 
    0.245901182, 0.249586940, 0.253265768, 0.256937623, 0.260602415, 0.264260024, 0.267910421, 0.271553546, 
    0.275189251, 0.278817534, 0.282438278, 0.286051452, 0.289656937, 0.293254703, 0.296844691, 0.300426781, 
    0.304000944, 0.307567120, 0.311125219, 0.314675212, 0.318217009, 0.321750551, 0.325275809, 0.328792691, 
    0.332301170, 0.335801154, 0.339292616, 0.342775494, 0.346249729, 0.349715292, 0.353172123, 0.356620133, 
    0.360059321, 0.363489658, 0.366911024, 0.370323449, 0.373726815, 0.377121150, 0.380506396, 0.383882463, 
    0.387249380, 0.390607059, 0.393955469, 0.397294581, 0.400624394, 0.403944820, 0.407255858, 0.410557449, 
    0.413849592, 0.417132229, 0.420405358, 0.423668921, 0.426922917, 0.430167317, 0.433402061, 0.436627150, 
    0.439842582, 0.443048298, 0.446244299, 0.449430555, 0.452607036, 0.455773771, 0.458930671, 0.462077737, 
    0.465215027, 0.468342423, 0.471459985, 0.474567622, 0.477665395, 0.480753273, 0.483831227, 0.486899257, 
    0.489957362, 0.493005514, 0.496043712, 0.499071956, 0.502090216, 0.505098522, 0.508096874, 0.511085212, 
    0.514063597, 0.517031968, 0.519990385, 0.522938788, 0.525877237, 0.528805614, 0.531724095, 0.534632564, 
    0.537531018, 0.540419519, 0.543298006, 0.546166599, 0.549025178, 0.551873744, 0.554712415, 0.557541132, 
    0.560359895, 0.563168705, 0.565967619, 0.568756580, 0.571535647, 0.574304819, 0.577064157, 0.579813540, 
    0.582553089, 0.585282803, 0.588002622, 0.590712667, 0.593412876, 0.596103251, 0.598783851, 0.601454735, 
    0.604115844, 0.606767178, 0.609408796, 0.612040758, 0.614662945, 0.617275536, 0.619878411, 0.622471690, 
    0.625055373, 0.627629459, 0.630193949, 0.632748842, 0.635294199, 0.637830138, 0.640356481, 0.642873406, 
    0.645380795, 0.647878826, 0.650367439, 0.652846634, 0.655316472, 0.657777011, 0.660228193, 0.662670076, 
    0.665102720, 0.667526066, 0.669940233, 0.672345161, 0.674740970, 0.677127600, 0.679505110, 0.681873500, 
    0.684232831, 0.686583102, 0.688924432, 0.691256642, 0.693579972, 0.695894361, 0.698199809, 0.700496376, 
    0.702784061, 0.705062985, 0.707333028, 0.709594309, 0.711846888, 0.714090705, 0.716325819, 0.718552291, 
    0.720770121, 0.722979367, 0.725180030, 0.727372110, 0.729555666, 0.731730759, 0.733897328, 0.736055553, 
    0.738205314, 0.740346730, 0.742479742, 0.744604468, 0.746720910, 0.748829126, 0.750929058, 0.753020823, 
    0.755104423, 0.757179856, 0.759247184, 0.761306465, 0.763357639, 0.765400827, 0.767436028, 0.769463241, 
    0.771482587, 0.773493946, 0.775497496, 0.777493179, 0.779481053, 0.781461179, 0.783433557, 0.785398185,
    0.785398185, 0.785398185
};
