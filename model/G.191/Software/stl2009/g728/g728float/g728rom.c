#include "g728.h"
/*
 * (c) Copyright 1990-2007 AT&T Intellectual Property.
 * All rights reserved.
 *
 * NOTICE:
 * This software source code ("Software"), and all of its derivations,
 * is subject to the "ITU-T General Public License". It can be viewed
 * in the distribution disk, or in the ITU-T Recommendation G.191 on
 * "SOFTWARE TOOLS FOR SPEECH AND AUDIO CODING STANDARDS".
 *
 * The availability of this "Software" does not provide any license by
 * implication, estoppel, or otherwise under any patent rights owned or
 * controlled by AT&T Inc. or others covering any use of the Software
 * herein.
 */

#define F Float
static Float wpcfv_16[LPCW]  = {		/* 16 Kbit/sec */
	(F)0.5999755859375,	(F)0.3599853515625,	(F)0.21600341796875,
	(F)0.12957763671875,	(F)0.0777587890625,	(F)0.046630859375,
	(F)0.02801513671875,	(F)0.01678466796875,	(F)0.01007080078125,
	(F)0.00604248046875
};

static Float spfzcfv_16[G728_LPCPF]  = {		/* 16 Kbit/sec */
	(F)0.6500244140625,	(F)0.4224853515625,	(F)0.27459716796875,
	(F)0.17852783203125,	(F)0.11602783203125,	(F)0.075439453125,
	(F)0.04901123046875,	(F)0.0318603515625,	(F)0.02069091796875,
	(F)0.01348876953125
};

