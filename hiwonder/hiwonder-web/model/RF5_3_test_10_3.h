/* This file is automatically generated by SCOptimizer */

#include "mcinfer.h"

fuzzy_model model={

0, /* model_ready set to OK */
1, /* engine code (algorithm type) */
0, /* min_inference_mode */
1, /* database type */
2, /* number of input variables */
4, /* number of output variables */

/* Input variable normalization parameters */
{{0.001053f,-0.057895f},{0.002155f,-0.088362f}},

/* Output variable normalization parameters */
{{0.010526f,-0.105263f},{0.014925f,0.000000f},{0.016949f,-1.881356f},{0.005556f,0.000000f}},

/* number of input MF's per variable*/ 
{20,20},
/* input MF paramteres */ 
{
	{triangular,0.055439f,0,0.110171f,0.058223f},{triangular,0.101754f,0,0.033818f,0.031029f},{triangular,0.137193f,0,0.039397f,0.061012f},{triangular,0.210526f,0,0.084720f,0.058572f},{triangular,0.263158f,0,0.046021f,0.018827f},{triangular,0.275088f,0,0.004881f,0.045323f},{triangular,0.321053f,0,0.046021f,0.025102f},{triangular,0.348421f,0,0.029286f,0.052296f},{triangular,0.392105f,0,0.034516f,0.012551f},{triangular,0.419298f,0,0.041488f,0.088206f},{triangular,0.481053f,0,0.034516f,0.024056f},{triangular,0.503684f,0,0.020918f,0.035561f},{triangular,0.549474f,0,0.055434f,0.063801f},{triangular,0.611228f,0,0.058921f,0.067637f},{triangular,0.661579f,0,0.032424f,0.026148f},{triangular,0.686842f,0,0.024056f,0.040791f},{triangular,0.733333f,0,0.051599f,0.086463f},{triangular,0.795789f,0,0.037653f,0.110868f},{triangular,0.916053f,0,0.128126f,0.095702f},{triangular,0.990263f,0,0.051773f,0.019350f},
	{descending,0.009159f,0,0,0.018292f},{triangular,0.025862f,0,0.015064f,0.015064f},{triangular,0.074353f,0,0.081777f,0.081777f},{triangular,0.119073f,0,0.007532f,0.007532f},{triangular,0.151940f,0,0.058105f,0.058105f},{triangular,0.238147f,0,0.114058f,0.114058f},{triangular,0.306573f,0,0.022596f,0.022596f},{triangular,0.345905f,0,0.055953f,0.055953f},{triangular,0.385237f,0,0.022596f,0.022596f},{triangular,0.407866f,0,0.022596f,0.022596f},{triangular,0.425647f,0,0.012912f,0.012912f},{triangular,0.452586f,0,0.040889f,0.040889f},{triangular,0.483297f,0,0.020444f,0.020444f},{triangular,0.526401f,0,0.065637f,0.065637f},{triangular,0.584590f,0,0.050573f,0.050573f},{triangular,0.646552f,0,0.073169f,0.073169f},{triangular,0.695043f,0,0.023672f,0.023672f},{triangular,0.753233f,0,0.092537f,0.092537f},{triangular,0.863147f,0,0.126970f,0.126970f},{ascending,0.963362f,0,0.073169f,0}
	},

/* number of output MF's per variable*/ 
{5,5,5,5},
/* output MF paramteres */ 
{
	{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},
	{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},
	{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},
	{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0},{exact,0.000000f,0,0,0}
	},

/* Rule database */
400, /*number of rules */

/* index */
{0}, /* empty */
/* database */
{
	0xF8,0xDE,0xF7,0x3E,0xCE,0xA4,0xCD,0x3E,0x9A,0x72,0x9A,0x3E,0x98,0x7B,0x18,0x3F,0xCE,0xA3,0x4D,0x3F,0xDB,0x51,0x5B,0x3F,0xD3,0x10,0xD3,0x3D,0xB8,0x14,0xB8,0x3E,0xF2,0xD4,0x71,0x3E,0xF6,0xBC,0x75,0x3E,0xC4,0x12,0x44,0x3F,0xEB,0x28,0xEB,0x3D,0xCF,0x4A,0x4F,0x3F,0xD7,0x9C,0xD6,0x3E,0x99,0xF0,0x98,0x3E,0x00,0xA6,0x7F,0x3F,0x8D,0xE8,0x0C,0x3E,0xBC,0x74,0x3C,0x3E,0xCC,0xBD,0x4B,0x3F,0xD4,0x90,0x53,0x3F,0x90,0xFA,0x0F,0x3F,0xDB,0x00,0x5B,0x3C,0xAB,0xD3,0x2A,0x3F,0xA4,0x86,0xA3,0x3E,0x89,0x88,0x08,0x3F,0xCF,0xF8,0x4E,0x3F,0x81,0x30,0x81,0x3E,0xF0,0x80,0xEF,0x3C,0x42,0xE3,0xF2,0x3E,0xBD,0xFC,0x56,0x3F,0xF9,0x6C,0x44,0x3F,0xD5,0x22,0x5F,0x3F,0xEA,0xF3,0x3C,0x3F,0x73,0x20,0x3C,0x3F,0x2B,0x2D,0x47,0x3F,0xA2,0xBD,0x6A,0x3F,0x56,0x68,0x07,0x3F,0xD8,0x3E,0x71,0x3F,0xAA,0x7C,0x77,0x3F,0x1C,0x9E,0x5B,0x3F,0x9A,0x00,0x9A,0x3D,0xED,0xAC,0x6C,0x3F,0x85,0x6A,0x05,0x3F,0xA0,0xDD,0x1F,0x3F,0xFF,0x81,0x7E,0x3F,0x9E,0x70,0x9E,0x3E,0xE1,0xB7,0x60,0x3F,0xFB,0x31,0x7B,0x3F,0x83,0xB0,0x02,0x3E,0xC7,0xFD,0x46,0x3F,0x98,0xFC,0x17,0x3E,0xA9,0xF4,0x28,0x3E,0xBF,0x6C,0xBF,0x3E,0xFE,0xCE,0x7D,0x3F,0xAE,0x4A,0x2E,0x3F,0x93,0x2B,0x13,0x3F,0xEA,0x5C,0x6A,0x3F,0xB7,0x29,0x37,0x3F,0x8D,0x00,0x0D,0x3B,0x92,0xF8,0x91,0x3E,0x8D,0xC5,0x0C,0x3F,0xE4,0xE6,0xE3,0x3E,0x90,0xA4,0x8F,0x3E,0xAF,0x12,0x2F,0x3F,
	0xE9,0x74,0x69,0x3F,0xFD,0xFC,0xFC,0x3E,0xD5,0x5B,0x55,0x3F,0xD5,0xED,0x54,0x3F,0x70,0x70,0x61,0x3F,0xBA,0x78,0x1F,0x3F,0x92,0x6A,0xFC,0x3E,0x5C,0x55,0x75,0x3F,0xF6,0xA8,0x2B,0x3F,0xE9,0x54,0xED,0x3E,0x23,0x9E,0x15,0x3F,0xB1,0x93,0x7D,0x3F,0xEB,0x48,0xEB,0x3E,0xDF,0x94,0x5E,0x3F,0xA4,0x28,0xA4,0x3D,0xFD,0xFF,0x7C,0x3F,0xDA,0x3E,0x5A,0x3F,0x99,0x87,0x18,0x3F,0xA3,0x21,0x23,0x3F,0x9B,0x0C,0x1B,0x3E,0xCD,0x04,0x4D,0x3E,0xA8,0xC5,0x27,0x3F,0xF4,0xE0,0xF3,0x3C,0xBC,0x96,0xBB,0x3E,0x8C,0xE4,0x0B,0x3E,0x87,0x70,0x07,0x3D,0xBD,0x10,0xBD,0x3E,0xF3,0x20,0x73,0x3F,0xEA,0xE2,0xE9,0x3E,0x85,0x56,0x05,0x3F,0xF4,0xB1,0x73,0x3F,0xE3,0x24,0x63,0x3E,0xBE,0xF1,0x3D,0x3F,0xDA,0x2B,0x5A,0x3F,0xC2,0xB8,0xC1,0x3D,0x8A,0x58,0x0A,0x3F,0x56,0x41,0x32,0x3F,0xB2,0x5F,0x2E,0x3F,0x40,0x11,0x46,0x3F,0x75,0x18,0x53,0x3F,0xEC,0x68,0xEC,0x3D,0xCF,0xC0,0xCE,0x3E,0x87,0x54,0x07,0x3F,0x92,0xE0,0x91,0x3D,0x1B,0x47,0x0A,0x3F,0xA0,0xFB,0x6D,0x3F,0xCE,0x2A,0x57,0x3F,0x19,0xE9,0x55,0x3F,0xFD,0x8C,0x37,0x3F,0xC7,0x5A,0x3F,0x3F,0x46,0xEE,0x5B,0x3F,0x37,0x09,0x4C,0x3F,0x56,0xF5,0xEB,0x3E,0x32,0x2C,0x6E,0x3F,0xED,0xA3,0x6D,0x3F,0xE3,0xB4,0x55,0x3F,0x99,0xBE,0x18,0x3F,0xA3,0x82,0x22,0x3F,0x8E,0xE8,0x0D,0x3E,0xF1,0x93,0x70,0x3F,0xDC,0x4B,0x5C,0x3F,0xC9,0xC5,0x48,0x3F,0xFC,0x1C,0xFC,0x3E,0xBF,0x02,0xBF,0x3E,
	0xF2,0x40,0xF2,0x3C,0xC1,0xCD,0x40,0x3F,0xA7,0x12,0x27,0x3F,0xBE,0x02,0x3E,0x3F,0xA7,0xB4,0x26,0x3E,0xF6,0x9E,0x75,0x3F,0x92,0xBB,0x11,0x3F,0xF2,0x24,0xF2,0x3E,0xEC,0x3E,0x6C,0x3F,0xCB,0x30,0x4B,0x3D,0xC8,0xC0,0xC7,0x3E,0xA0,0x68,0x20,0x3F,0xB4,0x49,0x34,0x3F,0xF8,0xC0,0xF7,0x3C,0xF3,0x20,0x73,0x3D,0xC7,0xE0,0x46,0x3F,0x95,0x14,0x15,0x3F,0x91,0xF8,0x90,0x3E,0x93,0x6A,0x13,0x3F,0x95,0x78,0x95,0x3D,0x7E,0xA1,0xB7,0x3E,0xE8,0xFB,0xD8,0x3E,0xBB,0x24,0x1E,0x3F,0xDE,0x24,0x78,0x3F,0x94,0xC3,0xCD,0x3E,0x43,0x61,0x08,0x3F,0x6E,0x73,0x04,0x3F,0xC4,0xCC,0x77,0x3F,0xDF,0xAE,0xDE,0x3E,0xB3,0x8E,0x32,0x3F,0x92,0x34,0x92,0x3E,0xC6,0xB6,0x45,0x3F,0xB7,0x0C,0x37,0x3E,0x98,0xF2,0x17,0x3F,0x9C,0x0C,0x1C,0x3F,0xD6,0x6F,0x56,0x3F,0xDB,0x00,0x5B,0x3D,0x84,0xB6,0x83,0x3E,0xBA,0xC0,0x39,0x3C,0x83,0x7C,0x03,0x3E,0xB4,0x48,0xB4,0x3D,0x82,0xF8,0x81,0x3E,0xDE,0x42,0xDE,0x3E,0xF5,0x6C,0x75,0x3E,0x93,0x1E,0x93,0x3E,0x8F,0x76,0x8F,0x3E,0x9B,0xE4,0x1A,0x3F,0xF1,0x6E,0xF1,0x3E,0x06,0xAE,0x18,0x3F,0x5E,0x8B,0x40,0x3F,0x17,0x17,0x50,0x3F,0xD2,0xF1,0x44,0x3F,0xC6,0xC8,0x36,0x3F,0xAD,0x2F,0x29,0x3F,0x37,0x61,0x3D,0x3F,0xD3,0xB2,0x53,0x3F,0xB6,0x40,0xB6,0x3D,0x8A,0x97,0x09,0x3F,0x8C,0x18,0x8C,0x3D,0xE5,0xED,0x64,0x3F,0x19,0xD2,0xE1,0x3E,0xDE,0xE4,0x65,0x3F,0xC6,0x06,0x79,0x3F,0x76,0xE3,0x4E,0x3F,
	0x04,0xCF,0xFF,0x3E,0xA1,0x46,0x7E,0x3F,0xE3,0x83,0x7C,0x3F,0x67,0xDC,0x72,0x3F,0xAC,0xE1,0xDB,0x3E,0x19,0xFC,0x70,0x3F,0x6F,0xB0,0x6F,0x3F,0x17,0x0C,0x57,0x3F,0xC2,0xC0,0x41,0x3E,0xFF,0xAD,0x7E,0x3F,0x8A,0x42,0x0A,0x3F,0xC1,0xDA,0x40,0x3F,0xB3,0x07,0x8C,0x3E,0x9E,0x06,0x2D,0x3F,0xE0,0x03,0x20,0x3F,0x5B,0x1D,0x41,0x3F,0x87,0xD8,0x86,0x3D,0x9F,0xA8,0x9E,0x3E,0xC2,0x3D,0x42,0x3F,0xE9,0x05,0x69,0x3F,0x9E,0xF0,0x1D,0x3F,0xF4,0xB6,0x73,0x3F,0xA4,0xDC,0x23,0x3F,0xEC,0x5C,0x6C,0x3F,0xB1,0x87,0x30,0x3F,0x84,0xEC,0x03,0x3E,0x96,0xC0,0x15,0x3F,0xE7,0xF0,0xE6,0x3D,0xA1,0x02,0xA1,0x3E,0xA3,0x90,0xA2,0x3D,0xAB,0x58,0x2B,0x3F,0xE7,0x44,0xE7,0x3E,0x95,0x98,0x94,0x3E,0xFD,0x23,0x7D,0x3F,0x98,0xED,0x17,0x3F,0x93,0xDE,0x92,0x3E,0x9E,0x8C,0x9D,0x3E,0xB0,0x4A,0xB0,0x3E,0x98,0x16,0x98,0x3E,0xEE,0x60,0xEE,0x3E,0xE0,0x97,0x5F,0x3F,0xEE,0xE0,0x6D,0x3D,0xE2,0x9A,0xE1,0x3E,0xD6,0x8F,0x55,0x3F,0xA2,0x4C,0x22,0x3F,0xC8,0x98,0xC7,0x3E,0xE3,0x09,0x63,0x3F,0x96,0xF0,0x95,0x3E,0x96,0xCF,0x15,0x3F,0xDC,0xF2,0x5B,0x3F,0xD7,0x98,0xD6,0x3D,0x9A,0x25,0x1A,0x3F,0xA7,0x80,0xA6,0x3B,0xFF,0x0A,0x7F,0x3F,0xE1,0x2C,0x61,0x3F,0x9B,0x80,0x1A,0x3C,0xF7,0xF0,0x09,0x3F,0xA0,0xA3,0x4F,0x3F,0x9D,0x57,0x3D,0x3F,0xB2,0xE5,0x52,0x3F,0xD7,0x2F,0x57,0x3F,0xCE,0x6E,0xCE,0x3E,0x87,0x2A,0x87,0x3E,0xA0,0x61,0x20,0x3F,
	0x2C,0xE5,0x0B,0x3F,0x03,0x21,0x4C,0x3F,0xC7,0x02,0x66,0x3F,0x0D,0x80,0x46,0x3F,0x30,0x08,0x0F,0x3F,0x42,0xAF,0x57,0x3F,0x70,0x33,0x43,0x3F,0xB3,0xDA,0x30,0x3F,0x83,0x42,0x03,0x3F,0xCC,0xC5,0x4B,0x3F,0xD3,0x1F,0x53,0x3F,0x8D,0x0A,0x0D,0x3F,0x51,0x32,0xBE,0x3E,0x02,0x14,0xF2,0x3E,0x3A,0xB9,0x03,0x3F,0x35,0x35,0x30,0x3F,0xDC,0x92,0xE2,0x3E,0x91,0x25,0xE8,0x3E,0x7A,0x86,0x10,0x3F,0xA4,0x38,0x39,0x3F,0x86,0xD5,0x05,0x3F,0xB9,0xBE,0x38,0x3F,0xE4,0x94,0xE3,0x3E,0x85,0x84,0x04,0x3F,0xE9,0xE0,0xE8,0x3E,0x91,0xEC,0x90,0x3E,0xF9,0x2E,0x79,0x3F,0xC5,0x96,0x44,0x3F,0x8C,0xE6,0x98,0x3E,0xDA,0xFD,0x34,0x3F,0x8A,0x48,0x30,0x3F,0x03,0x5F,0x4F,0x3F,0xDE,0x37,0x5E,0x3F,0xA3,0x8E,0x22,0x3F,0x9E,0x77,0x1E,0x3F,0xDD,0x86,0xDC,0x3E,0xF2,0xFA,0xF1,0x3E,0x99,0xB8,0x18,0x3F,0xBC,0xA0,0xBB,0x3C,0xEC,0xCE,0x6B,0x3F,0x8E,0x7C,0x8E,0x3E,0xAD,0xFA,0xAC,0x3E,0xD5,0xF8,0xD4,0x3E,0xB8,0xC3,0x37,0x3F,0x7A,0x41,0x66,0x3E,0x7E,0xF2,0x84,0x3E,0x20,0xE8,0x8F,0x3E,0xCC,0xB9,0x59,0x3F,0x81,0xDF,0x07,0x3E,0xB4,0xF6,0xBA,0x3E,0xFF,0xFC,0xA1,0x3E,0x88,0xD2,0x72,0x3F,0x0E,0xCA,0xE8,0x3D,0xD9,0x47,0xAD,0x3E,0x4C,0xAE,0x45,0x3E,0xC4,0x36,0x7D,0x3F,0x8B,0x57,0x0B,0x3F,0xD2,0x18,0xD2,0x3D,0x9A,0xF8,0x19,0x3F,0x84,0xF8,0x83,0x3E,0xC9,0x04,0x49,0x3E,0x86,0x04,0x86,0x3E,0xED,0xB8,0x6C,0x3F,0xE4,0x58,0xE4,0x3E,
	0xD5,0x82,0x54,0x3F,0xE5,0x80,0x64,0x3D,0x86,0x58,0x06,0x3E,0xFA,0xF4,0x79,0x3E,0xC4,0x8C,0x43,0x3F,0xE0,0xCB,0x5F,0x3F,0x90,0xA7,0x0F,0x3F,0xB1,0xE6,0xB0,0x3E,0x05,0x6B,0x1A,0x3F,0x86,0xDC,0x52,0x3F,0xA0,0x1B,0x4F,0x3F,0xB1,0x58,0x30,0x3F,0x64,0x5F,0x4B,0x3F,0xB0,0xE6,0x3D,0x3F,0x6A,0x6C,0x40,0x3F,0x78,0xF4,0x11,0x3F,0xE2,0x03,0x55,0x3F,0x8C,0xF1,0x29,0x3F,0x0C,0x52,0x69,0x3F,0x85,0xFE,0x3C,0x3F,0xD8,0x08,0x1D,0x3F,0xB7,0xD5,0x33,0x3F,0xC4,0xD5,0x63,0x3F,0x89,0x3F,0x2A,0x3F,0xDE,0xA0,0x5D,0x3E,0xA9,0xCD,0x28,0x3F,0xAA,0x99,0x29,0x3F,0xB9,0xA0,0xB8,0x3C,0xE3,0x00,0x00,0x3F,0x64,0xB8,0xEE,0x3E,0xE1,0x75,0x11,0x3F,0x83,0xC1,0x4A,0x3F,0xFC,0x43,0xD0,0x3E,0xDC,0xE4,0x06,0x3F,0xC5,0x9C,0xF2,0x3E,0xFB,0x24,0x2A,0x3F,0xA2,0x34,0xB4,0x3E,0x90,0xE2,0xD8,0x3E,0x62,0x23,0x98,0x3E,0x49,0xEE,0x3B,0x3F,0xA9,0x49,0x88,0x3E,0xB1,0x8E,0x93,0x3E,0x95,0xE6,0x97,0x3E,0x4C,0xAD,0x3E,0x3F,0x06,0x80,0x77,0x3E,0x69,0xF4,0xB1,0x3E,0xE6,0xA6,0x82,0x3E,0xE5,0xC2,0x3E,0x3F,0xA7,0xD9,0x26,0x3F,0x97,0xFC,0x16,0x3F,0xFC,0xC7,0x7B,0x3F,0x9B,0xAB,0x1A,0x3F,0x94,0x03,0xEB,0x3D,0x25,0x52,0x92,0x3E,0xF9,0x3E,0x26,0x3E,0xEA,0x1F,0x47,0x3F,0x6A,0x13,0xC2,0x3D,0x3F,0x1F,0x67,0x3E,0x11,0xA2,0x80,0x3E,0xCA,0xFE,0x59,0x3F,0x82,0x06,0x02,0x3F,0xF8,0x89,0x77,0x3F,0xB7,0x11,0x37,0x3F,0x87,0x7E,0x87,0x3E,
	0xFD,0x5E,0xFD,0x3E,0xAD,0x90,0xAC,0x3D,0x85,0x86,0x04,0x3F,0xBA,0x68,0x3A,0x3E,0xF1,0x40,0xF1,0x3C,0xB9,0x28,0x39,0x3F,0xF6,0xEE,0x75,0x3F,0xAB,0x4D,0x2B,0x3F,0xE4,0x19,0x64,0x3F,0xAB,0xC5,0x2A,0x3F,0xAE,0x20,0xAE,0x3E,0x8C,0x90,0x8B,0x3D,0xB7,0x2B,0x37,0x3F,0xC5,0xEE,0xC4,0x3E,0xC9,0x5B,0x49,0x3F,0x8F,0x3E,0x0F,0x3F,0x2F,0xB4,0xFB,0x3B,0x4D,0x97,0xE6,0x3C,0x01,0x8F,0x9A,0x3D,0x5A,0x3E,0xC0,0x3D,0xB2,0xA0,0xB1,0x3D,0x1B,0x5A,0x01,0x3D,0x1A,0x03,0xB3,0x3D,0x81,0x72,0x67,0x3D,0x3A,0xB6,0x61,0x3F,0xD4,0xB0,0x53,0x3F,0x9C,0x1A,0x1C,0x3F,0xFD,0xC7,0x37,0x3F,0x81,0x5F,0xEC,0x3E,0xB4,0xE3,0x33,0x3F,0xD8,0xA8,0x57,0x3F,0x99,0xBA,0x7E,0x3D,0x9D,0x48,0x37,0x3F,0xAC,0x36,0xAC,0x3E,0xFA,0x17,0x7A,0x3F,0x32,0x1E,0xFE,0x3E,0xD8,0x30,0x58,0x3D,0x50,0x49,0x83,0x3D,0xCD,0x90,0xCC,0x3D,0x32,0x47,0xCB,0x3D,0x79,0x82,0x5F,0x3D,0x00,0x64,0xCC,0x3D,0xB2,0x45,0x65,0x3D,0xED,0x96,0x86,0x3C,0xC0,0x30,0x40,0x3D,0xA6,0x60,0x25,0x3C,0x5B,0x25,0x8E,0x3D,0xED,0xE0,0x6C,0x3D,0x57,0x6D,0x8A,0x3D,0x29,0xE7,0x42,0x3D,0x41,0xFF,0x59,0x3D,0x01,0x2F,0x9A,0x3D,0xB0,0x09,0x17,0x3E,0xB4,0x99,0x7B,0x3E,0x35,0x4D,0xBD,0x3E,0xCC,0x37,0x25,0x3F,0x3A,0xAB,0x9F,0x3E,0x35,0x6C,0x81,0x3E,0xEC,0xD1,0xD1,0x3E,0x90,0x5B,0x36,0x3F,0x34,0x8F,0x81,0x3D,0x11,0x41,0xC2,0x3D,0x19,0x4B,0xD7,0x3D,0xDE,0x4E,0x2A,0x3F,
	0xA6,0x60,0x25,0x3D,0xC4,0xA4,0xC3,0x3D,0xB9,0x8B,0x05,0x3D,0x28,0x57,0xC1,0x3C,0x51,0xA4,0x1B,0x3D,0x19,0xDA,0x03,0x3E,0xD3,0xC3,0x9A,0x3D,0x24,0x36,0xCD,0x3E,0x37,0x9E,0x27,0x3D,0xBD,0xF7,0xA1,0x3E,0xD4,0x4D,0xA4,0x3E,0x2F,0x0B,0x04,0x3F,0xE0,0x77,0xAC,0x3D,0x62,0x31,0x95,0x3D,0x95,0x3A,0xFB,0x3C,0x70,0x92,0x56,0x3D,0x70,0x2D,0xA3,0x3D,0xE4,0xF3,0xB0,0x3B,0xDC,0x7F,0xA8,0x3D,0x90,0xA5,0xC2,0x3D,0x5B,0xCA,0xC0,0x3D,0x94,0x18,0x94,0x3D,0xD5,0x8B,0xA1,0x3D,0xB8,0x83,0x84,0x3C,0x45,0x92,0x2B,0x3D,0x7D,0x09,0xB0,0x3D,0x99,0xA9,0xCB,0x3D,0x26,0x47,0x3F,0x3D,0x8A,0x45,0xBD,0x3D,0xC8,0xB3,0x14,0x3C,0x6F,0x39,0xA2,0x3D,0x56,0x8D,0x89,0x3B,0xB1,0x80,0xB0,0x3D,0xB6,0x3C,0xB6,0x3D,0xF5,0x66,0x8E,0x3C,0x29,0x1B,0xC2,0x3D,0x5C,0x5A,0xC2,0x3B,0x2B,0x96,0x91,0x3D,0xD3,0x80,0xD2,0x3C,0x87,0xB2,0x6C,0x3D,0xA0,0xAA,0x33,0x3F,0xB0,0xC6,0x2F,0x3F,0xD0,0xE5,0x4F,0x3F,0x9B,0xFE,0x19,0x3F,0xFE,0xAF,0xB9,0x3E,0x99,0x8B,0x18,0x3F,0xF0,0x50,0x70,0x3F,0x17,0x7B,0x1E,0x3F,0x51,0xC7,0x7F,0x3E,0x00,0xCA,0x7F,0x3F,0xE5,0x6A,0x65,0x3F,0xBB,0x10,0x21,0x3F,0xA4,0xC0,0xA3,0x3B,0xC5,0x0D,0x78,0x3C,0xA8,0xE0,0xA7,0x3C,0x1F,0x36,0x85,0x3D,0x82,0xFA,0x67,0x3C,0x71,0x0A,0x57,0x3D,0xA0,0xC8,0x9F,0x3D,0x28,0xBA,0x8E,0x3C,0x36,0xD2,0x9B,0x3D,0x7F,0x6D,0xB2,0x3D,0xD1,0x13,0x1E,0x3C,0xB9,0xDB,0x85,0x3D,
	0x42,0x02,0x28,0x3D,0x2C,0x57,0xC5,0x3D,0x6A,0x5A,0x50,0x3D,0x8E,0x1A,0x74,0x3C,0x91,0x60,0x90,0x3E,0x91,0xD1,0x10,0x3C,0xE2,0xA5,0x73,0x3E,0x07,0xFF,0x9F,0x3E,0xC9,0x13,0xBB,0x3E,0xB0,0x31,0x7C,0x3E,0x94,0xB6,0xC2,0x3B,0x54,0xC2,0x39,0x3F,0xA8,0x7B,0xB5,0x3E,0xAE,0xC6,0x16,0x3E,0x97,0xDD,0xFF,0x3C,0x88,0x0C,0x25,0x3F,0xC2,0x30,0xA1,0x3D,0xE0,0xEA,0x89,0x3E,0x3A,0xFA,0x84,0x3E,0x5E,0xD6,0x29,0x3F,0x37,0x10,0xAD,0x3D,0xB1,0x59,0xFF,0x3D,0x9D,0x81,0x8A,0x3E,0xA8,0xE7,0x3C,0x3F,0x7B,0xB5,0xAD,0x3D,0x30,0xC7,0x49,0x3D,0x76,0x02,0x5C,0x3D,0xC3,0xAD,0xF5,0x3C,0x2F,0x52,0x95,0x3D,0xEB,0x16,0x84,0x3C,0x51,0xCD,0x83,0x3D,0xC4,0xF3,0x90,0x3D,0xB5,0x25,0x68,0x3D,0xDC,0x0F,0xA9,0x3D,0x21,0xDC,0x6D,0x3D,0x33,0x66,0x99,0x3D,0x45,0x67,0xDE,0x3C,0x4C,0x87,0x65,0x3C,0x58,0xF9,0x8A,0x3D,0x22,0x14,0x6E,0x3D,0x97,0x7A,0xFD,0x3C,0x0C,0xF3,0xA4,0x3D,0x86,0x50,0x06,0x3D,0x0B,0x84,0x57,0x3D,0x2F,0xAC,0x7B,0x3D,0xA1,0x6D,0xD4,0x3C,0x85,0x82,0x6B,0x3D,0xC2,0x30,0x42,0x3D,0x64,0x8A,0xCA,0x3D,0xF0,0x18,0x70,0x3D,0x17,0x37,0xB0,0x3D,0x13,0x9F,0x2C,0x3D,0x34,0xB2,0x1A,0x3D,0x5A,0x07,0x73,0x3C,0x87,0xA2,0x6D,0x3D,0xCC,0x7B,0x18,0x3D,0x55,0xA9,0x68,0x3C,0xF2,0x0D,0x28,0x3F,0x5D,0x53,0x0D,0x3F,0x12,0x60,0xC7,0x3E,0x8E,0xFB,0xCF,0x3D,0xB9,0x7F,0x6A,0x3D,0xA7,0x7A,0xE2,0x3E,0xDD,0xBD,0x34,0x3F,
	0x3E,0xDB,0xDE,0x3E,0xBD,0x48,0xF9,0x3E,0xB8,0x89,0x0C,0x3F,0x3A,0x49,0x31,0x3F,0x41,0xC7,0x5A,0x3D,0x48,0xAA,0x2E,0x3D,0xA4,0xFC,0xA3,0x3D,0x58,0x1F,0x71,0x3D,0xC5,0xCD,0xF7,0x3C,0x5B,0xE2,0x40,0x3D,0x1C,0x7F,0x35,0x3D,0x63,0x7D,0x96,0x3D,0xB5,0x38,0x35,0x3D,0xBE,0xD3,0x0A,0x3C,0x5C,0xEE,0xC1,0x3D,0xCA,0x60,0xC9,0x3D,0x76,0xC9,0xA8,0x3D,0x80,0x0D,0x33,0x3D,0x64,0x37,0x7D,0x3D,0x09,0xAB,0xA2,0x3D,0x8F,0xD6,0x29,0x3C,0xD1,0x0F,0xD2,0x3C,0xCB,0x9E,0x0A,0x3E,0x73,0xF2,0xD8,0x3E,0xF1,0xE4,0xAC,0x3E,0x57,0x8D,0x5A,0x3E,0x71,0x57,0xEB,0x3C,0x57,0xD6,0x3C,0x3F,0x10,0x13,0x74,0x3D,0xCE,0x5D,0x85,0x3E,0x90,0x5D,0x7E,0x3E,0x9F,0xE9,0x1D,0x3F,0xE0,0x65,0xDC,0x3D,0x28,0x9D,0x8E,0x3E,0x93,0xCE,0xB8,0x3E,0x47,0x54,0x3D,0x3F,0x20,0xF1,0x8A,0x3E,0x61,0xD4,0x71,0x3E,0xA5,0x68,0xEF,0x3D,0x27,0xC7,0xFA,0x3E,0x90,0x5A,0xF6,0x3C,0x6E,0xED,0x20,0x3D,0x7A,0x6D,0xAD,0x3D,0x6F,0x7A,0xD5,0x3C,0x15,0x20,0x40,0x3D,0xE6,0x30,0xE6,0x3C,0x8A,0x7E,0x7C,0x3D,0xB4,0xFD,0x46,0x3F,0x1D,0x7A,0x03,0x3C,0x1F,0x3C,0x6B,0x3D,0x3B,0xB6,0xA1,0x3D,0x50,0x52,0x36,0x3D,0x4F,0x41,0xA2,0x3C,0x7C,0x39,0xAF,0x3D,0x84,0x4E,0x90,0x3C,0x47,0x12,0x1F,0x3F,0x91,0xD5,0xC3,0x3D,0x78,0xA9,0xAA,0x3D,0xEA,0x83,0xB6,0x3D,0x50,0x8D,0x03,0x3C,0xB0,0x2D,0xE3,0x3C,0xB0,0xDC,0xAF,0x3D,0xE2,0x6F,0xAE,0x3D,0x7A,0x09,0xAD,0x3D,
	0xFA,0x82,0x93,0x3D,0x94,0x6D,0xC7,0x3C,0x25,0x2C,0x71,0x3D,0x83,0x48,0x03,0x3D,0x56,0x59,0x89,0x3D,0x94,0xBA,0xF9,0x3C,0x50,0x89,0x83,0x3D,0x32,0x27,0x4B,0x3C,0x01,0x88,0xFF,0x3E,0xF1,0x07,0xFC,0x3E,0xF7,0x49,0x2E,0x3E,0x71,0xEB,0xC5,0x3E,0x2F,0x99,0x42,0x3D,0x86,0x42,0x24,0x3F,0x0C,0x11,0x38,0x3F,0x12,0xFD,0x1F,0x3F,0x28,0x05,0xE4,0x3E,0x17,0xA2,0x0B,0x3F,0xB6,0xFE,0x44,0x3E,0x7E,0xFC,0xC1,0x3E,0x66,0x5F,0x7F,0x3D,0xC9,0x50,0xC8,0x3D,0xB5,0x9D,0x67,0x3D,0xF1,0x00,0x71,0x3B,0xF1,0x12,0x8A,0x3D,0xFB,0xB0,0x7A,0x3D,0xD1,0x60,0x50,0x3C,0x92,0xA0,0x91,0x3C,0xCD,0x93,0x99,0x3D,0xBC,0xBB,0x08,0x3D,0x8F,0x0A,0x75,0x3D,0x85,0xA0,0x04,0x3D,0xC0,0xCF,0x8C,0x3D,0xEF,0x33,0xBB,0x3A,0x87,0xCA,0xEC,0x3C,0xDC,0x70,0xDB,0x3C,0x92,0x11,0xC5,0x3D,0xE2,0xAB,0x2E,0x3D,0x9A,0xDA,0x7F,0x3C,0x5B,0x91,0x8D,0x3D,0xA5,0x88,0x8E,0x3E,0xF3,0x53,0x2B,0x3E,0x77,0xEE,0xA1,0x3D,0xB3,0x7B,0x17,0x3F,0xEE,0xA2,0xFC,0x3D,0x6A,0x17,0xB8,0x3E,0x71,0xF5,0x4B,0x3E,0xA5,0x33,0x41,0x3F,0xCF,0x1C,0x03,0x3C,0x6C,0x67,0x55,0x3D,0xF6,0xF0,0xAD,0x3D,0xCC,0xE7,0x2E,0x3E,0xFE,0xCE,0x42,0x3D,0x53,0x8E,0x56,0x3E,0x58,0xD6,0xFF,0x3D,0x7B,0xD1,0x16,0x3E,0xF0,0x80,0x6F,0x3D,0x81,0xCD,0xB3,0x3A,0x51,0x06,0xB7,0x3D,0x48,0x7F,0x61,0x3D,0xD4,0x98,0xBD,0x3C,0x77,0xCD,0x29,0x3C,0x6E,0x95,0x0E,0x3E,0x96,0x27,0x1B,0x3F,
	0x5B,0x0A,0x41,0x3D,0xDE,0x40,0x5E,0x3C,0x73,0x61,0xA6,0x3D,0xA4,0x20,0x24,0x3D,0x67,0xE9,0x83,0x3D,0xBC,0xD8,0x3B,0x3D,0x32,0x3A,0xEA,0x3D,0x59,0xE1,0x2A,0x3F,0xC2,0x73,0x8E,0x3B,0x66,0x6F,0x7F,0x3D,0xAE,0x0D,0xE1,0x3C,0x5C,0x15,0x0F,0x3D,0x58,0xEE,0xBD,0x3D,0x82,0xA0,0x81,0x3C,0xC0,0xC0,0xBF,0x3B,0x2A,0x3A,0x10,0x3C,0x9C,0x4B,0x1C,0x3F,0x85,0x60,0xF0,0x3E,0xD9,0xDC,0x58,0x3F,0xFE,0xCD,0xF7,0x3E,0xEE,0x38,0x6E,0x3F,0x5A,0x8E,0xE0,0x3E,0xEC,0xD6,0x6B,0x3F,0x6C,0xDC,0xDB,0x3C,0xFA,0x7E,0x7A,0x3F,0xDC,0xBE,0x07,0x3F,0xF5,0xE0,0xF4,0x3E,0x5B,0x2B,0x30,0x3F,0x7F,0xB5,0xC1,0x3E,0x86,0x42,0xA4,0x3E,0xEB,0x17,0x1F,0x3D,0x7C,0x8D,0x2E,0x3F,0x72,0xE5,0xE9,0x3E,0xB7,0x5C,0x1F,0x3F,0x6D,0xA0,0x2A,0x3E,0xC0,0x87,0x23,0x3F,0x7D,0xEE,0x94,0x3E,0x58,0x0D,0x0B,0x3C,0x1C,0x39,0x46,0x3E,0xC6,0xA5,0x34,0x3E,0x0A,0xAF,0xD3,0x3E,0xAD,0x65,0x60,0x3D,0x88,0xAF,0x90,0x3E,0xD5,0xD8,0x07,0x3F,0x8F,0x65,0x0A,0x3F,0xF3,0x53,0xBF,0x3D,0x81,0x06,0x44,0x3E,0xD5,0xC0,0xD4,0x3C,0xC3,0x40,0x43,0x3D,0x32,0x2A,0x98,0x3C,0x8D,0x0C,0x8D,0x3D,0xF4,0x56,0x8D,0x3D,0xCB,0xD3,0x97,0x3C,0x91,0x6D,0x44,0x3D,0x65,0xED,0x97,0x3C,0xCC,0x00,0x4C,0x3B,0xB3,0xA6,0x8C,0x3E,0x6C,0xD3,0x2A,0x3E,0x3D,0x84,0x5A,0x3C,0x3E,0xD9,0xF3,0x3E,0x5C,0x29,0x86,0x3E,0x35,0x67,0x32,0x3E,0xF9,0xC2,0x88,0x3D,0x10,0x36,0x04,0x3F,
	0x18,0x32,0x93,0x3C,0x76,0x41,0xA9,0x3D,0x60,0x73,0xE9,0x3D,0x8E,0xB7,0x9E,0x3D,0x2B,0x6F,0x18,0x3D,0x35,0x9A,0x1B,0x3C,0x40,0x7F,0xF6,0x3C,0xEE,0x4F,0x07,0x3F,0xFC,0x02,0x95,0x3D,0x89,0xF0,0x88,0x3C,0x96,0xC2,0x7B,0x3D,0xA8,0xA5,0x5A,0x3D,0x84,0x41,0x96,0x3D,0xE7,0x33,0xB3,0x37,0x4C,0xA2,0xE8,0x3D,0x60,0x24,0x3D,0x3F,0xC0,0x05,0x73,0x3D,0x91,0x62,0x77,0x3D,0xC9,0x78,0xC8,0x3D,0x12,0x74,0x5E,0x3D,0x36,0x3D,0x29,0x3D,0x29,0x3C,0x75,0x3D,0x98,0x6C,0xD5,0x3D,0xDF,0xE8,0x99,0x3E,0x1B,0x37,0xB4,0x3D,0x68,0x19,0x9B,0x3D,0x94,0xAA,0x79,0x3D,0x2E,0xA7,0xC7,0x3C,0xAF,0x0D,0x62,0x3C,0x82,0xDA,0xE7,0x3B,0x28,0xEA,0x8D,0x3D,0x02,0x1F,0x9B,0x3D,0xFC,0x10,0x7C,0x3F,0x2B,0x70,0xD7,0x3E,0x00,0xF6,0x7F,0x3F,0x07,0x18,0x1B,0x3F,0x91,0xDC,0x10,0x3F,0xF9,0x23,0x28,0x3F,0xFE,0xD5,0x7D,0x3F,0x30,0x4B,0x20,0x3F,0xBB,0x56,0x3B,0x3F,0x68,0x95,0x4D,0x3F,0xDD,0x03,0x5D,0x3F,0x51,0x60,0x09,0x3F,0xD3,0xF8,0x52,0x3D,0xC7,0xAD,0x79,0x3D,0xEB,0x56,0x04,0x3D,0x88,0x9A,0x6E,0x3C,0xC5,0xA0,0x44,0x3C,0x27,0x82,0x8D,0x3D,0xE3,0xCF,0xAF,0x3D,0x0E,0xEF,0x27,0x3D,0xBA,0x1F,0xA7,0x3E,0xFA,0x56,0x93,0x3D,0x1F,0x54,0x0F,0x3D,0xE1,0x58,0x83,0x3E,0x48,0x0A,0xF2,0x3E,0x34,0x07,0x4D,0x3C,0x31,0x43,0x7B,0x3E,0xA2,0xF2,0x10,0x3F,0xC3,0x00,0x0D,0x3F,0x2D,0x67,0x46,0x3C,0x53,0x3B,0x68,0x3E,0xE2,0x31,0xE2,0x3E,
	0x07,0xC7,0xA0,0x3C,0x72,0x15,0xA5,0x3D,0xAE,0x2D,0x61,0x3C,0x1A,0x96,0x80,0x3D,0xDD,0x53,0xA9,0x3D,0x2F,0x1A,0x95,0x3B,0xC4,0x2D,0x77,0x3D,0x83,0xC0,0x82,0x3C,0xA7,0x4D,0xDA,0x3B,0x8E,0x25,0xC1,0x3D,0xF0,0x3B,0xBC,0x3D,0xAF,0xA0,0x2E,0x3C,0xC6,0xC8,0xC5,0x3D,0x95,0xC8,0x14,0x3D,0xEE,0x1E,0x07,0x3D,0x31,0x1A,0x97,0x3B,0xA8,0x00,0x28,0x3D,0xEC,0xD6,0x85,0x3C,0xFF,0x56,0x98,0x3D,0x7E,0x8D,0xB0,0x3D,0xB3,0x0D,0x66,0x3C,0x91,0xEA,0x76,0x3D,0xCF,0x50,0xCE,0x3C,0xA4,0xCD,0xD6,0x3B,0x3B,0xF7,0xD3,0x3C,0x65,0x52,0x4B,0x3D,0xBB,0x18,0xBB,0x3D,0xDA,0xB3,0x26,0x3D,0x83,0x41,0xB6,0x3D,0x93,0x32,0x79,0x3D,0x5A,0xBA,0xBF,0x3C,0x2D,0x7B,0xC6,0x3D,0xB0,0x80,0x2F,0x3C,0x6E,0xA5,0x20,0x3D,0x61,0xBD,0x93,0x3C,0xE8,0x00,0xE8,0x3B,0x3E,0x2E,0xA4,0x3D,0x98,0x70,0x97,0x3D,0xF8,0x36,0x91,0x3D,0x98,0xB1,0xCA,0x3D,0xDA,0x8B,0x26,0x3D,0x1A,0xE4,0xE6,0x3C,0x94,0x1D,0xC7,0x3C,0xCF,0x60,0x4E,0x3D,0x13,0x57,0xAC,0x3C,0xC7,0xB8,0x46,0x3D,0x76,0x49,0xA9,0x3D,0xB6,0x4D,0x69,0x3D,0x98,0x14,0x18,0x3F,0x60,0xCA,0xD9,0x3E,0xF0,0x3F,0x70,0x3F,0x97,0x62,0x05,0x3E,0x86,0x2E,0x06,0x3F,0x6F,0x13,0x9C,0x3E,0x8C,0x3C,0x8C,0x3E,0xBE,0x69,0xCE,0x3D,0xB6,0x97,0x35,0x3F,0x63,0xDC,0xC6,0x3E,0xAD,0x42,0x2D,0x3F,0x30,0xC7,0x04,0x3F,0xFF,0x00,0xFF,0x3A,0x77,0x0D,0xAA,0x3C,0xAE,0x4D,0x61,0x3D,0xBB,0x25,0x6E,0x3D,
	0x36,0x62,0x9C,0x3D,0x79,0xE2,0x5E,0x3D,0x18,0x07,0xB1,0x3C,0xFF,0x66,0x18,0x3D,0x62,0xAF,0x94,0x3E,0xB1,0xF8,0x30,0x3D,0xD5,0x24,0x03,0x3D,0x69,0xC3,0xA4,0x3E,0xEF,0x39,0x31,0x3E,0x6E,0x9D,0xA0,0x3C,0xC4,0x37,0x8B,0x3E,0xEF,0x9F,0xEE,0x3E,0xE0,0x97,0xCC,0x3E,0x1C,0x8B,0xB5,0x3D,0x29,0x5C,0xFD,0x3D,0x19,0x0A,0xE6,0x3E,0x2A,0x73,0xC3,0x3D,0xA6,0x60,0xA5,0x3D,0xD6,0xB0,0xD5,0x3C,0x0F,0xE7,0x28,0x3C,0x37,0x67,0x50,0x3A,0xF8,0x13,0xC5,0x3D,0xF2,0x62,0x8B,0x3D,0x7E,0x85,0xB1,0x3D,0x38,0xBA,0x9E,0x3C,0x40,0xA6,0xA6,0x3D,0x0F,0x13,0xA8,0x3D,0xC5,0xDD,0x77,0x3D,0xDF,0xE4,0xBD,0x3D,0xD3,0xCB,0x9F,0x3D,0x3F,0xFC,0x11,0x3B,0x9E,0x4A,0x04,0x3F,0x06,0xB1,0x19,0x3E,0x55,0x02,0x3B,0x3D,0x5C,0xBE,0x2A,0x3E,0xF8,0x15,0xC5,0x3D,0xEC,0xF0,0x3F,0x3D,0x94,0x2D,0xC7,0x3D,0x5F,0xC6,0xCA,0x3D,0x2C,0x82,0xF0,0x3E,0x12,0xD2,0x97,0x3D,0xD9,0x5B,0xA5,0x3D,0xC6,0x4D,0x70,0x3B,0xBA,0xBE,0xF1,0x3E,0x65,0x67,0xD2,0x3C,0xE9,0xB3,0xB5,0x3C,0x71,0x6D,0x1E,0x3C,0x7C,0x35,0x5C,0x3E,0x8F,0x00,0x0F,0x3B,0x57,0xAD,0x09,0x3D,0x2D,0xA4,0x79,0x3D,0xDC,0x0B,0xA9,0x3D,0x96,0xDA,0xFB,0x3B,0x98,0xBA,0x7D,0x3C,0x55,0x9A,0xBB,0x3B,0xBF,0x8F,0x8B,0x3D,0xC5,0xF0,0xC4,0x3C,0xB5,0xF3,0x01,0x3D,0x73,0x0D,0xA6,0x3D,0xFB,0x80,0x7A,0x3B,0x99,0x1A,0xFF,0x3B,0x5E,0xDF,0x76,0x3D,0x01,0x57,0x9A,0x3C,0xCE,0x13,0x9B,0x3D,
	0xDD,0x83,0x12,0x3F,0x18,0xE8,0x8E,0x3E,0x47,0x63,0x0B,0x3F,0x96,0xEA,0xF3,0x3E,0x73,0xC2,0x21,0x3F,0xEA,0x6C,0xD9,0x3E,0xFC,0x71,0x4E,0x3F,0x68,0x5C,0xFB,0x3E,0x86,0x8C,0x63,0x3F,0xB4,0xB3,0x2B,0x3F,0xA2,0x7D,0x76,0x3E,0xD1,0xAA,0xD3,0x3E,0x1E,0x17,0x37,0x3D,0x8A,0xED,0x3C,0x3C,0xB9,0x70,0xB8,0x3D,0x5B,0x42,0xC1,0x3D,0x4B,0x42,0x31,0x3D,0x54,0xCD,0x86,0x3C,0xF0,0x5E,0x89,0x3D,0x05,0x7F,0x9E,0x3D,0xAD,0x97,0x70,0x3D,0x36,0x10,0xE3,0x3E,0xA5,0xAB,0x2C,0x3E,0x9C,0x9C,0xF1,0x3E,0x4B,0xFF,0xF6,0x3E,0x5F,0xFA,0x93,0x3D,0x37,0x0A,0xDD,0x3D,0x99,0xC9,0x1C,0x3F,0x32,0x0F,0x08,0x3F,0x0A,0x34,0x4C,0x3F,0x93,0xA0,0xBA,0x3E,0x0C,0x84,0x03,0x3F,0xC9,0xB3,0x95,0x3B,0xF8,0x1F,0xC4,0x3D,0x13,0x34,0xDF,0x39,0x49,0x1A,0x2F,0x3D,0xEF,0xDA,0x88,0x3D,0x92,0x9C,0x91,0x3D,0xDE,0x00,0xDE,0x3B,0x82,0x90,0x01,0x3D,0xC6,0xB3,0x92,0x3D,0x17,0x8F,0xB0,0x3D,0x94,0x8D,0x46,0x3D,0x33,0x7A,0x19,0x3D,0x39,0xD0,0x69,0x3E,0x83,0xC0,0x82,0x3B,0x19,0xA2,0x9E,0x3C,0x77,0x67,0x1D,0x3F,0xAD,0xA6,0xF1,0x3D,0x99,0xE9,0xAA,0x3E,0x3B,0x4D,0x87,0x3D,0x70,0xC6,0xBC,0x3E,0x31,0xBD,0x59,0x3E,0xE3,0x9F,0x9D,0x3D,0x5D,0xA4,0x04,0x3E,0x83,0xE4,0x17,0x3F,0xC6,0x04,0xBA,0x3B,0xB4,0xBB,0x80,0x3D,0x1B,0x34,0x60,0x3D,0x28,0x0B,0xEE,0x3E,0xB6,0x21,0x48,0x3D,0xEA,0x6B,0xB6,0x3D,0xD5,0x1D,0x7F,0x3D,0x02,0xD6,0xC2,0x3E,
	0x0B,0x53,0xA4,0x3D,0xBF,0x33,0x0B,0x3D,0x4B,0x0F,0x64,0x3D,0xF8,0x28,0x78,0x3D,0x6B,0xAD,0x1D,0x3D,0xF5,0xE6,0x0E,0x3C,0xF4,0x56,0x8D,0x3D,0x29,0xA4,0x75,0x3D,0xFE,0x88,0x7D,0x3D,0x66,0x07,0x7F,0x3D,0x3A,0x3A,0xA0,0x3D,0x94,0x1A,0x7A,0x3C,0xF9,0xE3,0xC5,0x3C,0x32,0x0F,0xCB,0x3D,0x20,0xB7,0xB9,0x3D,0xA4,0x24,0xA4,0x3D,0x0C,0x3E,0x2D,0x3E,0xAC,0xC9,0x00,0x3F,0xF7,0x7A,0xBA,0x3E,0xF6,0xDD,0xF8,0x3E,0xDD,0x39,0x41,0x3F,0x91,0x7A,0x33,0x3F,0x0C,0xC7,0xBD,0x3E,0xCF,0xA6,0xAC,0x3E,0x21,0xA6,0x4F,0x3F,0xD8,0x67,0x05,0x3F,0xD9,0xBF,0x32,0x3F,0x9C,0x16,0x0E,0x3F,0x5A,0xE6,0xBF,0x3D,0x9A,0xBA,0x7F,0x3C,0x64,0x82,0x4A,0x3D,0x34,0xE2,0x99,0x3D,0xF8,0x86,0x11,0x3C,0x1C,0x5A,0x02,0x3C,0x12,0xCC,0x5E,0x3D,0xCC,0x53,0x98,0x3D,0xCC,0xE5,0x3D,0x39,0xE3,0xB4,0x62,0x3F,0xC9,0x89,0x3A,0x3F,0x63,0x24,0xA9,0x3E,0xF3,0x25,0x3A,0x3D,0xAD,0xD6,0x2C,0x3F,0x0B,0x75,0x42,0x3E,0x0A,0x1C,0xB4,0x3E,0x69,0xF7,0xD6,0x3D,0xE3,0x9B,0x62,0x3F,0xC6,0xE4,0x11,0x3F,0x8E,0xD5,0x45,0x3E,0x3B,0x60,0x31,0x3E,0xB8,0x75,0x5D,0x3E,0x52,0x5A,0x3E,0x3E,0x69,0x3C,0x93,0x3E,0x7B,0x17,0x8E,0x3E,0x53,0x21,0x4C,0x3E,0xE7,0xA3,0x3C,0x3E,0xDB,0x54,0xCC,0x3E,0x82,0x75,0xB5,0x3D,0x59,0x8D,0x0C,0x3D,0xAE,0x00,0x2E,0x3C,0xEE,0x62,0x87,0x3D,0x1F,0x87,0xB8,0x3D,0x9F,0x45,0x52,0x3D,0xF2,0x00,0x72,0x3C,0x80,0x40,0x80,0x3C,
	0x5F,0xAE,0xC4,0x3D,0x30,0x19,0xBC,0x39,0x24,0x6B,0x46,0x3E,0xC6,0x7E,0xF7,0x3E,0x10,0xAE,0x00,0x3D,0x43,0x19,0xCB,0x3E,0x2B,0x2D,0x78,0x3E,0xF3,0xA3,0xE0,0x3E,0x8A,0x5F,0x0C,0x3E,0xCC,0x00,0x8C,0x3D,0x8C,0xD0,0x7F,0x3D,0x93,0x77,0x23,0x3D,0xF7,0x74,0xEB,0x3C,0xBC,0x27,0x93,0x3E,0x31,0x53,0x9E,0x3E,0xAA,0xD0,0xA9,0x3E,0x36,0xB7,0x4F,0x3D,0xB5,0x57,0x81,0x3D,0x55,0xA5,0x87,0x3D,0x66,0x9A,0x4C,0x3D,0x0F,0x3F,0xA8,0x3D,0xD5,0x50,0x54,0x3D,0xCA,0x6C,0xC9,0x3D,0x2E,0xBA,0x94,0x3D,0x4B,0x67,0xE4,0x3A,0x9E,0xD0,0x9D,0x3C,0x3F,0x87,0xD8,0x3C,0x6F,0x7A,0xD5,0x3C,0x5C,0xEA,0x41,0x3D,0xB3,0xC8,0xB2,0x3D,0x2D,0xB7,0xC6,0x3D,0x54,0x67,0xED,0x3A,0x51,0x07,0x6A,0x3C,0x2A,0x24,0x76,0x3D,0x2C,0x34,0x78,0x3B,0xF3,0x26,0x8C,0x3D,0xA4,0xE5,0x56,0x3D,0x0D,0xD4,0xD9,0x3C,0x2E,0xD2,0x93,0x3D,0xF7,0x5E,0x10,0x3D,0x83,0x69,0xFB,0x3E,0x61,0x70,0x5B,0x3E,0xF8,0x63,0xDF,0x3E,0x44,0xFD,0xBB,0x3E,0x80,0xF5,0xB2,0x3D,0x57,0x9A,0xBD,0x3D,0x95,0x05,0xC8,0x3D,0xB7,0x5B,0x83,0x3D,0x9B,0x00,0x1B,0x3B,0x59,0x0D,0x0C,0x3D,0x42,0x16,0xA8,0x3D,0x5B,0xE2,0x40,0x3D,0x80,0xB3,0x78,0x3C,0xBA,0x19,0x3A,0x3F,0xDE,0x8C,0x15,0x3E,0x2F,0x22,0x96,0x3C,0x2B,0xC5,0xA8,0x3D,0xBC,0x7A,0x3C,0x3F,0x29,0xF6,0x80,0x3E,0x21,0xC1,0x07,0x3E,0x24,0xC8,0x04,0x3E,0xE5,0xAF,0x64,0x3F,0xB8,0xAB,0x95,0x3E,0xED,0x90,0xC1,0x3E,
	0x3E,0x3C,0xBC,0x3E,0x14,0xA9,0x93,0x3B,0xCB,0x41,0x4D,0x3E,0x2C,0x51,0x81,0x3E,0x9B,0x82,0x20,0x3E,0x0A,0x83,0x57,0x3D,0x03,0xE3,0x23,0x3D,0xE4,0x8B,0xC7,0x3E,0x2F,0x34,0xFB,0x3B,0xA5,0x84,0xA4,0x3D,0x19,0x04,0x66,0x3D,0x52,0x02,0xB8,0x3D,0xC6,0x23,0x13,0x3D,0x20,0x07,0xB9,0x3C,0x42,0x07,0xDB,0x3C,0xCB,0x10,0xCB,0x3D,0x73,0x1A,0x59,0x3C,0x77,0x6D,0x2A,0x3D,0xB9,0xF0,0x38,0x3D,0xFD,0xE0,0x7C,0x3C,0xE4,0x6A,0xC8,0x3D,0xF6,0xC7,0x02,0x3E,0x57,0x32,0x29,0x3E,0x70,0x50,0x56,0x3E,0xB9,0x2F,0x36,0x3E,0xCA,0xC0,0x84,0x3E,0x07,0xC3,0x08,0x3E,0xE8,0xFA,0x45,0x3E,0x83,0x99,0x0A,0x3E,0xEE,0xCA,0x47,0x3D,0x11,0x0A,0x27,0x3E,0xDE,0xB1,0xCC,0x3C,0xDC,0x13,0xA9,0x3C,0xBC,0xB8,0x3B,0x3D,0x8F,0x1A,0x75,0x3D,0xCF,0x83,0x9B,0x3D,0xFA,0x43,0x46,0x3D,0x19,0x1F,0xB2,0x3D,0x21,0xAF,0xBA,0x3D,0x90,0x9A,0x76,0x3D,0x82,0x49,0xB5,0x3D,0x03,0x74,0xCF,0x3B,0xAB,0xA0,0xAA,0x3D,0x9B,0xFD,0xCD,0x3C,0x44,0x7A,0x2A,0x3D,0xC8,0xA0,0x47,0x3C,0xF5,0xC3,0xC1,0x3D,0xF2,0x20,0x72,0x3D,0x73,0xBA,0x58,0x3D,0x7A,0x91,0xAC,0x3D,0x70,0xDA,0x55,0x3D,0xA2,0x80,0xA1,0x3B,0x28,0xAA,0x8E,0x3C,0x66,0x21,0x99,0x3D,0x64,0xB1,0x96,0x3D,0x8C,0xA0,0x0B,0x3D,0x73,0x78,0x50,0x3F,0x69,0xBA,0x36,0x3F,0xA3,0xA1,0x22,0x3F,0xAD,0xDC,0xAC,0x3E,0xD3,0x80,0xD2,0x3C,0x4F,0x4D,0x82,0x3B,0x5F,0xEA,0xC4,0x3D,0x70,0xF9,0xA2,0x3D,
	0x12,0xAD,0x68,0x3F,0xEC,0x59,0xEF,0x3E,0xB6,0xB6,0xB5,0x3E,0xE7,0xF5,0xE6,0x3E,0xF5,0x7C,0x93,0x3D,0xBF,0x8D,0x3E,0x3F,0xB5,0x80,0xB4,0x3E,0x17,0x4C,0x6E,0x3E,0x4D,0xBC,0xE3,0x3D,0xA9,0x8C,0x28,0x3F,0x9C,0x7B,0xC8,0x3E,0x35,0x56,0x9B,0x3E,0xEB,0xDB,0x11,0x3E,0x8F,0xAF,0x0E,0x3F,0x60,0xF5,0x19,0x3F,0x28,0xFB,0xC1,0x3E,0x4D,0x36,0xB3,0x3D,0x69,0x3A,0x4F,0x3C,0xD8,0x53,0x24,0x3D,0xE6,0x8B,0x32,0x3D,0x80,0xF1,0xB2,0x3D,0x18,0x74,0xE4,0x3B,0x48,0x27,0x61,0x3C,0xCA,0x4D,0x7D,0x3D,0xEB,0xC0,0xEA,0x3C,0xCF,0x73,0x9B,0x3B,0x5E,0x0A,0x44,0x3D,0xD6,0xA3,0xA2,0x3C,0x5F,0x9A,0x45,0x3B,0xAF,0xCD,0x61,0x3D,0xC3,0x90,0xC2,0x3D,0xC4,0x18,0xC4,0x3D,0x01,0x23,0x9A,0x3D,0x8F,0xC5,0x41,0x3D,0xDD,0x30,0x5D,0x3D,0x25,0x4A,0x8B,0x3D,0xD7,0x50,0x00,0x3E,0x6F,0x11,0x65,0x3E,0xA9,0x05,0x89,0x3E,0x20,0x37,0x39,0x3E,0xFE,0x07,0x2A,0x3E,0x20,0xE5,0xA8,0x3D,0xAA,0xB4,0x95,0x3E,0xC1,0x08,0xB0,0x3E,0xA9,0xA2,0x53,0x3E,0xE8,0x9E,0x57,0x3D,0x19,0x1E,0x0E,0x3E,0xEC,0xA6,0x85,0x3E,0x56,0x3A,0x3C,0x3C,0x1D,0xE7,0xB6,0x3D,0xF8,0xB0,0xF7,0x3C,0x70,0x79,0xA3,0x3D,0xF7,0x63,0xC3,0x3C,0x9A,0x08,0x1A,0x3D,0x2F,0x8A,0x95,0x3D,0x2B,0x4A,0x91,0x3D,0xD6,0x33,0xA2,0x3B,0x95,0x8A,0x7B,0x3D,0xC1,0xC0,0xC0,0x3C,0x98,0x40,0x98,0x3B,0x54,0xE7,0x6C,0x3B,0x5F,0xE9,0x91,0x3D,0x79,0x1A,0x5F,0x3D,0xBC,0xE8,0x3B,0x3D,
	0x8E,0x82,0x74,0x3D,0x0A,0x6B,0xA3,0x3D,0x50,0xA7,0xE9,0x3C,0x66,0xE5,0x98,0x3D,0xE9,0x33,0x35,0x3A,0xF6,0x66,0x0F,0x3B,0x04,0x3C,0x50,0x3D,0xA8,0x25,0x5B,0x3D,0xEE,0x2D,0x0F,0x3F,0xF4,0x65,0x2C,0x3F,0xA2,0xA3,0x21,0x3F,0x93,0x0C,0x93,0x3E,0x95,0x80,0x94,0x3B,0x9D,0xFD,0x4F,0x3D,0x50,0x05,0x03,0x3D,0xC6,0x45,0x79,0x3D,0xDB,0xC7,0x6E,0x3F,0x81,0xAC,0xEB,0x3E,0xF0,0x68,0x70,0x3F,0xD9,0xAA,0xD8,0x3E,0xDF,0xC1,0x49,0x3F,0x9C,0x97,0x19,0x3F,0xAB,0x72,0xAB,0x3E,0xE6,0xAB,0x9E,0x3E,0x6F,0x51,0xA2,0x3D,0xBF,0xA0,0xBE,0x3C,0x9A,0x45,0x4D,0x3D,0x23,0x5A,0x89,0x3B,0x92,0x45,0x45,0x3D,0xE9,0x20,0xE9,0x3C,0x93,0x35,0x46,0x3D,0x2A,0x76,0x90,0x3D,0x30,0x04,0x7D,0x3D,0x32,0x1A,0x18,0x3D,0x7D,0x6D,0x30,0x3C,0x8D,0xCA,0x72,0x3D,0x11,0x67,0xAA,0x39,0x62,0x2D,0x95,0x3D,0x7D,0x9D,0xAF,0x3D,0xCA,0x80,0xC9,0x3C,0x0B,0x8F,0x24,0x3D,0xC6,0x8B,0x12,0x3D,0x43,0x9A,0x29,0x3D,0x5F,0xA9,0x91,0x3D,0x03,0xC4,0x4F,0x3D,0x10,0xCF,0xA9,0x3D,0xE9,0x93,0x35,0x3C,0xF4,0xB8,0x73,0x3D,0x83,0x9A,0x69,0x39,0xF4,0xE6,0x0D,0x3D,0x83,0x1A,0xE9,0x3C,0x61,0x1D,0x94,0x3C,0x1B,0x5A,0x81,0x3B,0x2B,0x5B,0xC4,0x3D,0x4B,0x67,0x64,0x3B,0xC0,0xF0,0xBF,0x3D,0xB5,0x98,0x9F,0x3D,0xAA,0x45,0x5C,0x3E,0xD2,0x25,0x7C,0x3E,0x69,0x29,0x83,0x3E,0xE8,0x57,0x81,0x3E,0x44,0x52,0x80,0x3C,0xBB,0x3F,0xAF,0x3E,0xA2,0x19,0x88,0x3E,
	0x04,0x64,0x3A,0x3D,0xA4,0xA9,0xB8,0x3D,0x71,0xBC,0x7E,0x3E,0x28,0x7B,0xD7,0x3B,0x06,0xE7,0x2E,0x3E,0x04,0x40,0x97,0x3E,0x51,0xA3,0x88,0x3D,0xFF,0xB2,0x17,0x3E,0x2A,0xB4,0xA0,0x3C,0x5B,0xE7,0x67,0x3E,0x26,0x49,0x3E,0x3E,0x79,0xB9,0x6F,0x3D,0xA7,0xD8,0xA6,0x3D,0xC1,0xFC,0xC0,0x3D,0x3D,0xB2,0x23,0x3D,0x76,0xDD,0x28,0x3D,0x9D,0x44,0x9D,0x3D,0xA8,0x40,0x28,0x3D,0xEF,0x4A,0x88,0x3D,0xC9,0x00,0x49,0x3C,0x3A,0xCF,0x53,0x3D,0x9C,0xC4,0x9B,0x3D,0xF0,0x8B,0xBC,0x3D,0xCA,0xC0,0x49,0x3D,0x71,0xA5,0xA3,0x3D,0x8A,0xC0,0x89,0x3C,0xAA,0x40,0xAA,0x3D,0x9F,0x80,0x9E,0x3D,0xBE,0x0F,0x8B,0x3D,0x51,0x7A,0xB7,0x3C,0xA6,0x58,0xA5,0x3D,0xCB,0xCD,0xFD,0x3C,0xC3,0x74,0x8C,0x3E,0x1B,0x0D,0x48,0x3C,0xF6,0x3D,0xC3,0x3E,0x10,0xAE,0xBA,0x3E,0x43,0x14,0x5D,0x3F,0x7D,0xF6,0x21,0x3F,0xC6,0x41,0x1F,0x3F,0x26,0x06,0xD1,0x3E,0x0B,0x64,0x39,0x3F,0x27,0xD7,0xB7,0x3E,0xDE,0x03,0x15,0x3D,0x5A,0x6B,0x2F,0x3E,0xE8,0xF6,0x08,0x3F,0x61,0xA0,0xE4,0x3E,0xA5,0xB7,0xCC,0x3E,0x09,0x67,0x22,0x3E,0x2D,0x73,0xC6,0x3D,0x78,0xFA,0xDD,0x3C,0x2F,0x37,0xC8,0x3C,0x44,0x87,0x5D,0x3D,0x91,0xEC,0x53,0x3F,0xBE,0xA6,0x34,0x3E,0x59,0xA6,0x25,0x3F,0x67,0x6C,0xAA,0x3D,0x0E,0x10,0x65,0x3E,0x77,0x3B,0xE9,0x3C,0xB0,0x46,0x44,0x3F,0x53,0xAF,0x91,0x3E,0x46,0xC0,0x59,0x3F,0xCE,0x6C,0x13,0x3F,0x60,0xCA,0x2A,0x3F,0x81,0xB9,0x9C,0x3E,
	0xCA,0xBF,0x96,0x3D,0x09,0x47,0xA2,0x3D,0xEC,0x83,0xB8,0x3D,0xF6,0x73,0xC2,0x3C,0x27,0x5F,0x40,0x3D,0xF2,0x13,0xBF,0x3D,0x3A,0xC2,0x1F,0x3D,0x98,0xF5,0xCA,0x3D,0x94,0xAA,0xF9,0x3C,0xC1,0x85,0x73,0x3D,0x84,0x3D,0x37,0x3D,0x42,0x0A,0xA8,0x3C,0x36,0xE9,0xB3,0x3D,0xB6,0xD5,0x8D,0x3E,0x0B,0xFD,0x21,0x3E,0xEA,0xF4,0x24,0x3D,0xD6,0x96,0x8C,0x3E,0xD8,0x76,0x8D,0x3E,0x97,0x57,0x8A,0x3E,0x2B,0xD3,0xAC,0x3B,0xA3,0xD5,0xBF,0x39,0x44,0xAC,0x11,0x3E,0xF1,0xB3,0x4D,0x3E,0xA7,0x6C,0x3D,0x3E,0xA3,0x81,0x15,0x3E,0xB5,0xE2,0x42,0x3E,0xB3,0xC1,0x97,0x3D,0x37,0x3F,0x22,0x3D,0x87,0x8C,0x86,0x3D,0x09,0x07,0x22,0x3D,0xB1,0x98,0x30,0x3D,0x71,0xF5,0x23,0x3D,0x2F,0xDA,0x14,0x3D,0x65,0x9A,0xCB,0x3B,0x04,0x37,0x9D,0x3D,0x94,0x30,0x94,0x3C,0xC2,0x70,0xC1,0x3C,0xE8,0xC0,0xE7,0x3B,0x2E,0xCC,0x7A,0x3D,0xD4,0x23,0x21,0x3D,0xB7,0x10,0xB7,0x3D,0x28,0xB2,0x8E,0x3D,0x6D,0x22,0x53,0x3D,0x9F,0x0D,0x52,0x3D,0x41,0x06,0xA7,0x3D,0x89,0xDA,0x6E,0x3C,0xD5,0x68,0x54,0x3D,0x20,0x1F,0x39,0x3D,0xB4,0x70,0x33,0x3D,0xD9,0x28,0x59,0x3D,0x89,0xCA,0x6E,0x3D,0x8D,0xF4,0x8C,0x3D,0xD2,0x78,0xEC,0x3E,0x77,0x49,0x1C,0x3F,0xC9,0x6F,0x1A,0x3F,0xCA,0x50,0xC9,0x3D,0xA3,0xE3,0x51,0x3F,0x43,0x31,0x02,0x3F,0x1F,0x5D,0x87,0x3E,0x01,0xD3,0x83,0x3E,0x47,0x5B,0x68,0x3F,0xAA,0xB0,0xD0,0x3E,0x57,0x77,0x07,0x3F,0xFF,0x4E,0x45,0x3E,
	0xB4,0x8D,0x66,0x3C,0x45,0xA7,0xDE,0x3C,0xF4,0xFA,0x8C,0x3D,0xCA,0xBC,0xC9,0x3D,0x73,0xF2,0x3C,0x3F,0xDC,0x87,0x07,0x3F,0xC2,0xEB,0x38,0x3F,0xDE,0x51,0x88,0x3E,0xEE,0x56,0x1C,0x3D,0xFA,0xF2,0x2B,0x3F,0xD7,0x96,0x3D,0x3F,0xB9,0x3C,0xFC,0x3D,0xBD,0x83,0x57,0x3F,0x59,0xEC,0xBD,0x3D,0x9C,0x01,0xF4,0x3C,0xC3,0x52,0x7B,0x3D,0x1C,0x1F,0xB5,0x3D,0x2A,0xB4,0xF6,0x3B,0xA7,0x90,0xA6,0x3C,0x1F,0x7F,0xB8,0x3D,0x90,0x9B,0x45,0x3F,0x25,0xBA,0x18,0x3F,0x7D,0x82,0xD9,0x3E,0xF3,0x02,0x50,0x3E,0x4B,0xE2,0x35,0x3F,0xB4,0xA9,0x23,0x3E,0x25,0x72,0xA9,0x3E,0xB7,0xCE,0x57,0x3D,0x75,0x60,0x31,0x3F,0xB0,0x67,0x2D,0x3F,0x19,0x06,0x17,0x3F,0xFF,0xA6,0x39,0x3E,0xE8,0x13,0x35,0x3C,0x46,0x3A,0xAC,0x3D,0xED,0x36,0x06,0x3D,0x7B,0x99,0xAD,0x3D,0xDC,0x32,0x62,0x3F,0x65,0x1E,0x29,0x3F,0xB5,0x9F,0xB2,0x3E,0x71,0x39,0x0D,0x3E,0xF4,0x34,0xD8,0x3E,0xEA,0xD4,0xA3,0x3D,0xC8,0x83,0x4B,0x3F,0x94,0x9A,0x79,0x3A,0x79,0x22,0xE4,0x3E,0x53,0x9F,0x19,0x3F,0x55,0xA1,0xE7,0x3E,0xB9,0xE0,0xB8,0x3C,0xF2,0x0E,0x8B,0x3D,0xD6,0x3F,0xA2,0x3D,0xC7,0x18,0xC7,0x3D,0x8A,0xE0,0x89,0x3C,0x5A,0x0A,0xC0,0x3C,0x9A,0x70,0x99,0x3D,0xC3,0x4D,0x76,0x3B,0x9E,0x5C,0x9D,0x3D,0x41,0x6E,0xA7,0x3D,0x4D,0x61,0x80,0x3D,0x29,0x74,0x75,0x3C,0xB0,0xFD,0x62,0x3D,0x3B,0x8E,0xA1,0x3D,0x63,0xB2,0x49,0x3D,0x65,0xAF,0x7E,0x3D,0xFC,0xF7,0xC8,0x3D,
	0xBD,0x15,0x70,0x3D,0xD9,0xDB,0xA5,0x3D,0x5C,0x8D,0x8F,0x3C,0x05,0xB7,0x9E,0x3D,0x4E,0xFA,0xB3,0x3C,0xD9,0x73,0x25,0x3C,0x82,0xB8,0x01,0x3D,0xB8,0x40,0xB8,0x3C,0xC3,0xA5,0x75,0x3D,0x63,0x8D,0x96,0x3B,0xFE,0xF0,0x7D,0x3D,0xB8,0x34,0xB8,0x3D,0x75,0x9A,0x5B,0x3B,0xED,0xB7,0xB9,0x3D,0x94,0x3D,0xC7,0x3D,0x6C,0xAA,0xD2,0x3C,0xC4,0x80,0xC3,0x3B,0xE3,0xA0,0xE2,0x3C,0xB4,0x53,0x00,0x3D,0x58,0x0D,0x8B,0x3B,0x1F,0x8F,0xB8,0x3D,0xD9,0x4B,0x25,0x3D,0x47,0xE7,0x5F,0x3B,0x41,0xEE,0xA6,0x3D,0x75,0xEA,0xDA,0x3C,0x9C,0xE0,0x1B,0x3D,0xEE,0xB3,0x3A,0x3B,0xF5,0xC2,0x8E,0x3D,0xD5,0x68,0x54,0x3D,0x0C,0x83,0xA5,0x3D,0xB5,0xD0,0xB4,0x3D,0xFD,0x53,0xC9,0x3D,0xBD,0xF3,0x89,0x3D,0x66,0x95,0x18,0x3D,0x0B,0xF3,0xA3,0x3D,0x2E,0x24,0x7A,0x3D,0xB5,0x4D,0x5D,0x3E,0x29,0x10,0xD0,0x3E,0x5B,0xF3,0x51,0x3E,0xED,0x93,0x39,0x3E,0xF8,0x95,0x19,0x3F,0xF0,0x07,0x0C,0x3F,0x3C,0xB5,0x59,0x3E,0xB6,0xB6,0xD6,0x3D,0x3C,0x5F,0x5D,0x3F,0xDA,0x3A,0x10,0x3F,0xDD,0xA9,0x61,0x3D,0x9C,0x6F,0xAC,0x3D,0xBB,0x74,0xBA,0x3D,0xF7,0xFA,0x8F,0x3D,0x83,0xA0,0x82,0x3D,0x93,0xB5,0xC5,0x3D,0x14,0x35,0x13,0x3F,0xE6,0xEE,0xE3,0x3E,0x24,0xDB,0x07,0x3F,0xA0,0xDB,0x19,0x3E,0x50,0x85,0x6A,0x3F,0x39,0xEA,0x0D,0x3F,0x49,0x63,0xE9,0x3E,0x44,0xFA,0x29,0x3E,0x5B,0xE5,0x5A,0x3F,0xEC,0x69,0xF2,0x3E,0x2E,0xF0,0x10,0x3F,0x5F,0x7A,0x45,0x3D
},

/* input data and other run-time fields*/

{0}

}; /* done */