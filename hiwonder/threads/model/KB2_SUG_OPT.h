/* This file is automatically generated by SCOptimizer */

#include "mcinfer.h"

fuzzy_model model={

0, /* model_ready set to OK */
1, /* engine code (algorithm type) */
1, /* min_inference_mode */
1, /* database type */
2, /* number of input variables */
4, /* number of output variables */

/* Input variable normalization parameters */
{{0.001634f,-2.385621f},{0.001323f,-0.096561f}},

/* Output variable normalization parameters */
{{0.031250f,-0.156250f},{0.023810f,-0.714286f},{0.025000f,-2.250000f},{0.019608f,-0.470588f}},

/* number of input MF's per variable*/ 
{9,9},
/* input MF paramteres */ 
{
	{descending,0.082925f,0,0,0.165721f},{triangular,0.217729f,0,0.103677f,0.103677f},{triangular,0.334559f,0,0.129801f,0.129801f},{triangular,0.421160f,0,0.043267f,0.043267f},{triangular,0.495098f,0,0.104494f,0.104494f},{triangular,0.589869f,0,0.084901f,0.084901f},{triangular,0.714052f,0,0.163272f,0.163271f},{triangular,0.852533f,0,0.113474f,0.113474f},{ascending,0.954657f,0,0.090616f,0},
	{descending,0.034722f,0,0,0.138213f},{triangular,0.172950f,0,0.138213f,0.162020f},{triangular,0.277778f,0,0.047614f,0.101841f},{triangular,0.371693f,0,0.085970f,0.044969f},{triangular,0.419312f,0,0.050259f,0.088615f},{triangular,0.558862f,0,0.190456f,0.141519f},{triangular,0.667328f,0,0.075389f,0.174585f},{triangular,0.828704f,0,0.148132f,0.068776f},{ascending,0.939153f,0,0.152100f,0}
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
81, /*number of rules */

/* index */
{0}, /* empty */
/* database */
{
	0xDE,0x9A,0x59,0x3F,0x12,0xE3,0x7F,0x3F,0x8A,0x77,0x7D,0x3F,0xF5,0xB9,0x7F,0x3F,0x4D,0xEC,0x7F,0x3F,0x09,0x98,0x1E,0x3F,0xD5,0x80,0x4D,0x3F,0xBE,0x2A,0x5E,0x3F,0xE4,0x94,0x1D,0x3F,0x30,0x31,0x66,0x3E,0x4A,0x4C,0x61,0x3E,0xE8,0xA0,0x73,0x3F,0x1C,0xD6,0x21,0x3F,0x6A,0xA0,0x6D,0x3E,0x38,0x74,0x83,0x3E,0x36,0xBD,0x63,0x3F,0xF5,0x38,0x2E,0x3F,0x10,0xCB,0x84,0x3E,0xE6,0x01,0x98,0x3E,0xC0,0xB4,0x59,0x3F,0xC1,0x03,0x8D,0x3B,0xFC,0xBD,0x65,0x3E,0x11,0x34,0xDD,0x39,0x1D,0x0C,0x58,0x3F,0x4D,0x4D,0x00,0x3B,0xDA,0xD3,0x81,0x3E,0x65,0xCD,0x94,0x3A,0x60,0x05,0x55,0x3F,0x30,0x2A,0x16,0x3D,0x50,0x34,0x92,0x3E,0x88,0x40,0x84,0x3C,0x10,0xA0,0x0F,0x3F,0xFA,0xAD,0x79,0x3F,0x93,0x2A,0x13,0x3F,0xD1,0x50,0x51,0x3D,0xA4,0xDE,0x23,0x3F,0x5A,0xE5,0x7F,0x3F,0x2B,0x7A,0x27,0x3F,0x1D,0x74,0x63,0x3F,0x5E,0x7C,0x66,0x3F,0x1A,0xF2,0x7F,0x3F,0x79,0x53,0x28,0x3F,0xE1,0x66,0x59,0x3F,0x97,0xFD,0x7E,0x3F,0xE0,0xE0,0xDF,0x3E,0xB2,0xCA,0xB1,0x3E,0x90,0xBC,0x0F,0x3F,0xBB,0xBC,0x3A,0x3F,0x66,0xCC,0x38,0x3F,0xE6,0x1D,0x50,0x3E,0xBC,0xFE,0xA1,0x3E,0x1A,0x8D,0x4D,0x3F,0x65,0xE0,0x37,0x3F,0x3C,0x7D,0x72,0x3E,0x58,0xD8,0xA3,0x3E,0x9B,0x1D,0x67,0x3F,0xC4,0xDD,0x10,0x3D,0x7B,0xA4,0x7E,0x3E,0xF1,0x1E,0x8A,0x3C,0x87,0xF0,0x11,0x3F,0x64,0x67,0x7D,0x3B,0x62,0xFC,0x64,0x3E,0x78,0x5A,0xD9,0x3B,0x3E,0xE0,0x39,0x3F,
	0xFA,0xE6,0x93,0x3C,0x10,0x12,0x7A,0x3E,0x97,0x6D,0x45,0x3C,0x28,0xAE,0x27,0x3F,0x8D,0x10,0x0D,0x3E,0xA3,0x6F,0x23,0x3F,0xAD,0xC0,0x2C,0x3D,0xAD,0x25,0x2D,0x3F,0xA4,0x84,0x48,0x3F,0xB3,0xF7,0x7F,0x3F,0x1A,0xFF,0x7F,0x3F,0xF3,0xC3,0x65,0x3F,0xDA,0x5A,0x4B,0x3F,0x00,0x74,0x7F,0x3F,0xE6,0x84,0x7F,0x3F,0xF1,0xE7,0x56,0x3F,0x8F,0xB1,0x0E,0x3F,0xEF,0x66,0x88,0x3C,0x6A,0xF5,0x9C,0x3D,0x78,0x1A,0x5E,0x3D,0xEF,0xD2,0x2E,0x3F,0x34,0x1F,0x79,0x3E,0x0E,0x16,0xDA,0x3D,0x87,0x13,0x66,0x3F,0x5C,0x42,0x28,0x3F,0xD2,0x12,0x2B,0x3E,0xA3,0xC1,0x88,0x3E,0x87,0x49,0x4F,0x3F,0x45,0xA3,0x17,0x3F,0xBA,0xDC,0xAA,0x3E,0xEE,0x82,0x86,0x3E,0x06,0xEA,0xF8,0x3E,0x57,0xFD,0x89,0x3C,0xDE,0x2C,0xA5,0x3E,0x74,0xCD,0x22,0x3B,0x47,0x9A,0x1D,0x3F,0x70,0x1A,0x56,0x3C,0xFA,0x6A,0xE1,0x3D,0x0A,0x87,0x1F,0x3C,0xAA,0xCB,0x08,0x3E,0x66,0x9A,0xCC,0x3B,0x69,0xB7,0x8F,0x3E,0x72,0x9A,0xD2,0x3A,0xD7,0x5C,0x02,0x3F,0xF3,0x09,0x2B,0x3F,0x00,0x93,0x7F,0x3F,0x00,0xFE,0x7F,0x3F,0x86,0x4B,0x4E,0x3F,0xC4,0xE2,0x6B,0x3F,0x00,0xFB,0x7F,0x3F,0xFE,0x53,0x7E,0x3F,0xE8,0x54,0x51,0x3F,0x11,0xE7,0x2A,0x3C,0x77,0xCD,0xA9,0x3A,0x7B,0xBD,0x2D,0x3D,0xD5,0xB3,0x21,0x3B,0x1C,0x6C,0x68,0x3D,0xD7,0xF3,0xA3,0x3C,0x2A,0x67,0xC3,0x3B,0x55,0x4D,0x88,0x3D,0x87,0x77,0x39,0x3F,0x70,0x0F,0x71,0x3E,0x3B,0x3F,0x87,0x3E,0xF7,0x10,0x00,0x3F,
	0x44,0x0F,0x37,0x3F,0x71,0xC2,0x17,0x3E,0xBD,0x3A,0x09,0x3E,0x92,0x04,0x30,0x3F,0xEA,0x56,0x83,0x3C,0xCA,0xF6,0x98,0x3E,0x60,0xF2,0x41,0x3D,0x86,0x65,0x04,0x3F,0xAB,0x4D,0xDE,0x3B,0x3F,0x36,0x38,0x3E,0xCB,0x00,0xC6,0x3A,0x7B,0xB2,0x0B,0x3F,0x65,0x4A,0x4B,0x3D,0xF9,0xE1,0x27,0x3E,0x69,0xB1,0x98,0x3D,0x82,0x1B,0x0B,0x3F,0x0E,0xFC,0x60,0x3F,0x00,0xA3,0x7F,0x3F,0xFF,0x55,0x7F,0x3F,0xAF,0xD2,0x5A,0x3F,0xFE,0x53,0x7E,0x3F,0xFF,0x70,0xFF,0x3E,0xCA,0x06,0x4A,0x3F,0x90,0x25,0x2C,0x3F,0x97,0x41,0xCA,0x3D,0xA4,0x00,0xA4,0x3D,0x64,0xD5,0x16,0x3D,0x5E,0x8D,0x91,0x3D,0x27,0xBA,0x0D,0x3C,0x30,0x0C,0x7D,0x3D,0xFC,0xFB,0x48,0x3D,0xAE,0x80,0x2D,0x3B,0x13,0x31,0x92,0x3E,0x90,0xBD,0x88,0x3D,0x54,0x45,0x86,0x3C,0x94,0xB0,0x30,0x3F,0x7B,0x15,0x27,0x3F,0xF3,0x9C,0xA2,0x3E,0x67,0x06,0xB3,0x3E,0x3F,0x34,0x11,0x3F,0x22,0xDA,0x07,0x3C,0xAA,0xB4,0xA7,0x3E,0xBD,0x50,0xB8,0x3C,0xC6,0xE5,0x06,0x3F,0x2C,0x34,0xF8,0x3B,0xB8,0xDA,0x9E,0x3E,0x28,0x67,0xBC,0x3A,0xA2,0xCA,0x5C,0x3E,0xFE,0x66,0x97,0x3A,0xE4,0xAC,0x84,0x3E,0x69,0xBD,0x98,0x3C,0xBE,0x25,0x02,0x3F,0x1C,0x7B,0x43,0x3F,0xFF,0xB5,0x7E,0x3F,0xFB,0xCB,0x7A,0x3F,0x4E,0x51,0x27,0x3F,0xFF,0xED,0x7E,0x3F,0xFB,0xBB,0x7A,0x3F,0x00,0xE9,0x7F,0x3F,0xA4,0x78,0x3B,0x3F,0xFF,0x6B,0x7F,0x3F,0x73,0x4D,0xA5,0x3B,0x1B,0x67,0xB3,0x3A,0x73,0x44,0xC8,0x3E,
	0xCD,0x13,0x1A,0x3C,0x14,0x5B,0xAD,0x3D,0xC7,0x23,0x94,0x3D,0x1F,0xB7,0xB8,0x3C,0x51,0x6D,0x04,0x3C,0x77,0x5A,0x5D,0x3D,0x88,0x41,0xBB,0x3D,0x35,0xE2,0x9A,0x3D,0xE0,0x23,0xAC,0x3D,0xDD,0x3B,0x29,0x3D,0x4B,0x7F,0x64,0x3D,0x0C,0xFF,0xA4,0x3D,0xBC,0x93,0x08,0x3C,0x01,0x4F,0x1A,0x3D,0xE5,0x4B,0x31,0x3D,0x6E,0x4A,0x54,0x3D,0x3A,0x7A,0x20,0x3C,0xBF,0xF1,0x86,0x3C,0x9A,0x00,0x96,0x3B,0x49,0xE0,0x89,0x3C,0x0E,0x27,0x27,0x3C,0xDD,0x00,0xA0,0x3E,0xFD,0xE6,0x92,0x3B,0x59,0x0A,0xB2,0x3E,0x7C,0x85,0x08,0x3E,0xFF,0x17,0x7F,0x3F,0xFD,0x26,0x7D,0x3F,0x7C,0x1E,0x4D,0x3F,0xCF,0xDD,0x4E,0x3F,0x00,0xE8,0x7F,0x3F,0xFF,0x7E,0x7F,0x3F,0x1D,0x0F,0x15,0x3F,0x00,0xB8,0x7F,0x3F,0xC2,0x70,0x42,0x3F,0xF3,0xB4,0x72,0x3F,0xD4,0xE9,0x24,0x3F,0xD9,0x88,0x35,0x3F,0xFD,0x98,0x7C,0x3F,0xF8,0x7A,0x78,0x3F,0x20,0x94,0x1B,0x3F,0xB2,0xC0,0xB1,0x3C,0x2D,0x97,0xC6,0x3D,0xD0,0x9F,0x9C,0x3D,0x75,0xB5,0xA7,0x3D,0x36,0x0A,0x9C,0x3D,0xF1,0x9E,0x0A,0x3D,0x71,0x6D,0xA4,0x3D,0x61,0x4D,0x14,0x3B,0x1E,0x87,0xB7,0x3D,0x19,0xE7,0xB2,0x3D,0xA9,0x74,0xA8,0x3D,0xF4,0xAA,0x8D,0x3D,0x14,0x73,0xAD,0x3D,0x59,0xEA,0xBE,0x3C,0xFC,0xD6,0x95,0x3D,0x56,0x49,0x89,0x3D,0x77,0x72,0x5D,0x3D,0xB0,0x80,0x2F,0x3D,0x4E,0x3F,0x67,0x3D,0xAD,0xE8,0xAC,0x3D,0x16,0x54,0x62,0x3C,0x4F,0xA1,0x81,0x3D,0x65,0xAA,0x4B,0x3D,0xC3,0x28,0xC3,0x3D,
	0x54,0xC7,0x40,0x3F,0xFD,0x28,0x7D,0x3F,0xFF,0x8C,0x7E,0x3F,0x21,0x8F,0x0B,0x3F,0x42,0xDA,0x6F,0x3F,0x00,0x93,0x7F,0x3F,0x00,0x8C,0x7F,0x3F,0xCC,0xE7,0x08,0x3F,0xAD,0xB1,0xEF,0x3E,0xFC,0x66,0x7C,0x3F,0xFA,0x00,0x7A,0x3F,0x3E,0x3E,0x53,0x3E,0x78,0x9A,0x5E,0x3D,0x95,0x21,0xC8,0x3D,0xC1,0x78,0x40,0x3D,0xDE,0xBF,0xAA,0x3D,0xF0,0x93,0x3C,0x3D,0xBA,0xF8,0x39,0x3D,0x50,0xE2,0x35,0x3D,0x59,0x4D,0x0C,0x3C,0x5F,0xBF,0x78,0x3D,0x4E,0x21,0x81,0x3D,0x39,0xBF,0x52,0x3D,0x87,0x70,0x86,0x3D,0xAA,0x38,0x61,0x3F,0xD4,0x33,0xA0,0x3A,0x51,0x32,0x36,0x3E,0x91,0xD1,0x80,0x3E,0x4E,0xDF,0xED,0x3E,0xEC,0x65,0x16,0x3E,0xBA,0x5C,0x1F,0x3E,0xF2,0xD4,0xDE,0x3B,0xF9,0x20,0x79,0x3D,0xE5,0xEB,0xB1,0x3D,0xB0,0x88,0x2F,0x3D,0x9E,0x90,0x9D,0x3C,0x32,0x0E,0x68,0x3F,0xE9,0xC5,0x68,0x3F,0xE1,0xA1,0x60,0x3F,0x52,0x5A,0x08,0x3F,0x35,0x75,0x44,0x3F,0xF0,0x29,0x70,0x3F,0xD4,0x2E,0x54,0x3F,0x8B,0xE1,0xE6,0x3E,0x8E,0x36,0x65,0x3F,0xFF,0xDB,0x7E,0x3F,0xFF,0x19,0x7F,0x3F,0x20,0x75,0xD3,0x3E,0x04,0xF1,0x53,0x3F,0xF9,0x0C,0x79,0x3F,0xFD,0xB7,0x7C,0x3F,0xC1,0x25,0xF4,0x3E,0x00,0x39,0xCF,0x3E,0xFF,0x68,0x7F,0x3F,0xFB,0x7D,0x7B,0x3F,0x1B,0x18,0x38,0x3E,0x4F,0x4E,0xB5,0x3D,0x7C,0xAA,0x62,0x3D,0x1B,0xAE,0x81,0x3D,0xAC,0x28,0xAC,0x3D,0xA8,0x6C,0x61,0x3F,0x67,0x4D,0x1A,0x3B,0x14,0xBF,0x2C,0x3D,0xB0,0xE1,0x82,0x3E,
	0xF0,0x66,0xE2,0x3E,0x88,0x20,0x07,0x3E,0x96,0x0C,0xC8,0x3D,0x36,0x15,0xA2,0x3B
},

/* input data and other run-time fields*/

{0}

}; /* done */