G728Rom g728rom = {

/* gq[NG] - gain quantizer levels */
{
	(F)0.515625,
	(F)0.90234375,
	(F)1.579101563,
	(F)2.763427734,
	(F)-0.515625,
	(F)-0.90234375,
	(F)-1.579101563,
	(F)-2.763427734
},

/* y[NCWD][IDIM] - shape code book */
{
 (F)-1.246582031, (F)-0.874023438, (F)-0.612304688, (F)-1.440429688, (F)0.326171875,
 (F)1.620117188, (F)1.419921875, (F)-0.510253906, (F)-2.234863281, (F)-2.45703125,
 (F)-2.172851563, (F)-1.379394531, (F)-0.462890625, (F)-1.307128906, (F)-1.376464844,
 (F)0.616210938, (F)-0.623046875, (F)0.723632813, (F)-0.166015625, (F)-3.261230469,
 (F)-0.622070313, (F)0.087402344, (F)0.625488281, (F)-3.299316406, (F)-0.274414063,
 (F)1.177246094, (F)3.375488281, (F)-2.404785156, (F)-3.481445313, (F)-1.2265625,
 (F)0, (F)-1.891113281, (F)2.286621094, (F)-0.076171875, (F)-1.209960938,
 (F)0.260253906, (F)-1.359863281, (F)-0.233398438, (F)1.044921875, (F)-4.0078125,
 (F)-2.887207031, (F)-3.395996094, (F)0.674316406, (F)1.347167969, (F)0.922363281,
 (F)0.27734375, (F)0.877441406, (F)-2.821289063, (F)-1.201171875, (F)2.481445313,
 (F)-3.664550781, (F)-3.044921875, (F)-2.208496094, (F)-1.615722656, (F)-1.078125,
 (F)1.30078125, (F)-0.561035156, (F)-1.458984375, (F)2.127929688, (F)-1.327636719,
 (F)-5.073242188, (F)-1.203125, (F)1.323730469, (F)0.485839844, (F)0.614746094,
 (F)-1.137207031, (F)1.060058594, (F)-1.338867188, (F)-3.695800781, (F)0.840820313,
 (F)-6.270996094, (F)-3.879882813, (F)-0.418945313, (F)0.364746094, (F)0.50390625,
 (F)0.505371094, (F)-1.240722656, (F)-2.047363281, (F)2.37109375, (F)1.516601563,
 (F)2.872070313, (F)0.200195313, (F)-3.236328125, (F)-0.46875, (F)0.909179688,
 (F)3.516601563, (F)-0.029296875, (F)-1.953125, (F)-1.283203125, (F)-1.217285156,
 (F)0.602050781, (F)-1.870605469, (F)0.75, (F)0.706054688, (F)-1.3046875,
 (F)1.789550781, (F)-2.044433594, (F)0.775878906, (F)3.375, (F)-2.588867188,
 (F)2.293945313, (F)0.67578125, (F)-3.764160156, (F)-3.989257813, (F)-1.6875,
 (F)8.217773438, (F)3.95703125, (F)-5.45703125, (F)-3.031738281, (F)-2.294433594,
 (F)1.253417969, (F)-4.383300781, (F)0.619628906, (F)3.419921875, (F)0.454101563,
 (F)0.585449219, (F)-2.762207031, (F)1.680175781, (F)5.763671875, (F)2.270019531,
 (F)1.577148438, (F)-3.89453125, (F)-4.325683594, (F)-0.089355469, (F)1.241210938,
 (F)6.339355469, (F)-4.094238281, (F)-4.742675781, (F)-0.981933594, (F)-1.40234375,
 (F)-1.4140625, (F)-4.708496094, (F)-1.796875, (F)1.044921875, (F)1.506835938,
 (F)3.091796875, (F)-1.231445313, (F)-1.114746094, (F)3.181152344, (F)-3.715332031,
 (F)2.706542969, (F)-5.435058594, (F)-4.458007813, (F)-2.744140625, (F)-1.627441406,
 (F)5.6484375, (F)-1.410644531, (F)-8.528320313, (F)-3.281738281, (F)-0.198730469,
 (F)-0.90625, (F)-5.295898438, (F)-0.127929688, (F)3.318359375, (F)1.802734375,
 (F)2.41015625, (F)-5.173339844, (F)-1.459472656, (F)6.544921875, (F)3.552246094,
 (F)-2.642578125, (F)1.84375, (F)1.296875, (F)-1.083496094, (F)0.119140625,
 (F)-1.399414063, (F)0.421386719, (F)1.040527344, (F)-2.897460938, (F)-1.974121094,
 (F)-1.001953125, (F)-0.0625, (F)-0.979492188, (F)0.851074219, (F)-1.612304688,
 (F)0.557617188, (F)-0.010253906, (F)-0.772949219, (F)1.631835938, (F)-3.105957031,
 (F)-1.390625, (F)1.242675781, (F)3.123535156, (F)-0.894042969, (F)-1.873535156,
 (F)-1.096191406, (F)0.969726563, (F)2.592285156, (F)-3.1875, (F)-4.556640625,
 (F)-0.240722656, (F)-1.478027344, (F)0.944824219, (F)0.365234375, (F)-2.192382813,
 (F)0.87109375, (F)-2.193847656, (F)1.559082031, (F)2.620117188, (F)-4.519042969,
 (F)-0.922363281, (F)0.516113281, (F)0.911132813, (F)-0.180664063, (F)2.3359375,
 (F)-0.298339844, (F)0.330078125, (F)-1.258300781, (F)-1.313476563, (F)3.584960938,
 (F)-1.000488281, (F)-0.867675781, (F)-0.903320313, (F)1.091308594, (F)-0.245117188,
 (F)-0.07421875, (F)1.078613281, (F)-1.203613281, (F)1.89453125, (F)0.493652344,
 (F)-3.567382813, (F)1.386230469, (F)2.728515625, (F)1.381347656, (F)1.265625,
 (F)-2.171386719, (F)4.492675781, (F)2.907714844, (F)-2.401367188, (F)-1.488769531,
 (F)-2.267578125, (F)-2.173339844, (F)2.831054688, (F)1.908203125, (F)0.340332031,
 (F)-0.586914063, (F)0.156738281, (F)-1.254882813, (F)2.658203125, (F)-1.035644531,
 (F)-0.345703125, (F)0.276367188, (F)1.724609375, (F)-1.018554688, (F)1.389648438,
 (F)0.220703125, (F)1.830566406, (F)2.431640625, (F)0.463867188, (F)-2.089355469,
 (F)0.116210938, (F)-0.083007813, (F)0.839355469, (F)1.709960938, (F)-1.212890625,
 (F)1.152832031, (F)-0.979003906, (F)1.032226563, (F)0.128417969, (F)-1.677246094,
 (F)-1.959472656, (F)-0.59375, (F)4.563964844, (F)-0.58984375, (F)-3.583007813,
 (F)0.176269531, (F)-2.06640625, (F)3.919921875, (F)-0.214355469, (F)-6.590820313,
 (F)-0.614746094, (F)-2.308105469, (F)0.978515625, (F)2.652832031, (F)-1.820800781,
 (F)-0.481933594, (F)-1.802246094, (F)4.115722656, (F)3.780761719, (F)-1.946289063,
 (F)0.3984375, (F)-0.625488281, (F)0.561523438, (F)-0.206542969, (F)2.538085938,
 (F)0.014648438, (F)1.313476563, (F)-1.928222656, (F)2.006347656, (F)3.617675781,
 (F)-0.120117188, (F)-0.668945313, (F)-0.017089844, (F)1.491699219, (F)0.608398438,
 (F)1.547851563, (F)-1.282714844, (F)-0.331054688, (F)2.751464844, (F)-0.727050781,
 (F)0.390136719, (F)-0.518554688, (F)0.98046875, (F)-2.238769531, (F)2.358398438,
 (F)5.227539063, (F)7.293457031, (F)2.243164063, (F)0.350097656, (F)-0.062988281,
 (F)-0.562988281, (F)-2.469238281, (F)0.903320313, (F)1.347167969, (F)0.203613281,
 (F)0.325195313, (F)2.092285156, (F)-2.81640625, (F)3.594238281, (F)-1.897949219,
 (F)-0.828613281, (F)-1.034667969, (F)0.009765625, (F)-0.458007813, (F)0.704589844,
 (F)1.04296875, (F)0.053710938, (F)-1.080078125, (F)-1.661132813, (F)-1.8125,
 (F)-0.905761719, (F)-1.708984375, (F)-0.765625, (F)0.005859375, (F)-1.44140625,
 (F)0.834472656, (F)-0.272460938, (F)0.56640625, (F)-0.845214844, (F)-0.642089844,
 (F)-1.443847656, (F)-0.221679688, (F)0.094726563, (F)-2.230957031, (F)0.04296875,
 (F)-0.075683594, (F)1.017578125, (F)-0.133300781, (F)-0.813476563, (F)-1.386230469,
 (F)-1.195800781, (F)-0.5078125, (F)0.812011719, (F)-1.16015625, (F)-0.092285156,
 (F)-1.307128906, (F)-0.12109375, (F)0.310546875, (F)-0.668457031, (F)-1.387695313,
 (F)-0.475097656, (F)-1.791503906, (F)-1.471191406, (F)0.038574219, (F)0.740722656,
 (F)0.620605469, (F)-0.365722656, (F)-2.59375, (F)-1.217285156, (F)0.934082031,
 (F)-0.893066406, (F)-1.801757813, (F)-1.833984375, (F)-1.623046875, (F)-1.417480469,
 (F)0.548828125, (F)-0.686523438, (F)-1.34765625, (F)-0.755371094, (F)-1.422363281,
 (F)-2.0859375, (F)0.344726563, (F)0.22265625, (F)-0.895507813, (F)0.900390625,
 (F)0.553710938, (F)0.742675781, (F)-0.932128906, (F)-2.078125, (F)0.228027344,
 (F)-3.170898438, (F)-0.728027344, (F)-0.311035156, (F)-0.485351563, (F)-0.062011719,
 (F)-0.282226563, (F)-1.36328125, (F)-1.869140625, (F)-0.998535156, (F)0.426269531,
 (F)0.9609375, (F)-1.490234375, (F)1.28515625, (F)-0.887207031, (F)1.128417969,
 (F)3.096679688, (F)2.005371094, (F)0.924316406, (F)0.583007813, (F)0.312988281,
 (F)1.075683594, (F)-0.707519531, (F)1.0546875, (F)0.584960938, (F)-0.021972656,
 (F)2.075683594, (F)1.294921875, (F)1.717773438, (F)0.836425781, (F)-0.978515625,
 (F)-0.720703125, (F)0.063964844, (F)0.625, (F)-1.9375, (F)1.43359375,
 (F)1.715332031, (F)1.297851563, (F)-0.94140625, (F)0.00390625, (F)1.380371094,
 (F)-0.687011719, (F)-0.850097656, (F)1.312011719, (F)-0.3984375, (F)1.562011719,
 (F)0.633789063, (F)-0.123535156, (F)0.192382813, (F)1.967285156, (F)1.439453125,
 (F)-0.321777344, (F)-0.015625, (F)-2.200683594, (F)0.024902344, (F)2.092773438,
 (F)2.786621094, (F)-1.265625, (F)-2.728515625, (F)2.756835938, (F)1.905761719,
 (F)0.32421875, (F)-2.532714844, (F)-0.784667969, (F)0.602539063, (F)-0.295898438,
 (F)0.833496094, (F)-1.233886719, (F)-1.0703125, (F)1.767578125, (F)-0.256347656,
 (F)-1.10546875, (F)0.396484375, (F)-1.114746094, (F)-1.587402344, (F)2.098144531,
 (F)0.81640625, (F)0.66015625, (F)-1.604980469, (F)0.2578125, (F)2.814941406,
 (F)-1.663574219, (F)-1.598144531, (F)-0.538574219, (F)0.605957031, (F)1.335449219,
 (F)0.705078125, (F)-0.573242188, (F)-1.447753906, (F)0.8046875, (F)1.969238281,
 (F)-0.500976563, (F)0.446777344, (F)0.976074219, (F)0.448242188, (F)0.036132813,
 (F)0.111816406, (F)1.440429688, (F)0.993164063, (F)-0.783691406, (F)-1.21875,
 (F)-0.1015625, (F)-0.6171875, (F)0.007324219, (F)0.994628906, (F)-1.05859375,
 (F)0.469726563, (F)0.728027344, (F)0.283691406, (F)0.747070313, (F)-1.734375,
 (F)-1.066894531, (F)-0.7265625, (F)1.768066406, (F)-1.141601563, (F)-1.275878906,
 (F)-1.102050781, (F)0.69921875, (F)2.704101563, (F)0.039550781, (F)-0.853027344,
 (F)-0.737304688, (F)-1.115234375, (F)0.104492188, (F)0.423339844, (F)-0.497558594,
 (F)-0.678222656, (F)1.245605469, (F)-0.111816406, (F)1.375, (F)-0.822265625,
 (F)-0.492675781, (F)1.358886719, (F)0.233886719, (F)0.24609375, (F)1.321777344,
 (F)0.941894531, (F)0.303222656, (F)-0.779296875, (F)-0.726074219, (F)1.229003906,
 (F)-0.684082031, (F)0.630859375, (F)-2.09375, (F)1.077148438, (F)-0.072265625,
 (F)0.625, (F)0.932128906, (F)-1.333496094, (F)0.606933594, (F)-0.257324219,
 (F)-2.241699219, (F)0.297851563, (F)1.80078125, (F)-0.732910156, (F)1.049316406,
 (F)-0.594238281, (F)1.775390625, (F)0.915527344, (F)-1.645019531, (F)1.614257813,
 (F)-2.431152344, (F)-1.500976563, (F)0.707519531, (F)1.227050781, (F)1.256835938,
 (F)-1.990722656, (F)2.067382813, (F)-1.219238281, (F)0.891601563, (F)0.397460938,
 (F)0.544433594, (F)0.489257813, (F)1.668945313, (F)-0.107421875, (F)0.8125,
 (F)1.533203125, (F)2.993164063, (F)1.913574219, (F)0.809570313, (F)0.381347656,
 (F)0.097167969, (F)0.397949219, (F)0.740234375, (F)1.984863281, (F)0.560546875,
 (F)1.192871094, (F)1.182128906, (F)1.250488281, (F)1.215332031, (F)0.581542969,
 (F)-1.562011719, (F)-0.1796875, (F)2.692871094, (F)-2.888183594, (F)0.375976563,
 (F)-1.020019531, (F)3.382324219, (F)1.827148438, (F)0.511230469, (F)0.581054688,
 (F)-1.198730469, (F)-0.370117188, (F)2.103515625, (F)1.513183594, (F)0.142578125,
 (F)0.041503906, (F)0.676757813, (F)1.974609375, (F)1.914550781, (F)0.255371094,
 (F)-0.675292969, (F)-0.715820313, (F)-0.611328125, (F)0.491210938, (F)2.132324219,
 (F)0.423828125, (F)1.0078125, (F)-0.037597656, (F)0.770996094, (F)1.880859375,
 (F)-0.248535156, (F)0.175292969, (F)-0.098632813, (F)1.425292969, (F)2.494628906,
 (F)0.6328125, (F)0.826660156, (F)1.124511719, (F)1.565429688, (F)1.782226563,
 (F)-1.138671875, (F)-0.583984375, (F)0.111816406, (F)-1.907714844, (F)1.418457031,
 (F)-1.532226563, (F)1.867675781, (F)0.393066406, (F)-1.750488281, (F)2.918457031,
 (F)-1.611816406, (F)-0.131347656, (F)0.025878906, (F)-0.295898438, (F)1.829101563,
 (F)0.194335938, (F)1.984375, (F)-0.642578125, (F)0.985351563, (F)0.295898438
},

/* wnr[NSBSZ] - LPC hybrid window */
{
	(F)0.047760009765625,	(F)0.095428466796875,	(F)0.142852783203125,
	(F)0.189971923828125,	(F)0.236663818359375,	(F)0.28277587890625,
	(F)0.328277587890625,	(F)0.373016357421875,	(F)0.416900634765625,
	(F)0.4598388671875,	(F)0.501739501953125,	(F)0.54248046875,
	(F)0.582000732421875,	(F)0.62017822265625,	(F)0.65692138671875,
	(F)0.69219970703125,	(F)0.72589111328125,	(F)0.757904052734375,
	(F)0.7882080078125,	(F)0.816680908203125,	(F)0.84332275390625,
	(F)0.8680419921875,	(F)0.8907470703125,	(F)0.91143798828125,
	(F)0.9300537109375,	(F)0.946533203125,	(F)0.96087646484375,
	(F)0.9730224609375,	(F)0.98291015625,	(F)0.9906005859375,
	(F)0.996002197265625,	(F)0.999114990234375,	(F)0.999969482421875,
	(F)0.998565673828125,	(F)0.994842529296875,	(F)0.988861083984375,
	(F)0.981781005859375,	(F)0.9747314453125,	(F)0.967742919921875,
	(F)0.9608154296875,	(F)0.953948974609375,	(F)0.94708251953125,
	(F)0.9403076171875,	(F)0.933563232421875,	(F)0.9268798828125,
	(F)0.92022705078125,	(F)0.91363525390625,	(F)0.9071044921875,
	(F)0.900604248046875,	(F)0.894134521484375,	(F)0.887725830078125,
	(F)0.881378173828125,	(F)0.87506103515625,	(F)0.8687744140625,
	(F)0.862548828125,	(F)0.85638427734375,	(F)0.850250244140625,
	(F)0.844146728515625,	(F)0.838104248046875,	(F)0.83209228515625,
	(F)0.826141357421875,	(F)0.820220947265625,	(F)0.8143310546875,
	(F)0.808502197265625,	(F)0.802703857421875,	(F)0.79693603515625,
	(F)0.791229248046875,	(F)0.78558349609375,	(F)0.779937744140625,
	(F)0.77435302734375,	(F)0.768798828125,	(F)0.7633056640625,
	(F)0.7578125,		(F)0.75238037109375,	(F)0.74700927734375,
	(F)0.74163818359375,	(F)0.736328125,		(F)0.731048583984375,
	(F)0.725830078125,	(F)0.720611572265625,	(F)0.7154541015625,
	(F)0.7103271484375,	(F)0.705230712890625,	(F)0.700164794921875,
	(F)0.695159912109375,	(F)0.690185546875,	(F)0.68524169921875,
	(F)0.680328369140625,	(F)0.675445556640625,	(F)0.67059326171875,
	(F)0.665802001953125,	(F)0.661041259765625,	(F)0.656280517578125,
	(F)0.651580810546875,	(F)0.64691162109375,	(F)0.64227294921875,
	(F)0.6376953125,	(F)0.63311767578125,	(F)0.628570556640625,
	(F)0.62408447265625,	(F)0.619598388671875,	(F)0.615142822265625,
	(F)0.610748291015625,	(F)0.60638427734375,	(F)0.602020263671875
},

/* wnrg[NSBGSZ] - Log-gain hybrid window */
{ 
	(F)0.09234619140625,	(F)0.183868408203125,	(F)0.273834228515625,
	(F)0.361480712890625,	(F)0.446014404296875,	(F)0.526763916015625,
	(F)0.602996826171875,	(F)0.674072265625,	(F)0.7393798828125,
	(F)0.79840087890625,	(F)0.8505859375,	(F)0.8955078125,
	(F)0.932769775390625,	(F)0.962066650390625,	(F)0.983154296875,
	(F)0.995819091796875,	(F)0.999969482421875,	(F)0.995635986328125,
	(F)0.982757568359375,	(F)0.96148681640625,	(F)0.9320068359375,
	(F)0.899078369140625,	(F)0.8673095703125,	(F)0.836669921875,
	(F)0.80712890625,	(F)0.77862548828125,	(F)0.751129150390625,
	(F)0.724578857421875,	(F)0.699005126953125,	(F)0.67431640625,
	(F)0.650482177734375,	(F)0.62750244140625,	(F)0.6053466796875,
	(F)0.583953857421875
},

/* wnrw[NSBWSZ] - Weighting filter hybrid window - TX only */
{
	(F)0.059722900390625,	(F)0.1192626953125,	(F)0.178375244140625,
	(F)0.23681640625,	(F)0.29443359375,	(F)0.35101318359375,
	(F)0.40631103515625,	(F)0.460174560546875,	(F)0.51239013671875,
	(F)0.562774658203125,	(F)0.61114501953125,	(F)0.6573486328125,
	(F)0.701171875,		(F)0.742523193359375,	(F)0.781219482421875,
	(F)0.817108154296875,	(F)0.85009765625,	(F)0.880035400390625,
	(F)0.906829833984375,	(F)0.930389404296875,	(F)0.95062255859375,
	(F)0.96746826171875,	(F)0.980865478515625,	(F)0.99072265625,
	(F)0.9970703125,	(F)0.999847412109375,	(F)0.99908447265625,
	(F)0.994720458984375,	(F)0.98681640625,	(F)0.975372314453125,
	(F)0.96044921875,	(F)0.943939208984375,	(F)0.927734375,
	(F)0.91180419921875,	(F)0.896148681640625,	(F)0.8807373046875,
	(F)0.8656005859375,	(F)0.850738525390625,	(F)0.83612060546875,
	(F)0.821746826171875,	(F)0.807647705078125,	(F)0.79376220703125,
	(F)0.780120849609375,	(F)0.7667236328125,	(F)0.753570556640625,
	(F)0.7406005859375,	(F)0.727874755859375,	(F)0.71539306640625,
	(F)0.703094482421875,	(F)0.691009521484375,	(F)0.67913818359375,
	(F)0.66748046875,	(F)0.656005859375,	(F)0.644744873046875,
	(F)0.6336669921875,	(F)0.622772216796875,	(F)0.612091064453125,
	(F)0.6015625,		(F)0.591217041015625,	(F)0.581085205078125
},

/* facv[LPC] - LPC bandwidth expansion weight table(BWEWT) */
{
	(F)0.98828125,		(F)0.9766845703125,	(F)0.96527099609375,
	(F)0.95391845703125,	(F)0.9427490234375,	(F)0.93170166015625,
	(F)0.9207763671875,	(F)0.9100341796875,	(F)0.89935302734375,
	(F)0.8887939453125,	(F)0.87835693359375,	(F)0.86810302734375,
	(F)0.85791015625,	(F)0.84783935546875,	(F)0.83795166015625,
	(F)0.828125,		(F)0.81842041015625,	(F)0.808837890625,
	(F)0.79931640625,	(F)0.78997802734375,	(F)0.78070068359375,
	(F)0.77154541015625,	(F)0.76251220703125,	(F)0.75360107421875,
	(F)0.7447509765625,	(F)0.73602294921875,	(F)0.7274169921875,
	(F)0.7188720703125,	(F)0.71044921875,	(F)0.7021484375,
	(F)0.69390869140625,	(F)0.685791015625,	(F)0.677734375,
	(F)0.6697998046875,	(F)0.66192626953125,	(F)0.6541748046875,
	(F)0.64654541015625,	(F)0.638916015625,	(F)0.6314697265625,
	(F)0.62408447265625,	(F)0.61676025390625,	(F)0.6094970703125,
	(F)0.60235595703125,	(F)0.5953369140625,	(F)0.58831787109375,
	(F)0.5814208984375,	(F)0.57464599609375,	(F)0.56787109375,
	(F)0.56121826171875,	(F)0.5546875
},

/* facgpv[LPCLG] - Log gain BWEWT */
{
	(F)0.90625,		(F)0.8212890625,	(F)0.74432373046875,
	(F)0.67449951171875,	(F)0.61126708984375,	(F)0.553955078125,
	(F)0.50201416015625,	(F)0.4549560546875,	(F)0.41229248046875,
	(F)0.3736572265625
},

/* wzcfv[LPCW] - Weight filter all-zero BWEWT - TX only */
{
	(F)0.9000244140625,	(F)0.80999755859375,	(F)0.72900390625,
	(F)0.6561279296875,	(F)0.59051513671875,	(F)0.53143310546875,
	(F)0.478271484375,	(F)0.43048095703125,	(F)0.38739013671875,
	(F)0.34869384765625
},

/* spfpcfv[G728_LPCPF] - Short term post filter all-pole BWEWT */
{
	(F)0.75,		(F)0.5625,		(F)0.421875,
	(F)0.31640625,		(F)0.2373046875,	(F)0.177978515625,
	(F)0.13348388671875,	(F)0.10009765625,	(F)0.0750732421875,
	(F)0.05633544921875
}

};

