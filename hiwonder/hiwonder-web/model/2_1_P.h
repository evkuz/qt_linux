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
	0xAC,0x5C,0x2C,0x3F,0xE6,0x40,0x66,0x3D,0x8E,0xEA,0x0D,0x3F,0xAF,0x76,0xAF,0x3E,0xF2,0xE8,0x71,0x3E,0xD4,0xA5,0x53,0x3F,0x88,0x7C,0x88,0x3E,0x95,0x00,0x15,0x3E,0x5E,0xF4,0x77,0x3F,0xA6,0x6E,0x73,0x3F,0x78,0xAF,0x78,0x3F,0x81,0xF7,0x3E,0x3F,0xB3,0x94,0x7F,0x3F,0x96,0x00,0x7C,0x3F,0x4C,0x83,0x7F,0x3F,0xE3,0x55,0x2F,0x3F,0xCA,0xC6,0x7D,0x3F,0x5C,0xF9,0x75,0x3F,0x4A,0xC8,0x7D,0x3F,0x3C,0xE3,0x46,0x3F,0xCC,0xC7,0x4B,0x3F,0xFC,0x92,0x7B,0x3F,0xA2,0xA5,0x21,0x3F,0xAF,0xE8,0x2E,0x3F,0xFE,0x81,0x7D,0x3F,0xAA,0x9B,0x29,0x3F,0xE5,0xCC,0x64,0x3F,0xFC,0xDD,0x7B,0x3F,0xE2,0x6C,0x7B,0x3F,0x44,0x3C,0x77,0x3F,0xB2,0x3C,0x7F,0x3F,0x05,0x80,0x51,0x3F,0xF5,0x6A,0x75,0x3F,0x0A,0xB7,0x70,0x3F,0x8D,0x7F,0x73,0x3F,0x70,0x7E,0x79,0x3F,0xA5,0x23,0x72,0x3F,0x92,0x7D,0x78,0x3F,0x66,0x75,0x7F,0x3F,0xB4,0x79,0x55,0x3F,0xF9,0x37,0x79,0x3F,0x92,0x07,0x79,0x3F,0x66,0x9C,0x79,0x3F,0xDC,0x2E,0x6D,0x3F,0x33,0xD3,0x7F,0x3F,0xAC,0xE5,0x78,0x3F,0x6A,0xFA,0x79,0x3F,0xCE,0x65,0x7E,0x3F,0xF4,0x43,0x27,0x3F,0xFE,0x77,0x7E,0x3F,0xE6,0x18,0x7D,0x3F,0x34,0x52,0x7B,0x3F,0xD6,0x2E,0xA2,0x3E,0x30,0xFF,0x62,0x3F,0x3A,0x8E,0x1A,0x3F,0x7D,0x49,0x7D,0x3F,0xBA,0x68,0xBA,0x3D,0xCF,0xC0,0x4E,0x3E,0xA1,0x80,0x20,0x3D,0x8D,0x36,0x8D,0x3E,0x20,0xC0,0x6C,0x3F,0x8E,0xB6,0x74,0x3F,0xB3,0x7E,0x7F,0x3F,0x69,0xF3,0x12,0x3F,
	0xC7,0x74,0x7A,0x3F,0x94,0x06,0x7B,0x3F,0xF6,0x20,0x76,0x3F,0x1A,0x85,0x13,0x3F,0xBA,0x74,0x6D,0x3F,0x8E,0x57,0x74,0x3F,0x8A,0x60,0x71,0x3F,0x7A,0xD8,0x13,0x3F,0x11,0xF2,0xA9,0x3E,0x3E,0xFD,0x3D,0x3F,0x60,0xF4,0x25,0x3F,0x40,0xC5,0x39,0x3F,0xC9,0x05,0x49,0x3F,0xE0,0xFF,0x5F,0x3F,0x95,0x2C,0x15,0x3E,0x9D,0x7B,0x1D,0x3F,0xA8,0xE7,0xA7,0x3E,0xFC,0xE9,0x61,0x3F,0x51,0xD2,0x17,0x3F,0x0A,0xBF,0x33,0x3F,0xDB,0x64,0xA7,0x3E,0xF3,0x52,0x59,0x3F,0x34,0x02,0x2E,0x3F,0x3F,0x2A,0x55,0x3F,0xD2,0x05,0x9E,0x3E,0x66,0xDF,0x65,0x3F,0x65,0xDF,0x1B,0x3F,0x48,0xD9,0x36,0x3F,0x6A,0x01,0x9D,0x3E,0x4E,0x55,0x4D,0x3F,0x49,0xAE,0xEC,0x3E,0x7A,0x48,0x57,0x3F,0x6B,0x76,0x9D,0x3E,0x63,0x0E,0x2F,0x3F,0xA1,0x47,0x89,0x3E,0x57,0xD5,0x55,0x3F,0x38,0x2F,0x9E,0x3E,0x19,0x71,0x32,0x3F,0x58,0xCB,0xFD,0x3E,0x26,0x68,0x61,0x3F,0xC7,0x51,0xC6,0x3E,0xAB,0x00,0x2B,0x3D,0x1F,0xAC,0x1A,0x3D,0x94,0xC6,0x7F,0x3F,0x9A,0x18,0x19,0x3F,0xB7,0xCB,0x03,0x3D,0x16,0x15,0x1B,0x3E,0x19,0x8F,0x7F,0x3F,0x7C,0x3D,0x7C,0x3F,0x2E,0xFD,0x7A,0x3F,0x4C,0x1C,0x7F,0x3F,0x37,0x87,0xCE,0x3E,0x4C,0x17,0x7F,0x3F,0x3C,0x68,0x6F,0x3F,0x73,0xE0,0x73,0x3F,0x09,0xA1,0x13,0x3F,0x77,0xE1,0x76,0x3F,0xE3,0xFD,0x7C,0x3F,0x8E,0x09,0x75,0x3F,0x04,0x55,0xFC,0x3E,0x8A,0xB3,0xF0,0x3E,0xAF,0x9D,0x48,0x3F,0x32,0x3A,0x2B,0x3F,0xFD,0xC6,0x1D,0x3F,
	0xA6,0xBC,0xD9,0x3E,0x8D,0x7E,0x59,0x3F,0xB7,0x86,0x17,0x3F,0xA9,0x22,0x28,0x3F,0xD3,0xB6,0xD2,0x3E,0xEC,0xAC,0x6B,0x3E,0x9A,0x84,0x19,0x3E,0xAF,0xC2,0x2E,0x3F,0xF7,0x23,0xF7,0x3E,0x24,0x5B,0x56,0x3F,0x66,0x5F,0x2C,0x3F,0x15,0x1B,0x3F,0x3F,0x47,0x69,0x13,0x3F,0x07,0xA7,0x20,0x3D,0x3E,0x4D,0x67,0x3D,0x98,0x6E,0x42,0x3F,0xB1,0xCE,0x16,0x3F,0x1C,0x34,0x68,0x3C,0x7A,0x01,0x8F,0x3D,0x43,0xE0,0x5E,0x3F,0xEB,0x7A,0x04,0x3F,0xC9,0x53,0x15,0x3C,0xF2,0xFD,0xE1,0x3D,0x3B,0x9B,0x5C,0x3F,0x58,0x2B,0xF1,0x3E,0xBB,0xB3,0x86,0x3B,0x6B,0x7F,0xC1,0x3D,0xC8,0x1D,0x47,0x3F,0x94,0x0D,0x13,0x3F,0xDC,0x13,0x28,0x3C,0x0B,0xC2,0xFF,0x3C,0x96,0x79,0x67,0x3F,0xC3,0x83,0x0F,0x3F,0xC0,0x00,0xBF,0x3A,0x65,0x92,0x88,0x3D,0xE4,0x9D,0x4C,0x3F,0x71,0x4A,0xD7,0x3D,0xB5,0xDB,0x34,0x3E,0x41,0x64,0x54,0x3C,0xF5,0xE0,0x74,0x3F,0xFB,0x19,0x7B,0x3F,0xAC,0xAC,0x78,0x3F,0x74,0xD5,0x74,0x3F,0x9D,0x84,0xEA,0x3E,0xDE,0x3F,0x78,0x3F,0x66,0xBB,0x7F,0x3F,0x62,0xDD,0x7B,0x3F,0xC6,0xA4,0xE6,0x3E,0x24,0x17,0x57,0x3F,0x01,0x09,0x4E,0x3F,0xE2,0x43,0x57,0x3F,0xAB,0x1F,0xDC,0x3E,0x2F,0x5C,0xC8,0x3E,0x9A,0xB6,0x65,0x3F,0x35,0x66,0x2E,0x3F,0x6F,0xF3,0x15,0x3F,0xA2,0x27,0xD4,0x3E,0xFB,0xD6,0xC6,0x3E,0xEA,0xBC,0x07,0x3F,0xA6,0xFE,0x02,0x3F,0x98,0x63,0x17,0x3F,0x59,0x7A,0xBE,0x3C,0x90,0x95,0x21,0x3E,0xAF,0x9F,0x1C,0x3F,
	0xF4,0x1A,0x0D,0x3F,0x4F,0xCD,0x81,0x3C,0x17,0x6B,0x84,0x3B,0xB1,0xF8,0x1E,0x3F,0xD1,0x6E,0xD0,0x3E,0x7B,0x85,0x2D,0x3D,0x69,0xE5,0x25,0x3E,0xC8,0x9D,0x1F,0x3F,0x04,0x77,0x9D,0x3E,0x89,0xB2,0x6D,0x3D,0x6A,0xAD,0x11,0x3E,0x04,0xEA,0xDB,0x3E,0x18,0xBA,0xE3,0x3E,0x0D,0x27,0xA5,0x3B,0x7D,0x88,0x61,0x3D,0x50,0x61,0xA4,0x3E,0x6E,0xDD,0x9F,0x3C,0x88,0xF4,0x07,0x3D,0x58,0xCD,0x0A,0x3B,0x98,0xEB,0xD4,0x3E,0xA8,0x40,0xA7,0x3D,0x93,0x9C,0x92,0x3D,0x3D,0x5A,0x23,0x3D,0xC0,0x7E,0x39,0x3F,0x78,0xCD,0xA9,0x3A,0xA8,0xEE,0x27,0x3E,0x28,0x2A,0x8E,0x3C,0xE6,0xD6,0x65,0x3F,0x97,0x00,0x16,0x3A,0xA4,0x9F,0x23,0x3E,0xBE,0x13,0x8B,0x3C,0xFD,0xC3,0x7C,0x3F,0xF1,0xA2,0x70,0x3F,0x00,0xD5,0x7F,0x3F,0xFD,0x46,0x7D,0x3F,0x77,0x3E,0xDC,0x3E,0xF8,0x35,0x78,0x3F,0xED,0xC7,0x6C,0x3F,0xF9,0xBB,0x78,0x3F,0xF9,0x69,0x39,0x3E,0xB5,0x6F,0xB5,0x3E,0x7B,0x74,0x47,0x3F,0x85,0x84,0x95,0x3D,0x27,0xBA,0x8B,0x3E,0xD1,0x10,0x51,0x3E,0x28,0xC5,0x5A,0x3F,0xF3,0x43,0x2D,0x3F,0xEE,0x45,0x06,0x3F,0x86,0x01,0x06,0x3F,0xB4,0x00,0xB4,0x3A,0x93,0x4E,0x0C,0x3D,0xD4,0xFA,0x1C,0x3F,0xF6,0xE6,0x0F,0x3B,0xB4,0x33,0x80,0x3C,0xF8,0x5F,0xC4,0x3D,0x66,0x31,0x99,0x3D,0x64,0x7D,0x97,0x3C,0x40,0x0F,0x59,0x3D,0x8C,0xF0,0x8B,0x3C,0x2C,0x72,0x92,0x3D,0x23,0x0A,0x89,0x3D,0xF6,0x8A,0x8F,0x3D,0xF6,0xD3,0x42,0x3C,0xBD,0x7B,0x89,0x3D,
	0x00,0x74,0xCC,0x3D,0x6D,0x12,0x53,0x3D,0xE9,0xE6,0x82,0x3D,0xE6,0x0B,0xB3,0x3D,0xE3,0xC3,0xAF,0x3C,0x90,0xA1,0xC2,0x3D,0xF5,0x33,0xC1,0x3A,0x8A,0x2D,0x3D,0x3D,0x04,0x0C,0x50,0x3D,0xC0,0x09,0x40,0x3E,0xD0,0xC7,0x9C,0x3D,0x0D,0x69,0x42,0x3F,0x2D,0xDA,0x92,0x3B,0x9D,0x1E,0x1D,0x3E,0xDC,0x00,0xDC,0x3A,0xDD,0xAA,0x50,0x3F,0x30,0xDA,0x95,0x3B,0xCB,0xEF,0x4A,0x3E,0xDF,0x28,0x5F,0x3D,0x00,0x79,0x3A,0x3F,0x48,0x1A,0xAE,0x3B,0xC7,0x87,0x93,0x3D,0x01,0x67,0x1A,0x3D,0x55,0xFA,0x3A,0x3C,0xE5,0xB2,0x64,0x3F,0xEE,0x96,0x6D,0x3F,0xFA,0x3F,0x7A,0x3F,0x4C,0x9C,0xA6,0x3E,0xFF,0xF6,0x7E,0x3F,0xDC,0x9F,0x5B,0x3F,0xFF,0x1C,0x7F,0x3F,0xC8,0x11,0xE3,0x3E,0xD4,0x27,0xD4,0x3E,0x2F,0x19,0x48,0x3F,0xBE,0xAD,0x19,0x3F,0x83,0x34,0xE7,0x3E,0xF7,0xC9,0x0F,0x3F,0xF4,0xDB,0x0C,0x3F,0x10,0xBF,0x34,0x3F,0xEA,0xD6,0x02,0x3F,0x21,0xBC,0x06,0x3F,0xE3,0x20,0xE2,0x3C,0xFD,0x8B,0x0B,0x3D,0x24,0x55,0xFB,0x3E,0x99,0x28,0x19,0x3D,0x1E,0x94,0xEA,0x3C,0xEA,0x8E,0x83,0x3D,0x98,0x20,0x18,0x3D,0xCA,0x63,0x16,0x3D,0xBC,0x84,0xBB,0x3D,0xEB,0x70,0xEA,0x3C,0x22,0xF6,0x87,0x3D,0x90,0xCD,0xC2,0x3A,0xE8,0xB3,0x34,0x3D,0x2B,0x47,0x44,0x3D,0x88,0xAC,0x87,0x3D,0x99,0xB0,0x98,0x3C,0x74,0x4D,0x27,0x3B,0x9C,0x74,0x9B,0x3D,0x68,0x9A,0x4E,0x3B,0x12,0xD3,0xAA,0x3D,0x85,0x1D,0x05,0x3E,0x3F,0x77,0x58,0x3D,0x4F,0x1E,0x13,0x3E,
	0x2F,0xB4,0xFA,0x3C,0xCF,0x0E,0xCF,0x3D,0x8B,0xED,0x3D,0x3C,0x00,0xCE,0x3A,0x3F,0xB0,0xB0,0xAE,0x3C,0xCD,0x0D,0x4D,0x3E,0x54,0x1A,0x3A,0x3C,0xC3,0xB2,0x50,0x3F,0x76,0xBD,0xA7,0x3C,0xCE,0x94,0x4D,0x3D,0xEC,0x0B,0x39,0x3D,0xC3,0x91,0x31,0x3F,0xA5,0x00,0xA5,0x3C,0x48,0x2F,0x61,0x3D,0xA4,0x6C,0xA3,0x3D,0x93,0xDA,0xF8,0x3C,0x00,0xE4,0x7F,0x3F,0xFE,0x9E,0x7D,0x3F,0xFA,0x9B,0x79,0x3F,0x64,0xFE,0xD9,0x3E,0xE1,0x84,0x60,0x3F,0xE0,0x63,0x60,0x3F,0xCD,0x49,0x4D,0x3F,0x3E,0x29,0xB4,0x3C,0xB5,0x76,0xB5,0x3E,0x13,0x4F,0x5F,0x3F,0x80,0x15,0x07,0x3F,0xC7,0xF0,0xCA,0x3E,0xE8,0x38,0x81,0x3E,0xC5,0x00,0xC4,0x3C,0x9E,0x7F,0x74,0x3D,0x26,0xD0,0xA1,0x3E,0xCD,0x8B,0x19,0x3F,0x10,0x84,0xDB,0x3C,0xD0,0xAD,0xBE,0x3D,0xE0,0x78,0x04,0x3F,0x1A,0x4C,0x66,0x3D,0xDF,0xA0,0x5E,0x3C,0xE7,0xD0,0x66,0x3D,0xF9,0x27,0xC5,0x3D,0x85,0x35,0xB8,0x3D,0xE3,0xB0,0xE2,0x3C,0x5C,0x29,0x8F,0x3D,0x4F,0x9A,0xB5,0x3B,0x8E,0x95,0x40,0x3D,0xB8,0xCD,0xEA,0x3B,0x82,0x9C,0x81,0x3D,0x6B,0x6D,0x1E,0x3C,0xC7,0xC3,0x92,0x3C,0xD5,0x42,0xD5,0x3D,0x6A,0xC5,0x1C,0x3D,0x1F,0xD4,0xAC,0x3E,0x2A,0x07,0xC2,0x3C,0xF7,0x38,0x77,0x3D,0x96,0x9A,0xFC,0x3B,0xBA,0xBB,0x32,0x3F,0xF0,0x42,0x89,0x3D,0xE3,0x88,0x62,0x3E,0xFD,0x66,0x96,0x3C,0xA5,0xF1,0x12,0x3F,0x50,0x3A,0xB6,0x3C,0x00,0x74,0xCC,0x3B,0x24,0x5B,0xBD,0x3D,0x94,0xD2,0x79,0x3D,
	0xA8,0x34,0xA8,0x3D,0x40,0x6A,0x26,0x3D,0xA7,0x00,0xA7,0x3A,0xAC,0xC0,0xAB,0x3B,0xE3,0x8B,0xAF,0x3D,0xC4,0x53,0x90,0x3D,0x5F,0x69,0x92,0x3D,0xBA,0x30,0xBA,0x3C,0xFC,0x71,0x7C,0x3F,0xFE,0x9C,0x7D,0x3F,0x00,0xDB,0x7F,0x3F,0xAC,0x0E,0x89,0x3E,0xF5,0xF8,0x74,0x3F,0xAE,0xC3,0x2D,0x3F,0xA4,0x59,0x24,0x3F,0xE3,0x91,0x9D,0x3E,0xFB,0x7C,0xFB,0x3E,0xFB,0x27,0x2E,0x3F,0xB3,0xD3,0x33,0x3F,0xDD,0xF2,0xB9,0x3E,0xF6,0x9C,0x0F,0x3F,0xD4,0xD8,0x52,0x3D,0x43,0x1E,0xA9,0x3C,0x2E,0xAD,0x98,0x3E,0x45,0x1B,0xDE,0x3E,0x17,0xB4,0x62,0x3C,0xE9,0xD0,0xFA,0x3D,0x42,0x4B,0x08,0x3F,0x53,0x1D,0x06,0x3D,0x35,0x3F,0x4E,0x3D,0xBE,0xD5,0x70,0x3D,0x7D,0x82,0x63,0x3D,0xB5,0x98,0xB4,0x3D,0x1B,0xE7,0x34,0x3B,0x42,0x62,0xA8,0x3D,0x20,0x9A,0x86,0x3B,0xE2,0x40,0xE0,0x3C,0x91,0x88,0x90,0x3C,0x1D,0x24,0xE9,0x3C,0x5B,0x50,0xBF,0x3E,0x6B,0xDA,0xCF,0x3C,0xB3,0x93,0x32,0x3E,0x2E,0x54,0xFA,0x3C,0xC6,0x13,0x07,0x3F,0x23,0xA4,0xEE,0x3C,0xE3,0xCB,0x62,0x3E,0x55,0xA7,0xEE,0x3C,0x64,0xA6,0x32,0x3F,0xA6,0x00,0xA5,0x3D,0xD0,0x95,0x4F,0x3E,0x62,0xDA,0xC7,0x3B,0x3C,0x20,0x70,0x3E,0x20,0x2A,0x86,0x3D,0x11,0x87,0x2A,0x3D,0x32,0xEE,0x97,0x3D,0xCC,0x8F,0x98,0x3D,0xFB,0x33,0xC7,0x39,0x24,0xA7,0x3D,0x3D,0x54,0xAA,0xBA,0x3C,0xBB,0x9D,0x6D,0x3D,0xB1,0xED,0xE3,0x3C,0x2A,0xFA,0x0F,0x3D,0x6B,0xBA,0x50,0x3C,0xF5,0x23,0x41,0x3D,
	0x00,0x81,0x7F,0x3F,0xFD,0xDD,0x7C,0x3F,0x00,0xF8,0x7F,0x3F,0x69,0x52,0x95,0x3E,0xC4,0xCE,0xC3,0x3E,0xF9,0xB5,0x78,0x3F,0xC9,0xD4,0x48,0x3F,0x0B,0x25,0xC6,0x3E,0x8E,0x40,0x8E,0x3E,0xE2,0x87,0x61,0x3F,0x06,0xAF,0xEB,0x3E,0x28,0xBB,0x9E,0x3E,0x22,0xF1,0x07,0x3F,0x96,0x9A,0x7A,0x3B,0xF0,0x3A,0x30,0x3E,0xF9,0x9A,0xBF,0x3E,0x87,0xDF,0x06,0x3F,0xF2,0x6A,0xF2,0x3D,0x1C,0x79,0x07,0x3E,0xC1,0xD0,0xBF,0x3E,0xBB,0x83,0x87,0x3C,0x0A,0xA7,0x23,0x3D,0x52,0x65,0x05,0x3D,0xDE,0x33,0x2A,0x3D,0x99,0x92,0x7D,0x3D,0xF8,0xD1,0x77,0x3E,0xF5,0x23,0xC1,0x3C,0x43,0x03,0xAE,0x3E,0x2B,0x1A,0x90,0x3B,0xAB,0x78,0xAB,0x3D,0x81,0x40,0x01,0x3C,0xBE,0xF1,0xEE,0x3E,0x49,0xB2,0x2E,0x3D,0xCC,0x3C,0x4C,0x3E,0xF5,0x83,0xC1,0x3C,0x21,0xC4,0xEB,0x3E,0x34,0x9A,0x9A,0x3C,0x00,0xAC,0xCC,0x3D,0xC7,0xAD,0xF9,0x3C,0x7B,0x05,0x2E,0x3D,0xC0,0x1C,0xC0,0x3D,0x97,0x8D,0xC9,0x3B,0xA6,0x28,0xA6,0x3D,0x1B,0xA3,0xB4,0x3D,0xDD,0x40,0xDD,0x3B,0x6F,0x5D,0xA2,0x3C,0xC8,0xCF,0x94,0x3D,0x49,0xDA,0x2E,0x3D,0x6F,0x3D,0xA2,0x3C,0x4A,0x3A,0xB0,0x3D,0xBF,0xF3,0x0B,0x3C,0xED,0x73,0xB9,0x3C,0xB6,0x54,0xB5,0x3D,0xFE,0xBE,0x97,0x3D,0xA7,0x7D,0x59,0x3D,0x8B,0x15,0xBE,0x3D,0x00,0xEE,0x7F,0x3F,0xFB,0xF0,0x7A,0x3F,0xF6,0x88,0x75,0x3F,0x81,0x2F,0x49,0x3E,0xD5,0x4F,0xD5,0x3E,0xC0,0x7B,0x59,0x3F,0x74,0xB2,0x30,0x3F,0x5D,0xED,0x3C,0x3E,
	0x63,0x9D,0xFB,0x3E,0x93,0x52,0x92,0x3E,0x42,0x9E,0xD5,0x3E,0x9E,0x82,0x6C,0x3E,0x00,0x93,0x19,0x3F,0x26,0x9A,0x8B,0x3B,0xC8,0x2E,0xB0,0x3C,0xAD,0x40,0xAC,0x3E,0xF7,0x26,0x90,0x3C,0x8A,0x60,0x8A,0x3C,0xC9,0x17,0x17,0x3E,0xA2,0xFC,0x64,0x3D,0x73,0xBD,0x24,0x3D,0xE8,0x06,0x68,0x3E,0x1C,0x37,0x35,0x3D,0x46,0x96,0x89,0x3E,0xA1,0x10,0xA0,0x3C,0xC1,0x99,0x40,0x3E,0xBC,0x00,0x3C,0x3D,0x48,0xDF,0xC9,0x3E,0xF5,0x1F,0xC1,0x3D,0x96,0xD8,0x95,0x3D,0x8E,0x90,0x8D,0x3C,0x4D,0xA1,0xAC,0x3E,0xF7,0x03,0xC4,0x3C,0xB8,0x93,0x84,0x3C,0xA0,0xE0,0x1F,0x3C,0xEA,0xDE,0x83,0x3D,0x12,0x4F,0x2B,0x3D,0xB9,0xEB,0x05,0x3D,0x07,0xC4,0xD3,0x3C,0xE5,0x08,0x65,0x3D,0x4F,0xFF,0x67,0x3D,0xD7,0x10,0xD7,0x3C,0x33,0x3F,0xCC,0x3D,0x71,0x99,0xA3,0x3D,0x52,0x2D,0x85,0x3C,0x43,0x2A,0x29,0x3D,0x32,0x1A,0x98,0x3B,0x0E,0x63,0xA7,0x3D,0xF9,0xD3,0x45,0x3D,0x97,0x6D,0x4A,0x3D,0x44,0x5A,0xAA,0x3C,0x12,0xB4,0x5E,0x3C,0x05,0xFF,0x9D,0x3D,0x30,0xEA,0x95,0x3C,0xD2,0xC8,0x51,0x3D,0xE4,0x63,0xB0,0x3D,0xF7,0xD9,0x76,0x3F,0xF9,0x4E,0x79,0x3F,0xFF,0x69,0x7F,0x3F,0x21,0x0A,0x06,0x3E,0xE7,0xDB,0xE6,0x3E,0xCB,0x61,0x64,0x3F,0xB3,0xD3,0x33,0x3F,0x03,0x61,0x5F,0x3E,0xEA,0xDC,0x82,0x3E,0xC4,0x42,0xC3,0x3E,0xB4,0xE9,0x78,0x3E,0x95,0xFB,0x51,0x3E,0xC3,0x74,0x0F,0x3F,0xC2,0xA3,0x8D,0x3C,0x83,0x29,0xCB,0x3D,0x9A,0x6F,0x93,0x3E,
	0xBC,0x93,0x08,0x3D,0xFC,0xC4,0xFB,0x3D,0x2F,0x2E,0x6B,0x3B,0xD5,0x51,0xD4,0x3E,0x98,0xDA,0xFC,0x3B,0xDF,0xED,0x5E,0x3E,0xB7,0xF3,0x03,0x3C,0xB8,0x16,0xAB,0x3C,0x3C,0x0A,0x21,0x3D,0x95,0x36,0x95,0x3D,0x31,0x52,0x17,0x3D,0x6E,0xE8,0xD2,0x3E,0x17,0x5B,0xAF,0x3D,0xF1,0x6F,0x71,0x3E,0x9C,0xCD,0xCE,0x3B,0x87,0xED,0xB8,0x3D,0xF8,0x27,0xC4,0x3D,0x1E,0x32,0x04,0x3D,0x4D,0x05,0x00,0x3D,0xEC,0x68,0x6B,0x3D,0x88,0x11,0xBB,0x3D,0xBD,0x33,0x89,0x3D,0x23,0xEA,0x08,0x3D,0x24,0x67,0xBD,0x3B,0x09,0xE7,0x22,0x3C,0x0E,0x4C,0x5A,0x3D,0xBE,0xF0,0xBD,0x3C,0xF4,0xD0,0x73,0x3D,0x33,0x22,0x99,0x3D,0x24,0x97,0xBD,0x3D,0xFB,0x1F,0xC7,0x3D,0xC0,0xC0,0x3F,0x3D,0x33,0xD4,0x7F,0x3D,0xA7,0xA8,0x26,0x3D,0xF8,0x88,0x77,0x3D,0xE7,0x4E,0x00,0x3D,0x55,0xDD,0x87,0x3C,0x14,0xF7,0xAC,0x3C,0xC9,0x57,0x95,0x3D,0x90,0x3C,0x90,0x3D,0xFD,0xBF,0x7C,0x3F,0xFE,0x40,0x7E,0x3F,0xF1,0x26,0x71,0x3F,0xCC,0xB4,0x06,0x3E,0xE5,0x00,0x65,0x3E,0x4C,0x7D,0x65,0x3F,0x3B,0x4A,0x32,0x3F,0x37,0x85,0xF0,0x3D,0xEF,0x19,0x08,0x3F,0x1B,0x40,0x01,0x3E,0x41,0x66,0x60,0x3E,0x3F,0x39,0x1E,0x3E,0xBF,0x97,0xF1,0x3E,0xF7,0x00,0xF6,0x3A,0x60,0x3D,0x0C,0x3D,0x75,0x99,0x32,0x3E,0x70,0x3D,0x23,0x3D,0x8A,0xC4,0x09,0x3D,0x88,0x1E,0x2F,0x3C,0xEF,0x81,0x5C,0x3D,0x67,0xDD,0x98,0x3C,0xF0,0x88,0x6F,0x3E,0x56,0xFA,0xBB,0x3C,0x98,0x9C,0x96,0x3E,
	0x72,0xDA,0x56,0x3C,0xB1,0x7A,0xB1,0x3D,0x35,0xF7,0xCD,0x3C,0x34,0x52,0x8E,0x3E,0x26,0x3F,0x3F,0x3D,0x5A,0x47,0x73,0x3C,0x36,0xA2,0x1C,0x3D,0x94,0x70,0x93,0x3C,0x0F,0x73,0xA8,0x3D,0x27,0x43,0xC0,0x3D,0x8B,0xAC,0x8A,0x3D,0x5E,0x52,0x44,0x3D,0xBF,0x65,0x72,0x3D,0x10,0xC7,0x29,0x3D,0x79,0xE1,0xAB,0x3D,0x30,0x3A,0x16,0x3C,0x54,0x15,0x07,0x3D,0xE4,0x9B,0x30,0x3D,0x6E,0x95,0xA0,0x3D,0x78,0x5D,0xAB,0x3D,0xFC,0x98,0x7B,0x3D,0x18,0x14,0x64,0x3D,0x62,0x8E,0xC8,0x3D,0xA6,0x90,0xA5,0x3D,0x5A,0x7F,0x73,0x3D,0x8C,0x38,0x0C,0x3D,0xA4,0xA8,0xA3,0x3D,0xE8,0xB7,0xB4,0x3D,0x2F,0xC7,0xC8,0x3C,0x52,0x8D,0x05,0x3C,0xA7,0xAD,0xD9,0x3C,0x2A,0x34,0x76,0x3C,0xF7,0xF7,0x76,0x3F,0xFF,0x5E,0x7F,0x3F,0xF5,0x78,0x75,0x3F,0x93,0xD8,0x06,0x3E,0xD9,0x99,0xD8,0x3E,0x2F,0xBB,0x61,0x3F,0x45,0xCE,0x1C,0x3F,0xF4,0xDE,0x23,0x3E,0x97,0xC6,0x16,0x3F,0xB4,0xB3,0x00,0x3B,0x45,0xE4,0x14,0x3D,0xD6,0x28,0x55,0x3D,0xFC,0x00,0x95,0x3E,0x73,0x9A,0x58,0x3C,0x11,0xC7,0xAF,0x3D,0xA0,0x77,0xC6,0x3D,0xCB,0xED,0xFC,0x3C,0xBD,0x74,0xBD,0x3D,0xE2,0xC8,0x61,0x3D,0x8A,0xCD,0xBA,0x3C,0x0C,0x07,0x25,0x3D,0x8E,0x89,0x0D,0x3E,0xB5,0xD3,0x81,0x3C,0x14,0x58,0x1B,0x3E,0x35,0x2A,0x1B,0x3D,0x4E,0x25,0x81,0x3D,0x0A,0x34,0xD6,0x3C,0x4C,0x87,0x65,0x3D,0x57,0x1D,0x0A,0x3D,0x0C,0xCB,0xA5,0x3D,0x03,0xC7,0x1C,0x3D,0x4C,0x07,0xE5,0x3C,
	0x1F,0xB4,0x6B,0x3D,0x6D,0xDA,0xD2,0x3B,0x6A,0x0D,0x9D,0x3B,0xC9,0xB3,0x15,0x3B,0x9B,0xCC,0x9A,0x3D,0x5E,0x3D,0x91,0x3D,0x67,0x9A,0x4D,0x3C,0x9F,0x10,0x9F,0x3C,0xF4,0x93,0xC0,0x3D,0xF1,0x00,0x71,0x3C,0xBE,0xFF,0x8A,0x3D,0xCD,0x03,0x9A,0x3D,0x66,0xC5,0x18,0x3D,0xEC,0xA0,0xEB,0x3C,0x8B,0x9A,0xF1,0x39,0xDA,0x6F,0xA6,0x3D,0x7C,0xEA,0x61,0x3D,0x42,0x1A,0x28,0x3B,0xF7,0x03,0xC4,0x3D,0x96,0xB8,0x95,0x3D,0x22,0x32,0x08,0x3D,0xEF,0x26,0x08,0x3C,0x8B,0x08,0x8B,0x3D,0x3A,0x87,0x53,0x3D,0xFE,0xD7,0x7D,0x3F,0xF8,0x7E,0x78,0x3F,0xFC,0x1C,0x7C,0x3F,0x46,0xB5,0x25,0x3C,0xC0,0x1F,0xC0,0x3E,0x99,0x4D,0x65,0x3F,0x8F,0xFF,0x35,0x3F,0x27,0xB1,0x4A,0x3C,0x47,0x13,0xE0,0x3E,0x95,0xDF,0x47,0x3E,0x0F,0xB0,0xC1,0x3E,0xC6,0x00,0x44,0x3A,0x26,0x96,0x0B,0x3F,0x99,0x00,0x18,0x3C,0x2C,0x89,0x28,0x3C,0x70,0x33,0x16,0x3D,0x8C,0x8D,0x3E,0x3C,0xF8,0x6A,0xF8,0x3D,0x89,0x82,0x6F,0x3D,0xAF,0xB4,0xC4,0x3C,0xBC,0x23,0x88,0x3C,0xE0,0x78,0x60,0x3E,0x70,0x5A,0x56,0x3C,0x30,0x9B,0x84,0x3C,0xF1,0xE6,0x89,0x3B,0xA1,0xA4,0x20,0x3D,0xC4,0x3B,0x10,0x3D,0x6A,0x4D,0x1A,0x3B,0xBE,0x9D,0x70,0x3D,0xDD,0x53,0xA9,0x3D,0x39,0x2E,0x9F,0x3D,0xFF,0xC7,0xCB,0x3D,0x3F,0x2E,0xA5,0x3D,0x50,0xDA,0xB5,0x3C,0x2E,0xA7,0x47,0x3C,0x0A,0x7F,0xA3,0x3D,0x4F,0x4A,0x35,0x3D,0xF1,0x13,0xBE,0x3D,0x6B,0x11,0x9E,0x3D,0x66,0x09,0x99,0x3D,
	0x2A,0xB2,0x90,0x3D,0x00,0xFC,0x4C,0x3D,0xEE,0x00,0xEE,0x3C,0x84,0xFA,0xE9,0x3C,0x54,0xF1,0x86,0x3D,0xDB,0x13,0x28,0x3D,0x2B,0x33,0xC4,0x3D,0xFE,0xC6,0x17,0x3D,0x83,0x0A,0xE9,0x3C,0x8E,0xFA,0x73,0x3C,0x60,0xAD,0x92,0x3D,0x5E,0x66,0xC4,0x3D,0xC1,0x6D,0xF4,0x3C,0x2F,0x42,0x95,0x3D,0x02,0xB4,0x4E,0x3D,0x4E,0x29,0x81,0x3D
},

/* input data and other run-time fields*/

{0}

}; /* done */
