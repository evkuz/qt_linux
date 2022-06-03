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
{{0.000861f,-0.070568f},{0.001063f,-0.232731f}},

/* Output variable normalization parameters */
{{0.020000f,-0.100000f},{0.025000f,-0.250000f},{0.025641f,-2.846154f},{0.011111f,-1.000000f}},

/* number of input MF's per variable*/ 
{14,14},
/* input MF paramteres */ 
{
	{triangular,0.099613f,0,0.198601f,0.198601f},{triangular,0.256454f,0,0.114099f,0.114099f},{triangular,0.354346f,0,0.081070f,0.081070f},{triangular,0.438683f,0,0.087076f,0.087076f},{triangular,0.489028f,0,0.013297f,0.013297f},{triangular,0.521730f,0,0.051902f,0.051902f},{triangular,0.563253f,0,0.030884f,0.030884f},{triangular,0.607788f,0,0.057907f,0.057907f},{triangular,0.662005f,0,0.050186f,0.050186f},{triangular,0.712780f,0,0.051044f,0.051044f},{triangular,0.766566f,0,0.056192f,0.056191f},{triangular,0.832616f,0,0.075494f,0.075494f},{triangular,0.895224f,0,0.049328f,0.049328f},{triangular,0.959983f,0,0.079783f,0.079784f},
	{descending,0.025505f,0,0,0.173666f},{triangular,0.203640f,0,0.179192f,0.082886f},{triangular,0.322795f,0,0.153142f,0.098411f},{triangular,0.404623f,0,0.063678f,0.095253f},{triangular,0.514745f,0,0.122882f,0.101305f},{triangular,0.573326f,0,0.014735f,0.019998f},{triangular,0.592455f,0,0.017893f,0.023155f},{triangular,0.620351f,0,0.032102f,0.054205f},{triangular,0.677471f,0,0.058941f,0.049469f},{triangular,0.725027f,0,0.044732f,0.062625f},{triangular,0.784803f,0,0.055784f,0.025261f},{triangular,0.841126f,0,0.086307f,0.048416f},{triangular,0.905845f,0,0.079781f,0.090727f},{ascending,0.975912f,0,0.048065f,0}
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
196, /*number of rules */

/* index */
{0}, /* empty */
/* database */
{
	0xAC,0x5C,0x2C,0x3F,0xE6,0x40,0x66,0x3D,0x8E,0xEA,0x0D,0x3F,0xAF,0x76,0xAF,0x3E,0xF2,0xE8,0x71,0x3E,0xD4,0xA5,0x53,0x3F,0x88,0x7C,0x88,0x3E,0x95,0x00,0x15,0x3E,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x6E,0xCF,0x46,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x0B,0x72,0x2A,0x3F,0xCD,0xFF,0x7F,0x3F,0xCD,0xFF,0x7F,0x3F,0xCD,0xFF,0x7F,0x3F,0xED,0xA8,0x46,0x3F,0xCC,0xC7,0x4B,0x3F,0xFC,0x92,0x7B,0x3F,0xA2,0xA5,0x21,0x3F,0xAF,0xE8,0x2E,0x3F,0xFE,0x81,0x7D,0x3F,0xAA,0x9B,0x29,0x3F,0xE5,0xCC,0x64,0x3F,0xFC,0xDD,0x7B,0x3F,0xCD,0xFF,0x7F,0x3F,0xCD,0xFF,0x7F,0x3F,0x00,0x00,0x80,0x3F,0x24,0x55,0x55,0x3F,0x00,0x00,0x80,0x3F,0x1E,0xEE,0x7F,0x3F,0xFD,0xFA,0x7F,0x3F,0x34,0x57,0x79,0x3F,0x29,0xF2,0x7F,0x3F,0x00,0x00,0x80,0x3F,0x48,0xE9,0x7F,0x3F,0xFE,0x5B,0x54,0x3F,0xFE,0xFF,0x7F,0x3F,0x00,0x00,0x80,0x3F,0xF9,0xFF,0x7F,0x3F,0xEC,0xDA,0x75,0x3F,0x42,0xF7,0x7F,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x76,0xB2,0x7E,0x3F,0xFF,0xAC,0x3F,0x3F,0x00,0x00,0x80,0x3F,0x62,0x65,0x7F,0x3F,0xB7,0xFE,0x7F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3F,0xFF,0x2F,0x07,0x3F,0x00,0x00,0x80,0x3F,0xBA,0x68,0xBA,0x3D,0xCF,0xC0,0x4E,0x3E,0xA1,0x80,0x20,0x3D,0x8D,0x36,0x8D,0x3E,0x1E,0x5D,0x71,0x3F,0x04,0xC5,0x77,0x3F,0x00,0x00,0x80,0x3F,0xD4,0x4D,0x15,0x3F,
	0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0xD1,0x24,0x0E,0x3F,0x8D,0x0E,0x7D,0x3F,0x15,0xBB,0x7E,0x3F,0xE2,0xB7,0x7D,0x3F,0xAF,0x8F,0x22,0x3F,0x55,0x20,0xB8,0x3E,0x7D,0xC0,0x27,0x3F,0xFE,0xDB,0x22,0x3F,0xAA,0xF1,0x38,0x3F,0xC9,0x05,0x49,0x3F,0xE0,0xFF,0x5F,0x3F,0x95,0x2C,0x15,0x3E,0x9D,0x7B,0x1D,0x3F,0x63,0x93,0xA9,0x3E,0xF4,0x00,0x74,0x3F,0xF1,0x0E,0x28,0x3F,0x15,0xAC,0x2D,0x3F,0xD4,0x05,0xA8,0x3E,0xC3,0x3D,0x61,0x3F,0xED,0xDF,0x35,0x3F,0x90,0x1F,0x53,0x3F,0x32,0x2D,0x9D,0x3E,0x5D,0xBF,0x68,0x3F,0xD8,0x93,0x28,0x3F,0x7F,0xD9,0x3C,0x3F,0xFD,0x27,0x5C,0x3E,0x26,0xC6,0x41,0x3F,0xEB,0x94,0x03,0x3F,0x8A,0x2D,0x52,0x3F,0x7F,0xA8,0x71,0x3E,0x1F,0xEA,0x25,0x3F,0xDE,0x09,0xE0,0x3E,0x73,0xD1,0x5B,0x3F,0xD2,0xBE,0xF9,0x36,0x64,0xBF,0x18,0x3F,0x44,0xBC,0xCF,0x3E,0x91,0xB0,0x53,0x3F,0x42,0xE3,0x70,0x3E,0x4D,0xB8,0x2D,0x3D,0xF0,0xAD,0xF1,0x3D,0x5D,0x5F,0x7E,0x3F,0xAE,0x2F,0x2A,0x3F,0x1B,0x54,0xBC,0x38,0x59,0x34,0x19,0x3D,0x00,0x00,0x80,0x3F,0x7F,0x84,0x7E,0x3F,0x94,0x44,0x7E,0x3F,0x00,0x00,0x80,0x3F,0x45,0xC5,0xCC,0x3E,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x77,0x96,0x12,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x33,0xA4,0x09,0x3F,0xEE,0x6A,0xD4,0x3E,0x00,0x00,0x80,0x3F,0x30,0x2E,0x3A,0x3F,0xFC,0xC9,0x1C,0x3F,
	0x35,0xED,0xCF,0x3E,0xEF,0x3A,0x58,0x3F,0xD0,0xC5,0x1D,0x3F,0x26,0x51,0x27,0x3F,0xD3,0xB6,0xD2,0x3E,0xEC,0xAC,0x6B,0x3E,0x9A,0x84,0x19,0x3E,0xAF,0xC2,0x2E,0x3F,0x17,0x37,0xF8,0x3E,0x74,0xFD,0x61,0x3F,0xAB,0xD0,0x36,0x3F,0xEB,0x32,0x3B,0x3F,0x1F,0x3F,0x19,0x3F,0x83,0xCE,0x9A,0x3A,0x34,0xE4,0x68,0x3D,0xBB,0x1A,0x3E,0x3F,0x41,0xEE,0x19,0x3F,0xC8,0xDB,0x62,0x34,0x2C,0x5F,0xA4,0x3D,0x4A,0x20,0x62,0x3F,0x4E,0x23,0x04,0x3F,0x00,0x00,0x00,0x00,0xA4,0xBD,0xB8,0x3D,0x4F,0x47,0x5B,0x3F,0x6C,0x63,0x04,0x3F,0x00,0x00,0x00,0x00,0x32,0xFE,0xDA,0x3C,0x47,0x0C,0x49,0x3F,0x4A,0x1C,0x3E,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xB9,0x90,0x6D,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x00,0x00,0x53,0xB7,0x4B,0x35,0xEA,0x2A,0x51,0x3F,0x41,0x5E,0x9C,0x3D,0x00,0x00,0x00,0x00,0x84,0x0C,0xAC,0x3A,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0xFE,0x62,0xF0,0x3E,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x68,0xFE,0xDB,0x3E,0xF9,0xFE,0x5A,0x3F,0xB4,0x7D,0x51,0x3F,0xAC,0x1A,0x58,0x3F,0x2F,0xDF,0xD9,0x3E,0x21,0x52,0xD1,0x3E,0x00,0x00,0x80,0x3F,0xAC,0x0A,0x4C,0x3F,0x6C,0xA2,0x15,0x3F,0xD6,0x3E,0xC5,0x3E,0x97,0x63,0xAC,0x3E,0xD5,0x83,0xC4,0x3E,0x65,0x07,0x03,0x3F,0x03,0x7F,0x16,0x3F,0x00,0x00,0x00,0x00,0xF2,0x82,0x1C,0x3E,0xAB,0x33,0x1D,0x3F,
	0x71,0x83,0x06,0x3F,0x00,0x00,0x00,0x00,0x22,0xD6,0x6B,0x3C,0x77,0x44,0x23,0x3F,0x57,0x05,0xDD,0x3E,0x00,0x00,0x00,0x00,0xC6,0x99,0x0C,0x3E,0x05,0x90,0x1C,0x3F,0x16,0xED,0xA1,0x3E,0x00,0x00,0x00,0x00,0x10,0xCF,0x12,0x3E,0x9E,0x2C,0xDE,0x3E,0x38,0x99,0xE3,0x3E,0x00,0x00,0x00,0x00,0x54,0xF1,0x51,0x3D,0xD3,0xE2,0xA3,0x3E,0x00,0x00,0x00,0x00,0x9C,0x1D,0x4D,0x3D,0x00,0x00,0x00,0x00,0xB6,0x82,0xDE,0x3E,0xCE,0xB0,0x6D,0x3C,0xF8,0x4A,0xA0,0x3D,0x00,0x00,0x00,0x00,0xE8,0xDD,0x37,0x3F,0x00,0x00,0x00,0x00,0xFD,0x7C,0x16,0x3E,0x00,0x00,0x00,0x00,0xA3,0x81,0x63,0x3F,0xEB,0x41,0xDD,0x2D,0x38,0x89,0x2D,0x3E,0x00,0x00,0x00,0x00,0x4F,0xFB,0x7A,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0xBC,0x83,0xE8,0x3E,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x62,0xA7,0x7D,0x3F,0x12,0x71,0x47,0x3E,0x63,0x9B,0xBA,0x3E,0x30,0xCB,0x47,0x3F,0x0D,0x8F,0x80,0x3D,0x19,0xEB,0x8A,0x3E,0xA3,0x13,0x77,0x3E,0x04,0x04,0x5C,0x3F,0x87,0xA2,0x23,0x3F,0x0A,0xCA,0x04,0x3F,0xCE,0x7F,0x0F,0x3F,0xBE,0x55,0x06,0x34,0x00,0x00,0x00,0x00,0x66,0x79,0x1A,0x3F,0xF6,0xE6,0x0F,0x3B,0xB4,0x33,0x80,0x3C,0xF8,0x5F,0xC4,0x3D,0x66,0x31,0x99,0x3D,0x64,0x7D,0x97,0x3C,0x40,0x0F,0x59,0x3D,0x8C,0xF0,0x8B,0x3C,0x2C,0x72,0x92,0x3D,0x23,0x0A,0x89,0x3D,0xF6,0x8A,0x8F,0x3D,0xF6,0xD3,0x42,0x3C,0xBD,0x7B,0x89,0x3D,
	0x00,0x74,0xCC,0x3D,0x6D,0x12,0x53,0x3D,0xE9,0xE6,0x82,0x3D,0xE6,0x0B,0xB3,0x3D,0xE3,0xC3,0xAF,0x3C,0x90,0xA1,0xC2,0x3D,0xF5,0x33,0xC1,0x3A,0x8A,0x2D,0x3D,0x3D,0x2C,0x66,0xA5,0x3B,0x31,0x66,0x56,0x3E,0x73,0x7E,0xEB,0x3C,0x8F,0x9C,0x48,0x3F,0x00,0x00,0x00,0x00,0x0F,0x2D,0x23,0x3E,0x00,0x00,0x00,0x00,0x6A,0xC1,0x4C,0x3F,0x00,0x00,0x00,0x00,0xB6,0x64,0x58,0x3E,0x00,0x00,0x00,0x00,0x6E,0x64,0x48,0x3F,0x48,0x1A,0xAE,0x3B,0xC7,0x87,0x93,0x3D,0x01,0x67,0x1A,0x3D,0x55,0xFA,0x3A,0x3C,0xB6,0xBB,0x7E,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0xCD,0x72,0xAC,0x3E,0x00,0x00,0x80,0x3F,0x49,0x26,0x6C,0x3F,0x00,0x00,0x80,0x3F,0xFB,0x43,0xE7,0x3E,0xF7,0xD8,0xCD,0x3E,0xC0,0xD2,0x4B,0x3F,0x64,0x50,0x1E,0x3F,0x28,0x89,0xE4,0x3E,0x0A,0x6B,0x14,0x3F,0x5A,0x70,0x06,0x3F,0x8E,0x6B,0x30,0x3F,0xEA,0xC8,0x01,0x3F,0x87,0x2B,0x03,0x3F,0x36,0xF6,0x5D,0x33,0x73,0x7F,0x87,0x3D,0x09,0x59,0xFF,0x3E,0x99,0x28,0x19,0x3D,0x1E,0x94,0xEA,0x3C,0xEA,0x8E,0x83,0x3D,0x98,0x20,0x18,0x3D,0xCA,0x63,0x16,0x3D,0xBC,0x84,0xBB,0x3D,0xEB,0x70,0xEA,0x3C,0x22,0xF6,0x87,0x3D,0x90,0xCD,0xC2,0x3A,0xE8,0xB3,0x34,0x3D,0x2B,0x47,0x44,0x3D,0x88,0xAC,0x87,0x3D,0x99,0xB0,0x98,0x3C,0x74,0x4D,0x27,0x3B,0x9C,0x74,0x9B,0x3D,0x68,0x9A,0x4E,0x3B,0x4D,0x78,0x89,0x3C,0xB8,0xF5,0x18,0x3E,0x4D,0xE8,0x8A,0x3C,0x1C,0x17,0x11,0x3E,
	0x00,0x00,0x00,0x00,0xD9,0x8D,0x06,0x3E,0x00,0x00,0x00,0x00,0x32,0xB7,0x35,0x3F,0x00,0x00,0x00,0x00,0x8B,0xFB,0x59,0x3E,0x00,0x00,0x00,0x00,0xFB,0x5B,0x4B,0x3F,0x00,0x00,0x00,0x00,0x34,0x24,0x6C,0x3D,0x94,0x75,0x61,0x3B,0x55,0x07,0x3A,0x3F,0xA5,0x00,0xA5,0x3C,0x48,0x2F,0x61,0x3D,0xA4,0x6C,0xA3,0x3D,0x93,0xDA,0xF8,0x3C,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x2B,0x6F,0xCE,0x3E,0xFB,0x54,0x6B,0x3F,0x02,0x4C,0x6F,0x3F,0x0A,0xE8,0x5F,0x3F,0xB5,0x87,0x95,0x3C,0x73,0xB2,0xB7,0x3E,0xED,0xC9,0x60,0x3F,0x47,0x3D,0x0D,0x3F,0x30,0xF6,0xCF,0x3E,0xAF,0x7D,0x89,0x3E,0x00,0x00,0x00,0x00,0x00,0xDF,0x53,0x3D,0x97,0x97,0x9C,0x3E,0x38,0x6D,0x21,0x3F,0x00,0x00,0x00,0x00,0xDC,0x94,0x9F,0x3D,0x5D,0xFE,0xFE,0x3E,0x1A,0x4C,0x66,0x3D,0xDF,0xA0,0x5E,0x3C,0xE7,0xD0,0x66,0x3D,0xF9,0x27,0xC5,0x3D,0x85,0x35,0xB8,0x3D,0xE3,0xB0,0xE2,0x3C,0x5C,0x29,0x8F,0x3D,0x4F,0x9A,0xB5,0x3B,0x8E,0x95,0x40,0x3D,0xB8,0xCD,0xEA,0x3B,0x82,0x9C,0x81,0x3D,0x6B,0x6D,0x1E,0x3C,0x9D,0xB0,0x54,0x3B,0x12,0x96,0xA8,0x3D,0x37,0x24,0x10,0x3C,0xFA,0xF8,0xA6,0x3E,0x00,0x00,0x00,0x00,0x01,0x7A,0x01,0x3E,0x00,0x00,0x00,0x00,0x30,0xE4,0x36,0x3F,0x70,0xEB,0x2F,0x3D,0xA1,0xCF,0x6B,0x3E,0xE2,0xF8,0xD2,0x3B,0x22,0xA2,0x13,0x3F,0x50,0x3A,0xB6,0x3C,0x00,0x74,0xCC,0x3B,0x24,0x5B,0xBD,0x3D,0x94,0xD2,0x79,0x3D,
	0xA8,0x34,0xA8,0x3D,0x40,0x6A,0x26,0x3D,0xA7,0x00,0xA7,0x3A,0xAC,0xC0,0xAB,0x3B,0xE3,0x8B,0xAF,0x3D,0xC4,0x53,0x90,0x3D,0x5F,0x69,0x92,0x3D,0xBA,0x30,0xBA,0x3C,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0xD1,0xA4,0x82,0x3E,0x03,0x4B,0x7F,0x3F,0xF1,0x0A,0x35,0x3F,0x8E,0x1D,0x30,0x3F,0x0B,0x36,0x9C,0x3E,0xDB,0x66,0xF0,0x3E,0x1F,0x07,0x2A,0x3F,0x31,0xED,0x38,0x3F,0x76,0x6D,0xB7,0x3E,0xD4,0xFC,0x07,0x3F,0x00,0x00,0x00,0x00,0x90,0x10,0x6B,0x3D,0xDB,0xD7,0x9B,0x3E,0xE1,0xC4,0xD6,0x3E,0x00,0x00,0x00,0x00,0x07,0xDB,0x13,0x3E,0xCA,0x6E,0x08,0x3F,0x53,0x1D,0x06,0x3D,0x35,0x3F,0x4E,0x3D,0xBE,0xD5,0x70,0x3D,0x7D,0x82,0x63,0x3D,0xB5,0x98,0xB4,0x3D,0x1B,0xE7,0x34,0x3B,0x42,0x62,0xA8,0x3D,0x20,0x9A,0x86,0x3B,0x00,0x00,0x00,0x00,0x36,0x36,0xFC,0x3C,0xC5,0x5D,0xD2,0x3B,0x98,0x6B,0xBD,0x3E,0x00,0x00,0x00,0x00,0x62,0x32,0x1F,0x3E,0x00,0x00,0x00,0x00,0xD6,0xA5,0x07,0x3F,0x00,0x00,0x00,0x00,0xFE,0x08,0x58,0x3E,0x00,0x00,0x00,0x00,0x97,0x36,0x30,0x3F,0x9E,0x44,0x9A,0x3D,0xCC,0x9A,0x51,0x3E,0x55,0x99,0x65,0x3B,0x0B,0xBA,0x70,0x3E,0x20,0x2A,0x86,0x3D,0x11,0x87,0x2A,0x3D,0x32,0xEE,0x97,0x3D,0xCC,0x8F,0x98,0x3D,0xFB,0x33,0xC7,0x39,0x24,0xA7,0x3D,0x3D,0x54,0xAA,0xBA,0x3C,0xBB,0x9D,0x6D,0x3D,0xB1,0xED,0xE3,0x3C,0x2A,0xFA,0x0F,0x3D,0x6B,0xBA,0x50,0x3C,0xF5,0x23,0x41,0x3D,
	0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0xF5,0xA0,0x92,0x3E,0xEA,0x82,0xD8,0x3E,0x00,0x00,0x80,0x3F,0x3B,0x45,0x53,0x3F,0x7E,0xB8,0xC8,0x3E,0xDD,0x51,0x85,0x3E,0x27,0xD9,0x71,0x3F,0x3A,0xC0,0xF2,0x3E,0xED,0xAE,0x9B,0x3E,0xFF,0x2D,0x0E,0x3F,0x00,0x00,0x00,0x00,0xBF,0x48,0x10,0x3E,0x1F,0xA9,0xBA,0x3E,0x25,0xEF,0x02,0x3F,0x5B,0x62,0xB4,0x3C,0xFE,0xA6,0xEB,0x3D,0xEB,0x62,0xBF,0x3E,0xBB,0x83,0x87,0x3C,0x0A,0xA7,0x23,0x3D,0x52,0x65,0x05,0x3D,0xDE,0x33,0x2A,0x3D,0x00,0x00,0x00,0x00,0x84,0xE2,0x80,0x3E,0x00,0x00,0x00,0x00,0x0B,0xB7,0xB2,0x3E,0x00,0x00,0x00,0x00,0x63,0x0E,0x26,0x3E,0x00,0x00,0x00,0x00,0x2B,0x4A,0xE8,0x3E,0x0A,0x74,0x24,0x3C,0x05,0x0B,0x5A,0x3E,0x00,0x00,0x00,0x00,0xB5,0x7A,0xEA,0x3E,0x34,0x9A,0x9A,0x3C,0x00,0xAC,0xCC,0x3D,0xC7,0xAD,0xF9,0x3C,0x7B,0x05,0x2E,0x3D,0xC0,0x1C,0xC0,0x3D,0x97,0x8D,0xC9,0x3B,0xA6,0x28,0xA6,0x3D,0x1B,0xA3,0xB4,0x3D,0xDD,0x40,0xDD,0x3B,0x6F,0x5D,0xA2,0x3C,0xC8,0xCF,0x94,0x3D,0x49,0xDA,0x2E,0x3D,0x6F,0x3D,0xA2,0x3C,0x4A,0x3A,0xB0,0x3D,0xBF,0xF3,0x0B,0x3C,0xED,0x73,0xB9,0x3C,0xB6,0x54,0xB5,0x3D,0xFE,0xBE,0x97,0x3D,0xA7,0x7D,0x59,0x3D,0x8B,0x15,0xBE,0x3D,0xC6,0xFF,0x7F,0x3F,0xC6,0xFF,0x7F,0x3F,0xC5,0xFF,0x7F,0x3F,0x21,0xE9,0x5D,0x3E,0xFC,0xAC,0xD9,0x3E,0x00,0x00,0x80,0x3F,0x47,0xB7,0x42,0x3F,0xBB,0xDC,0x45,0x3E,
	0xA1,0x85,0xE7,0x3E,0xAB,0x17,0x64,0x3B,0x37,0xCE,0x63,0x3E,0x6C,0x86,0x71,0x3E,0x68,0x0D,0x0E,0x3F,0x00,0x00,0x00,0x00,0x37,0x31,0x89,0x35,0xE3,0x15,0xA9,0x3E,0x37,0xD3,0xBA,0x34,0x00,0x00,0x00,0x00,0x04,0x15,0xAC,0x3D,0x2B,0xAF,0x5A,0x3D,0xBC,0xAB,0x21,0x3D,0x7E,0x4E,0x69,0x3E,0x5C,0xCE,0x32,0x3D,0x15,0x05,0x89,0x3E,0x00,0x00,0x00,0x00,0x17,0xE3,0x4E,0x3E,0x00,0x00,0x00,0x00,0x3D,0xDB,0xC9,0x3E,0x00,0x00,0x00,0x00,0x72,0x81,0xC6,0x3D,0x00,0x00,0x00,0x00,0x87,0xF1,0xB6,0x3E,0xF7,0x03,0xC4,0x3C,0xB8,0x93,0x84,0x3C,0xA0,0xE0,0x1F,0x3C,0xEA,0xDE,0x83,0x3D,0x12,0x4F,0x2B,0x3D,0xB9,0xEB,0x05,0x3D,0x07,0xC4,0xD3,0x3C,0xE5,0x08,0x65,0x3D,0x4F,0xFF,0x67,0x3D,0xD7,0x10,0xD7,0x3C,0x33,0x3F,0xCC,0x3D,0x71,0x99,0xA3,0x3D,0x52,0x2D,0x85,0x3C,0x43,0x2A,0x29,0x3D,0x32,0x1A,0x98,0x3B,0x0E,0x63,0xA7,0x3D,0xF9,0xD3,0x45,0x3D,0x97,0x6D,0x4A,0x3D,0x44,0x5A,0xAA,0x3C,0x12,0xB4,0x5E,0x3C,0x05,0xFF,0x9D,0x3D,0x30,0xEA,0x95,0x3C,0xD2,0xC8,0x51,0x3D,0xE4,0x63,0xB0,0x3D,0x88,0xFF,0x7F,0x3F,0x88,0xFF,0x7F,0x3F,0x00,0x00,0x80,0x3F,0xFB,0xCC,0x58,0x3E,0xF4,0xF1,0xF1,0x3E,0x00,0x00,0x80,0x3F,0xD0,0x12,0x60,0x3F,0x7D,0x9C,0x7B,0x3E,0x40,0x46,0x89,0x3E,0xA9,0x36,0xE4,0x3E,0xC8,0xE3,0x9E,0x3E,0x73,0x31,0x4E,0x3E,0x93,0x74,0x18,0x3F,0x00,0x00,0x00,0x00,0x7E,0x1B,0x28,0x3D,0x29,0x5C,0x7F,0x3E,
	0xE2,0x28,0x7B,0x2F,0x01,0x83,0xFE,0x3D,0xE8,0xAB,0xA8,0x32,0xDB,0x01,0xD6,0x3E,0x00,0x00,0x00,0x00,0x2F,0x66,0x50,0x3E,0x00,0x00,0x00,0x00,0x80,0x82,0x51,0x3D,0x00,0x00,0x00,0x00,0xDE,0x8D,0x03,0x3E,0x00,0x00,0x00,0x00,0x19,0x87,0xBB,0x3E,0x58,0x17,0xA0,0x3D,0x74,0xC6,0x71,0x3E,0x00,0x00,0x00,0x00,0xE5,0xDC,0xB7,0x3D,0xF8,0x27,0xC4,0x3D,0x1E,0x32,0x04,0x3D,0x4D,0x05,0x00,0x3D,0xEC,0x68,0x6B,0x3D,0x88,0x11,0xBB,0x3D,0xBD,0x33,0x89,0x3D,0x23,0xEA,0x08,0x3D,0x24,0x67,0xBD,0x3B,0x09,0xE7,0x22,0x3C,0x0E,0x4C,0x5A,0x3D,0xBE,0xF0,0xBD,0x3C,0xF4,0xD0,0x73,0x3D,0x33,0x22,0x99,0x3D,0x24,0x97,0xBD,0x3D,0xFB,0x1F,0xC7,0x3D,0xC0,0xC0,0x3F,0x3D,0x33,0xD4,0x7F,0x3D,0xA7,0xA8,0x26,0x3D,0xF8,0x88,0x77,0x3D,0xE7,0x4E,0x00,0x3D,0x55,0xDD,0x87,0x3C,0x14,0xF7,0xAC,0x3C,0xC9,0x57,0x95,0x3D,0x90,0x3C,0x90,0x3D,0xFC,0xFF,0x7F,0x3F,0xD0,0xFF,0x7F,0x3F,0xFF,0xFF,0x7F,0x3F,0xB4,0x9D,0xA5,0x3D,0x6D,0xF1,0xAF,0x3E,0x2F,0xFC,0x7F,0x3F,0x8B,0x8B,0x3C,0x3F,0x50,0x2B,0x16,0x3E,0xD8,0x62,0x0B,0x3F,0x67,0xC1,0x25,0x3C,0x27,0xFB,0x4E,0x3E,0x89,0x4D,0x24,0x3E,0xDD,0x0A,0xE6,0x3E,0x00,0x00,0x00,0x00,0x5D,0x08,0xEF,0x35,0x46,0xA8,0x29,0x3E,0xE5,0xFF,0x7B,0x2E,0xEA,0xCA,0x82,0x3D,0x4D,0x3C,0xA9,0x31,0x40,0xD0,0x7D,0x3C,0x00,0x00,0x00,0x00,0x50,0x83,0x5B,0x3E,0x00,0x00,0x00,0x00,0xC5,0xA4,0x98,0x3E,
	0x19,0x1D,0x74,0x3B,0xDA,0x84,0x7F,0x3D,0x2C,0x58,0x81,0x3C,0x57,0xF3,0x9D,0x3E,0x26,0x3F,0x3F,0x3D,0x5A,0x47,0x73,0x3C,0x36,0xA2,0x1C,0x3D,0x94,0x70,0x93,0x3C,0x0F,0x73,0xA8,0x3D,0x27,0x43,0xC0,0x3D,0x8B,0xAC,0x8A,0x3D,0x5E,0x52,0x44,0x3D,0xBF,0x65,0x72,0x3D,0x10,0xC7,0x29,0x3D,0x79,0xE1,0xAB,0x3D,0x30,0x3A,0x16,0x3C,0x54,0x15,0x07,0x3D,0xE4,0x9B,0x30,0x3D,0x6E,0x95,0xA0,0x3D,0x78,0x5D,0xAB,0x3D,0xFC,0x98,0x7B,0x3D,0x18,0x14,0x64,0x3D,0x62,0x8E,0xC8,0x3D,0xA6,0x90,0xA5,0x3D,0x5A,0x7F,0x73,0x3D,0x8C,0x38,0x0C,0x3D,0xA4,0xA8,0xA3,0x3D,0xE8,0xB7,0xB4,0x3D,0x2F,0xC7,0xC8,0x3C,0x52,0x8D,0x05,0x3C,0xA7,0xAD,0xD9,0x3C,0x2A,0x34,0x76,0x3C,0x91,0xF0,0x7F,0x3F,0xB8,0x92,0x7F,0x3F,0xE4,0xFB,0x7F,0x3F,0xAB,0x86,0x20,0x3E,0xB1,0x39,0xB1,0x3E,0x88,0x4A,0x60,0x3F,0x08,0x2F,0x1F,0x3F,0x6A,0xE6,0xCA,0x3D,0x8A,0x36,0x1B,0x3F,0x45,0xEE,0xB6,0x37,0x85,0x81,0x70,0x3D,0xBD,0x8E,0xA7,0x3D,0x90,0x6B,0x9B,0x3E,0x00,0x00,0x00,0x00,0x45,0xC9,0xCF,0x3D,0xBE,0xEC,0xD0,0x3D,0x00,0x00,0x00,0x00,0x9F,0x1C,0x08,0x3E,0x9D,0xA0,0x66,0x35,0xA1,0x98,0x05,0x3D,0x5C,0x6F,0xF3,0x38,0x75,0x95,0x2A,0x3E,0x00,0x00,0x00,0x00,0xE9,0xA0,0x26,0x3E,0x35,0x2A,0x1B,0x3D,0x4E,0x25,0x81,0x3D,0x0A,0x34,0xD6,0x3C,0x4C,0x87,0x65,0x3D,0x57,0x1D,0x0A,0x3D,0x0C,0xCB,0xA5,0x3D,0x03,0xC7,0x1C,0x3D,0x4C,0x07,0xE5,0x3C,
	0x1F,0xB4,0x6B,0x3D,0x6D,0xDA,0xD2,0x3B,0x6A,0x0D,0x9D,0x3B,0xC9,0xB3,0x15,0x3B,0x9B,0xCC,0x9A,0x3D,0x5E,0x3D,0x91,0x3D,0x67,0x9A,0x4D,0x3C,0x9F,0x10,0x9F,0x3C,0xF4,0x93,0xC0,0x3D,0xF1,0x00,0x71,0x3C,0xBE,0xFF,0x8A,0x3D,0xCD,0x03,0x9A,0x3D,0x66,0xC5,0x18,0x3D,0xEC,0xA0,0xEB,0x3C,0x8B,0x9A,0xF1,0x39,0xDA,0x6F,0xA6,0x3D,0x7C,0xEA,0x61,0x3D,0x42,0x1A,0x28,0x3B,0xF7,0x03,0xC4,0x3D,0x96,0xB8,0x95,0x3D,0x22,0x32,0x08,0x3D,0xEF,0x26,0x08,0x3C,0x8B,0x08,0x8B,0x3D,0x3A,0x87,0x53,0x3D,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0xAA,0xEC,0x66,0x3C,0xEC,0x78,0xC0,0x3E,0x00,0x00,0x80,0x3F,0xB1,0x0E,0x4A,0x3F,0xB1,0x9F,0x9A,0x3C,0xD3,0xEE,0xEC,0x3E,0xA9,0x72,0xA5,0x3D,0xD7,0xB2,0x52,0x3E,0xA6,0x8D,0xD0,0x3B,0xA2,0x87,0x09,0x3F,0x00,0x00,0x00,0x00,0x36,0xC4,0x5D,0x3B,0xC3,0x31,0xBB,0x3A,0x00,0x00,0x00,0x00,0xDF,0x69,0xD1,0x3D,0x00,0x00,0x00,0x00,0xF7,0x9F,0x12,0x3C,0x00,0x00,0x00,0x00,0xAD,0xCA,0x5F,0x3E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0x88,0x5E,0x38,0xB7,0x6A,0x94,0x3D,0x79,0x4E,0xF8,0x39,0x00,0x00,0x00,0x00,0xBE,0x9D,0x70,0x3D,0xDD,0x53,0xA9,0x3D,0x39,0x2E,0x9F,0x3D,0xFF,0xC7,0xCB,0x3D,0x3F,0x2E,0xA5,0x3D,0x50,0xDA,0xB5,0x3C,0x2E,0xA7,0x47,0x3C,0x0A,0x7F,0xA3,0x3D,0x4F,0x4A,0x35,0x3D,0xF1,0x13,0xBE,0x3D,0x6B,0x11,0x9E,0x3D,0x66,0x09,0x99,0x3D,
	0x2A,0xB2,0x90,0x3D,0x00,0xFC,0x4C,0x3D,0xEE,0x00,0xEE,0x3C,0x84,0xFA,0xE9,0x3C,0x54,0xF1,0x86,0x3D,0xDB,0x13,0x28,0x3D,0x2B,0x33,0xC4,0x3D,0xFE,0xC6,0x17,0x3D,0x83,0x0A,0xE9,0x3C,0x8E,0xFA,0x73,0x3C,0x60,0xAD,0x92,0x3D,0x5E,0x66,0xC4,0x3D,0xC1,0x6D,0xF4,0x3C,0x2F,0x42,0x95,0x3D,0x02,0xB4,0x4E,0x3D,0x4E,0x29,0x81,0x3D
},

/* input data and other run-time fields*/

{0}

}; /* done */