/*
 * Initialize the ROM tables
 */
void g728rominit(void)
{
	int	i;
	Float	f;
static	int	first = 1;

	if (first == 0)
		return;
	first = 0;

	/* normal 16 kbps operation */
	g728rom.wpcfv = wpcfv_16;
	g728rom.spfzcfv = spfzcfv_16;
	g728rom.tiltf = (Float)0.15;
	g728rom.pzcf = (Float)0.15;
	for (i = 0; i < NG; i++) {
		/* Double gain */
		g728rom.g2[i] = -(g728rom.gq[i] * (F)2.);
		/* Gain squared */
		g728rom.gsq[i] = g728rom.gq[i] * g728rom.gq[i];	
	}
	for (i = 0; i < NM - 1; i++) {		/* Cell boundaries */
		g728rom.gb[i] = (g728rom.gq[i] + g728rom.gq[i + 1]) / (F)2.;
		g728rom.gb[NM + i] = -g728rom.gb[i];
	}
	g728rom.gb[NM - 1] = (F)1e30;
	g728rom.gb[NG - 1] = -g728rom.gb[NM - 1];
	for(i = 0; i < NCWD; i++)		/* Compute shape gain in db */
		g728rom.y_db[i] = (F)(10. * log10(
			g728_vdotp(g728rom.y[i], g728rom.y[i], IDIM) / IDIM));
	/* Compute gain in db */
	for(i = 0; i < NG; i++)
		g728rom.gq_db[i] = (F)(10.*log10(g728rom.gq[i] *g728rom.gq[i]));
	f = (Float)1.;
	for(i = 0; i < LPC; i++) {
		f *= (F)FE_ATTEN;
		g728rom.fatten[i] = f;
	}
}
