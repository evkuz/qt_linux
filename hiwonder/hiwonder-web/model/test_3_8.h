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
{{0.001053f,-0.057895f},{0.002222f,-0.091111f}},

/* Output variable normalization parameters */
{{0.010526f,-0.105263f},{0.014925f,0.000000f},{0.016949f,-1.881356f},{0.005556f,0.000000f}},

/* number of input MF's per variable*/ 
{15,15},
/* input MF paramteres */ 
{
	{triangular,0.058684f,0,0.116552f,0.116552f},{triangular,0.178684f,0,0.121778f,0.121778f},{triangular,0.268947f,0,0.057492f,0.057492f},{triangular,0.336316f,0,0.076307f,0.076307f},{triangular,0.419210f,0,0.088328f,0.088329f},{triangular,0.478421f,0,0.029269f,0.029269f},{triangular,0.507368f,0,0.028223f,0.028223f},{triangular,0.551579f,0,0.059582f,0.059583f},{triangular,0.613421f,0,0.063241f,0.063241f},{triangular,0.660000f,0,0.029269f,0.029269f},{triangular,0.691053f,0,0.032404f,0.032405f},{triangular,0.742105f,0,0.068990f,0.068990f},{triangular,0.814210f,0,0.074217f,0.074217f},{triangular,0.907895f,0,0.111848f,0.111848f},{triangular,0.982105f,0,0.035540f,0.035540f},
	{triangular,0.016667f,0,0.033318f,0.035539f},{triangular,0.107302f,0,0.145646f,0.038712f},{triangular,0.187937f,0,0.122483f,0.232907f},{triangular,0.318889f,0,0.028875f,0.017769f},{triangular,0.348333f,0,0.041092f,0.074410f},{triangular,0.395556f,0,0.019991f,0.026654f},{triangular,0.418519f,0,0.019250f,0.027395f},{triangular,0.437778f,0,0.011106f,0.015548f},{triangular,0.460000f,0,0.028875f,0.055530f},{triangular,0.494444f,0,0.013327f,0.028875f},{triangular,0.533333f,0,0.048866f,0.086626f},{triangular,0.613333f,0,0.073299f,0.031097f},{triangular,0.658889f,0,0.059972f,0.091069f},{triangular,0.744444f,0,0.079963f,0.159926f},{triangular,0.909778f,0,0.170587f,0.180360f}
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
225, /*number of rules */

/* index */
{0}, /* empty */
/* database */
{
	0xE6,0x36,0xE6,0x3E,0xB2,0xC7,0x31,0x3F,0x80,0x00,0x80,0x3D,0xCB,0x46,0xCB,0x3E,0x9C,0xBC,0x9B,0x3E,0x9B,0xF8,0x1A,0x3F,0xB7,0xE0,0xB6,0x3D,0xF8,0x94,0x77,0x3E,0x78,0x92,0x34,0x3F,0xDB,0xB2,0x53,0x3F,0x7B,0x11,0x5E,0x3F,0xD2,0x90,0x4F,0x3F,0xB9,0xE8,0x3B,0x3F,0x65,0x19,0x36,0x3F,0x7D,0x06,0x3C,0x3F,0xFC,0xE8,0x3D,0x3F,0x4F,0x56,0x5B,0x3F,0x8E,0x88,0x73,0x3F,0xE9,0xF8,0x7F,0x3F,0xD5,0x27,0x4C,0x3F,0x67,0x9F,0x1A,0x3F,0x62,0xF3,0x7F,0x3F,0xE1,0xF9,0x7F,0x3F,0xFD,0xD3,0x3C,0x3F,0x6C,0xBB,0x0D,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x80,0x3F,0x05,0x57,0x38,0x3F,0xC4,0x0A,0x44,0x3F,0xD6,0xE5,0x55,0x3F,0x97,0x34,0x97,0x3E,0xEC,0x0A,0x6C,0x3F,0xAD,0x0D,0xD9,0x3E,0xB1,0x03,0x39,0x3F,0xCA,0x32,0x25,0x3F,0x16,0x6E,0x34,0x3F,0xF9,0x42,0x79,0x3F,0xCA,0x47,0x4A,0x3F,0xCE,0xC0,0xCD,0x3D,0x88,0x63,0x08,0x3F,0xD2,0x4F,0x52,0x3F,0x86,0x19,0x06,0x3F,0xA4,0x92,0xA3,0x3E,0xB7,0x6A,0x37,0x3F,0x93,0x40,0x93,0x3D,0xAF,0x2C,0x2F,0x3E,0x94,0xB1,0x13,0x3F,0xD9,0xCF,0x58,0x3F,0xA3,0x10,0xA3,0x3E,0xAE,0x11,0x2E,0x3F,0xB6,0xB8,0xB5,0x3D,0x93,0xA0,0x12,0x3D,0x00,0x00,0x80,0x3F,0xAA,0xB5,0x3D,0x3F,0x3C,0x4A,0x48,0x3F,0xB3,0x7C,0x7F,0x3F,0x65,0xFB,0x97,0x3E,0x6C,0x57,0xC3,0x3E,0xC0,0x39,0x16,0x3F,0xE4,0x99,0xF9,0x38,0xBB,0x6E,0xBB,0x3E,0xCB,0xE0,0x4A,0x3D,0xB6,0xB8,0xB5,0x3D,0xFA,0x14,0x7A,0x3F,
	0x00,0x00,0x80,0x3F,0x5E,0x80,0x16,0x3F,0xB2,0x08,0x26,0x3F,0xDE,0xFE,0x14,0x3F,0x97,0xF1,0x23,0x3F,0x32,0x22,0x3A,0x3F,0x50,0xB3,0x53,0x3F,0x5E,0xB1,0x3F,0x3F,0x93,0xF7,0x48,0x3F,0xDB,0x65,0x45,0x3F,0x19,0x23,0x3E,0x3F,0x1D,0x75,0x5D,0x3F,0x60,0x65,0xC4,0x3E,0x8A,0x7E,0x86,0x3E,0x57,0xC1,0x11,0x3F,0xDA,0xFE,0x7F,0x3F,0x26,0xA1,0xC8,0x3E,0x15,0x8A,0x56,0x3F,0xAF,0x36,0x2A,0x3F,0xDA,0xFE,0x7F,0x3F,0x6C,0x5E,0xFB,0x3E,0x2E,0x68,0x58,0x3F,0x5F,0xFF,0x7F,0x3F,0x01,0x98,0x47,0x3F,0x51,0x8A,0xC2,0x3E,0x0E,0x0E,0xE2,0x3E,0x67,0x4F,0xB8,0x3E,0xEB,0xD0,0x29,0x3F,0x9E,0x87,0xB1,0x3E,0x5D,0xA2,0x26,0x3F,0x46,0x73,0x2A,0x3F,0x7D,0x6A,0x44,0x3F,0xB1,0x22,0x31,0x3F,0x86,0xA0,0x05,0x3E,0xA9,0xB2,0xA8,0x3E,0x93,0x50,0x13,0x3D,0x73,0x24,0x07,0x3E,0x03,0x6B,0x9B,0x3E,0x8E,0xA2,0x02,0x3E,0x9B,0xCF,0x76,0x3F,0x7B,0x5D,0x11,0x3E,0x35,0xD4,0x7B,0x3E,0xC8,0x91,0x9E,0x3E,0x17,0xA2,0x79,0x3F,0xDD,0x69,0x36,0x3E,0x15,0x15,0x68,0x3E,0x52,0xC4,0x0A,0x3E,0x00,0x00,0x80,0x3F,0x5F,0xB3,0x5D,0x3E,0x47,0x65,0xA0,0x3E,0x4A,0x71,0x84,0x3E,0x74,0x3B,0x6E,0x3F,0x96,0x34,0xF7,0x3E,0x38,0x87,0x05,0x3F,0x21,0xF3,0xDC,0x3E,0x00,0x00,0x00,0x00,0xB9,0x40,0xB9,0x3C,0xC3,0xEA,0xC2,0x3E,0x9B,0xAC,0x9A,0x3E,0xBD,0xB4,0x3C,0x3E,0x00,0x00,0x80,0x3F,0x38,0x9A,0x1B,0x3F,0x2C,0xC6,0x26,0x3F,0x21,0x86,0x03,0x3F,
	0xDC,0xBE,0x2B,0x3F,0xBE,0xBB,0x3B,0x3F,0x46,0xEF,0x4E,0x3F,0xA5,0xEA,0x23,0x3F,0x96,0xE0,0x95,0x3C,0xB7,0x48,0xB7,0x3E,0xD6,0x48,0xD6,0x3E,0xAA,0x36,0x2A,0x3F,0x9A,0xF2,0xD9,0x3E,0x7D,0xEC,0x18,0x3E,0xBC,0x08,0xDE,0x3E,0x00,0x00,0x80,0x3F,0x58,0xF2,0x96,0x3E,0x77,0xF2,0x85,0x3E,0x44,0x74,0xCD,0x3E,0x00,0x00,0x80,0x3F,0x4A,0xEF,0xC2,0x3E,0xBC,0x2D,0xA1,0x3E,0xB2,0x4D,0x83,0x3E,0x22,0x5B,0x33,0x3F,0x09,0xCC,0x87,0x3E,0x5A,0x0E,0xA1,0x3E,0x81,0xEB,0x42,0x3E,0x08,0x15,0x29,0x3F,0x86,0x29,0x18,0x3E,0x4F,0x7D,0xD3,0x3E,0x85,0xE7,0xAB,0x3E,0x72,0x20,0x2F,0x3F,0xE0,0x69,0x45,0x3E,0xE4,0x24,0x62,0x3E,0xE0,0x77,0xAA,0x3E,0xF7,0x14,0xBA,0x3E,0x25,0xAB,0x45,0x3E,0xE2,0xD6,0xA5,0x3E,0x77,0xD1,0x75,0x3E,0x86,0x89,0x6D,0x3F,0xE5,0x3D,0xC0,0x3D,0xA5,0x44,0x98,0x3E,0x98,0xA4,0xA1,0x3E,0xAD,0xB2,0x73,0x3F,0x92,0x7C,0x12,0x3E,0xB4,0xFF,0x33,0x3F,0x98,0x86,0x17,0x3F,0xF9,0xE0,0xF8,0x3C,0xED,0x56,0xED,0x3E,0xBB,0x58,0xBB,0x3E,0xF1,0xE0,0x70,0x3E,0x80,0x04,0x00,0x3F,0x91,0xE0,0x10,0x3F,0xBB,0xF0,0x3A,0x3F,0xF8,0x3A,0x78,0x3F,0xC3,0xE8,0x42,0x3F,0xB7,0x2F,0x37,0x3F,0xCC,0x7F,0x4C,0x3F,0xBB,0xF2,0x3A,0x3F,0xB3,0x02,0x33,0x3F,0xA0,0x9A,0x9A,0x3E,0x26,0xFB,0x7F,0x3F,0xE5,0xFD,0x7F,0x3F,0xB1,0xC6,0x10,0x3F,0x2A,0xFB,0xF7,0x3E,0x26,0xFB,0x7F,0x3F,0x84,0xCB,0x60,0x3F,0x7F,0x10,0x31,0x3F,
	0xC4,0xCA,0x43,0x3F,0xF0,0x00,0x70,0x3A,0xA2,0xFD,0x21,0x3F,0xAF,0x2C,0x2F,0x3E,0xD9,0x9C,0x58,0x3F,0x9F,0xB2,0x1E,0x3F,0xAA,0xAC,0xA9,0x3E,0xB3,0x7E,0xB3,0x3E,0xEA,0xA5,0x69,0x3F,0xC1,0x7E,0x41,0x3F,0xE9,0xAC,0x68,0x3F,0xBA,0xD9,0x39,0x3F,0x45,0x1A,0xB4,0x3E,0x55,0x27,0x8A,0x3E,0x5D,0xF0,0x93,0x3E,0x22,0x27,0x31,0x3F,0x39,0x71,0xB1,0x3E,0xD0,0x08,0xE7,0x3D,0x22,0xB6,0xE9,0x3D,0x23,0x33,0x28,0x3F,0x4C,0x7B,0x0B,0x3E,0x7B,0x6B,0x95,0x3E,0xD9,0x74,0x72,0x3E,0x6A,0x0C,0x65,0x3F,0xE6,0x5D,0x26,0x3E,0x19,0xB0,0xB0,0x3E,0x07,0xC6,0x8D,0x3E,0x1D,0xA4,0xA3,0x3E,0x29,0x1D,0x25,0x3E,0x3B,0x87,0xA3,0x3E,0xE9,0x91,0xD0,0x3E,0x39,0x22,0x16,0x3F,0x51,0xD1,0xF8,0x3D,0x39,0x69,0x06,0x3E,0x3A,0x53,0x03,0x3E,0x29,0x39,0x5A,0x3F,0x00,0x00,0x00,0x00,0xF2,0x14,0x3C,0x3C,0xFF,0x19,0x51,0x3C,0x43,0xC7,0x16,0x3F,0x00,0x00,0x00,0x00,0x86,0x4F,0xE5,0x3B,0x82,0x38,0xD3,0x3D,0x29,0x99,0x3C,0x3F,0x4E,0xD5,0x80,0x3D,0xD6,0x83,0x22,0x3D,0x7B,0x3D,0x2E,0x3D,0x31,0xEE,0x96,0x3D,0xBC,0x3B,0x00,0x3F,0x0B,0xA6,0x61,0x3F,0x71,0xAC,0x57,0x3F,0x5D,0xA6,0xB9,0x3E,0xE4,0x36,0x09,0x3F,0xC8,0x54,0x05,0x3F,0xAA,0x57,0x49,0x3F,0x0D,0xCB,0x46,0x3F,0x98,0xF6,0xD8,0x3E,0x89,0xC8,0x05,0x3F,0x52,0xE3,0xDA,0x3D,0x1F,0x6C,0x3E,0x3F,0x67,0x9F,0xF8,0x3E,0xDA,0x2F,0x3C,0x3D,0xB8,0x4A,0x24,0x3E,0x4D,0xCB,0xB0,0x3D,
	0xF5,0x9E,0x4D,0x3E,0xCD,0x83,0x37,0x3C,0x05,0xDE,0xAE,0x3E,0xFD,0xAF,0xF7,0x3E,0x05,0x97,0x9E,0x3C,0x57,0x42,0xBD,0x3D,0x28,0xB4,0x74,0x3B,0x1B,0xCA,0x00,0x3D,0x7F,0xC4,0xBF,0x3E,0x6E,0x0A,0xF0,0x3D,0x4F,0x8F,0xFE,0x3D,0x4F,0xB4,0xD8,0x3E,0x2A,0x9A,0x80,0x3E,0x00,0x00,0x00,0x00,0x5A,0x98,0xDE,0x3D,0x0A,0x20,0x6B,0x3E,0x01,0x0F,0x31,0x3E,0x45,0xFE,0x6A,0x3D,0x2A,0xDE,0x89,0x3C,0x93,0xA9,0x02,0x3F,0x00,0x00,0x00,0x00,0x56,0xC6,0x5B,0x3E,0xFB,0xA7,0x89,0x3D,0x00,0x00,0x80,0x3F,0x23,0x84,0x50,0x3E,0x06,0x59,0xAF,0x3E,0x14,0xB4,0x8D,0x3D,0xDC,0xFC,0x32,0x3F,0x49,0xF6,0xAE,0x3D,0xC0,0x7F,0x8C,0x3D,0x56,0xEA,0xBB,0x3D,0xBF,0x94,0xBE,0x3D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC9,0xAD,0xAB,0x3C,0x80,0x72,0x1B,0x3F,0xBE,0x3B,0x91,0x3D,0x00,0x00,0x00,0x00,0x1B,0x61,0xF9,0x3D,0x4A,0xEC,0x3E,0x3F,0x25,0x22,0x8B,0x3D,0xAC,0x10,0x2C,0x3D,0xE8,0x66,0x81,0x3C,0x1F,0x36,0x85,0x3D,0xE0,0xF1,0x7F,0x3F,0x26,0x52,0x2C,0x3F,0xE8,0xFE,0x7F,0x3F,0xCC,0xBD,0x15,0x3F,0x6A,0xF8,0x7F,0x3F,0x65,0xF1,0x1C,0x3F,0xDA,0xED,0x6F,0x3F,0x56,0x11,0x2F,0x3F,0x10,0x8F,0xFB,0x3E,0x00,0x00,0x00,0x00,0x77,0x7F,0x9F,0x3E,0x97,0x5D,0x4A,0x3E,0x1D,0x1F,0x04,0x3F,0x00,0x00,0x00,0x00,0xE7,0x39,0x91,0x3E,0x47,0x30,0x01,0x3F,0x55,0x01,0xDD,0x3E,0x00,0x00,0x00,0x00,0x09,0x49,0x86,0x3E,0xBA,0x9B,0x0F,0x3F,
	0xA1,0xE4,0xA0,0x3D,0x1C,0xDA,0x81,0x3D,0x14,0x17,0xAD,0x3D,0x4C,0x9A,0xB2,0x3D,0xD6,0x73,0xA2,0x3D,0x71,0x8D,0xA3,0x3C,0x3D,0xDE,0xA2,0x3D,0xC7,0x08,0x47,0x3D,0x8C,0x30,0x8C,0x3D,0xB8,0xA5,0x6A,0x3D,0x20,0xB7,0xB9,0x3C,0x98,0xAD,0x4A,0x3D,0x30,0xB0,0x85,0x3D,0x5C,0x24,0x8F,0x3D,0x22,0x9B,0x8B,0x3C,0x8E,0x82,0xB9,0x3E,0x07,0x7F,0x1C,0x3E,0xD7,0x5A,0xA3,0x3D,0x46,0xD4,0x8F,0x3D,0xDA,0x0F,0xB9,0x3E,0xCC,0x62,0x5D,0x3E,0x7D,0x14,0xB0,0x3D,0x64,0x18,0xE7,0x3D,0x54,0x64,0xEF,0x3D,0x01,0xE7,0x9A,0x3D,0x94,0x88,0x13,0x3D,0x7E,0x65,0xB1,0x3D,0xE8,0x83,0xB4,0x3C,0x98,0xA9,0xCA,0x3D,0x8A,0xBA,0x6F,0x3D,0x66,0xBD,0x18,0x3D,0xFB,0x7E,0x94,0x3D,0x43,0xB2,0x29,0x3D,0x04,0x94,0xD0,0x3C,0xB4,0x93,0x00,0x3D,0xCF,0x03,0x9C,0x3C,0xA7,0x10,0xA7,0x3D,0xED,0x10,0xED,0x3C,0x7D,0x25,0x30,0x3D,0xC8,0xA0,0xC7,0x3D,0x00,0x00,0x80,0x3F,0xB1,0x4A,0x46,0x3D,0x6F,0xE6,0x46,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3F,0x28,0x88,0x91,0x3E,0x88,0xAE,0x5F,0x3F,0x00,0x00,0x00,0x00,0xA1,0x13,0x00,0x3F,0x00,0x00,0x00,0x00,0x97,0x08,0xE1,0x3D,0x68,0x76,0xD2,0x3E,0x28,0xA0,0x0E,0x3F,0x00,0x00,0x00,0x00,0x76,0x7A,0x93,0x3E,0xC4,0x4F,0x29,0x3F,0x13,0x3D,0x0A,0x3F,0x00,0x00,0x00,0x00,0x4B,0xD7,0x5E,0x3D,0x40,0xB3,0x0B,0x3F,0x26,0xBF,0x3F,0x3D,0x14,0x0F,0x2D,0x3D,0x35,0xF2,0x1A,0x3D,0x4C,0xD7,0xE5,0x3C,
	0x8F,0x74,0x8E,0x3D,0x00,0x91,0xFF,0x3C,0x2E,0xE6,0x93,0x3D,0xBD,0x9B,0x09,0x3D,0x7B,0xAA,0x61,0x3D,0x64,0xAA,0xCA,0x3C,0x8A,0xCD,0x3C,0x3A,0xC0,0xED,0x72,0x3C,0x4D,0xBE,0x64,0x3E,0x00,0x00,0x00,0x00,0xD9,0xC4,0xAC,0x3C,0xC7,0x27,0xD1,0x3E,0x56,0x2D,0x1B,0x3E,0x00,0x00,0x00,0x00,0xFB,0xA1,0xB5,0x3D,0xD6,0x37,0x15,0x3F,0x20,0xB0,0x40,0x3D,0x00,0x00,0x00,0x00,0x06,0x40,0x28,0x3E,0x9B,0x2D,0x97,0x3D,0xAA,0x80,0x29,0x3C,0xEE,0x6B,0xBA,0x3D,0x1A,0xBB,0xB3,0x3D,0x0C,0xA4,0x58,0x3D,0x21,0x8F,0xBA,0x3D,0xC2,0x97,0x8E,0x3D,0x1E,0xE4,0xEA,0x3C,0xBB,0x23,0x88,0x3C,0x59,0x67,0xF2,0x3A,0xD7,0xF7,0xA3,0x3D,0x27,0x5A,0x0D,0x3C,0xA4,0x25,0x57,0x3D,0x24,0x52,0x0A,0x3D,0xC4,0x64,0xC3,0x3D,0x50,0x67,0xE9,0x3C,0x52,0xBE,0xB7,0x3D,0x00,0x00,0x80,0x3F,0x0E,0x35,0x61,0x3F,0x34,0x3C,0x1E,0x3F,0xA1,0x72,0xE8,0x3E,0xCC,0xEC,0x6A,0x3E,0x8E,0x01,0x80,0x3E,0xC9,0xFD,0x3C,0x3F,0x9E,0xAC,0xF3,0x3E,0x61,0x34,0x56,0x3E,0xB3,0xDD,0xFF,0x3E,0x92,0x66,0x0F,0x3F,0x43,0x30,0x71,0x3F,0xE8,0xFE,0xB5,0x3E,0x85,0x7B,0x40,0x3E,0x3E,0x4A,0x4B,0x3E,0xE1,0x3E,0xD0,0x3E,0x00,0x32,0xD8,0x3D,0x57,0x7F,0xF7,0x3E,0x52,0x90,0x64,0x3E,0x2E,0x12,0x9D,0x3E,0x60,0x3D,0x13,0x3D,0x93,0xC0,0x12,0x3C,0x66,0xF5,0x18,0x3D,0xA8,0x74,0xA7,0x3D,0xFD,0x46,0x16,0x3D,0x37,0x42,0x1D,0x3D,0x17,0xAC,0x63,0x3D,0x25,0xB7,0x3E,0x3D,
	0x38,0x97,0xD1,0x3C,0x30,0x9A,0x96,0x3C,0x96,0x15,0x49,0x3D,0x33,0x12,0x99,0x3D,0xAB,0xCD,0x8F,0x3E,0x00,0x00,0x00,0x00,0x2B,0x29,0x65,0x3D,0xD1,0x95,0x1A,0x3F,0xED,0x71,0x24,0x3C,0x29,0x75,0x21,0x3E,0x9C,0x64,0x1F,0x3D,0xA6,0x49,0x14,0x3F,0x00,0x00,0x00,0x00,0xB1,0x0E,0xBF,0x3E,0xAE,0xDF,0x4B,0x3E,0x97,0x42,0x3E,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x05,0x15,0x3F,0xA3,0xBD,0xDA,0x3C,0xB3,0x86,0x57,0x3B,0xE7,0x58,0x4A,0x3B,0x83,0xF8,0x63,0x3E,0x79,0x9A,0x5F,0x3A,0x17,0x9B,0xB0,0x3D,0x69,0x2D,0x1C,0x3D,0x37,0xE7,0x4F,0x3B,0x6C,0xCD,0x9E,0x3B,0xDD,0x57,0xA9,0x3D,0xEC,0xF3,0xB8,0x3B,0x75,0xCA,0x5A,0x3D,0xC6,0xFE,0x7F,0x3F,0xC0,0x5B,0x07,0x3F,0x75,0x38,0x57,0x3F,0x56,0x99,0x84,0x3E,0x47,0x10,0x79,0x3F,0xC5,0xE9,0x45,0x3F,0xAB,0xEE,0x78,0x3F,0x4E,0x7E,0x0F,0x3F,0xB2,0xA0,0x65,0x3E,0x00,0x00,0x80,0x3F,0x6B,0x30,0xD8,0x3E,0x8D,0xD4,0xE5,0x3E,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3F,0x11,0x56,0x50,0x3F,0x01,0x0E,0x35,0x3E,0x4D,0x89,0x7F,0x3D,0xAE,0x0B,0x1F,0x3E,0x0C,0xA9,0x13,0x3F,0x20,0xBD,0x3E,0x3F,0x51,0x65,0xB8,0x3E,0xE9,0x0C,0xB0,0x3D,0xA4,0x17,0x5F,0x3E,0xD7,0x08,0x3F,0x3F,0x94,0x7C,0x48,0x3E,0x42,0xBA,0x19,0x3D,0xA9,0x4A,0x02,0x3E,0xDD,0xE7,0x45,0x3F,0x5A,0xF2,0x3F,0x3D,0x47,0xD7,0x60,0x3D,0x63,0x4D,0x16,0x3C,0x14,0x27,0xAD,0x3B,
	0x83,0xCD,0x35,0x3C,0x89,0xA0,0x08,0x3D,0xBC,0xE0,0x3B,0x3C,0x3A,0x82,0x20,0x3D,0x6E,0x99,0x34,0x3C,0x1A,0xE2,0xBF,0x3D,0x38,0xB1,0x9D,0x3D,0x0B,0x2B,0x3D,0x3F,0x4A,0xD5,0x47,0x3D,0x58,0xD0,0x7A,0x3E,0x93,0x50,0x28,0x3E,0xA6,0x8B,0xE9,0x3E,0x9C,0x7D,0xCF,0x3C,0x1F,0xD3,0xB8,0x3D,0x03,0x5F,0x9C,0x3D,0x5E,0x0D,0x11,0x3C,0xC8,0xB3,0x94,0x3B,0x1F,0x54,0x6B,0x3C,0xF5,0x22,0x8E,0x3D,0x83,0x55,0x36,0x3D,0xFC,0x5F,0xC8,0x3D,0x51,0x05,0x04,0x3D,0x43,0x02,0x29,0x3D,0x23,0xEB,0xBB,0x3D,0x46,0x42,0xAC,0x3D,0x63,0xB2,0x49,0x3D,0xC6,0x8B,0x12,0x3D,0x28,0xE6,0x8D,0x3D,0xE3,0xE9,0x6D,0x3F,0xDC,0x1D,0x2D,0x3F,0xD2,0x65,0x9B,0x3E,0x4D,0xF9,0xF7,0x3E,0x8B,0x5D,0x28,0x3F,0x7A,0x86,0x15,0x3F,0x91,0xE8,0x00,0x3F,0x18,0x59,0x04,0x3E,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3F,0x3B,0x2B,0xCD,0x3E,0xB2,0x92,0x9C,0x3E,0x45,0x66,0x0A,0x3E,0xF5,0x91,0x4F,0x3E,0xB5,0xC6,0x20,0x3F,0xDC,0x15,0x8F,0x3E,0x78,0x2B,0x52,0x3D,0x00,0x00,0x80,0x3F,0xAA,0x8C,0x46,0x3F,0x0C,0xC0,0x11,0x3E,0x08,0xE6,0x8A,0x3E,0x6F,0xAD,0x5D,0x3D,0xED,0x34,0x5C,0x3C,0x35,0xCB,0x32,0x3F,0xA8,0x74,0xFD,0x3E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8A,0x7F,0x71,0x3F,0x66,0x8F,0x7F,0x3D,0xDF,0x97,0xAB,0x3D,0x28,0x1A,0x0E,0x3B,0x62,0x3E,0xC8,0x3D,0x41,0x1A,0x27,0x3C,0xC9,0x9B,0x15,0x3D,0xE8,0x4A,0x81,0x3D,0x2F,0x9A,0x95,0x3B,
	0x55,0x17,0x6E,0x3D,0x78,0xDA,0x5D,0x3D,0x48,0x4A,0xAE,0x3D,0x2C,0xE7,0x45,0x3B,0x48,0xAC,0x16,0x3E,0x07,0x5D,0x01,0x3E,0xB6,0x3B,0x0E,0x3E,0x31,0x08,0xFB,0x3E,0x68,0xEB,0x37,0x3E,0xDC,0xB9,0x15,0x3E,0x24,0x09,0x36,0x3E,0x06,0xCF,0x33,0x3F,0xFF,0x69,0x13,0x3E,0xDC,0x38,0x87,0x3E,0x20,0x55,0x98,0x3E,0x42,0x2D,0x89,0x3E,0xA3,0xA0,0x22,0x3C,0x31,0xBE,0x97,0x3D,0xEF,0x33,0xBB,0x3D,0xF1,0x06,0x8A,0x3D,0x11,0x0C,0x5E,0x3D,0x5D,0xFD,0x0F,0x3D,0xED,0x2B,0x39,0x3D,0xDE,0x5F,0xAA,0x3D,0x52,0x65,0x85,0x3D,0xDA,0xAF,0xA6,0x3D,0x08,0x77,0xA1,0x3C,0xE2,0xD3,0xAE,0x3C,0x00,0x00,0x80,0x3F,0xEA,0x69,0x1F,0x3F,0x5E,0x40,0x20,0x3F,0x32,0x70,0xC2,0x3E,0x8F,0x83,0x05,0x3F,0xB5,0xD9,0x4F,0x3F,0x52,0x55,0x7A,0x3F,0xB9,0x48,0x65,0x3E,0xCF,0xD4,0xA4,0x3C,0x33,0x85,0xB6,0x3E,0xC8,0xD5,0x47,0x3F,0xD7,0xC4,0xAB,0x3E,0xB9,0x70,0xB9,0x3B,0x81,0xF1,0x71,0x3F,0xBF,0xBE,0x35,0x3F,0x74,0x10,0xAE,0x3E,0x9A,0x88,0x99,0x3D,0xCF,0x03,0x9C,0x3C,0xC8,0x3B,0x14,0x3D,0x67,0x8A,0xCD,0x3C,0xC8,0xB3,0x14,0x3C,0x9B,0xAC,0x9A,0x3D,0x42,0x97,0x5B,0x3D,0xA3,0xED,0x55,0x3C,0x15,0xE7,0x2E,0x3B,0x21,0x27,0xBA,0x3C,0x20,0xDA,0x85,0x3C,0x96,0x44,0x96,0x3D,0x57,0x49,0x8A,0x3D,0xEA,0xA6,0x03,0x3C,0x11,0xFF,0xA9,0x3D,0x7A,0x89,0xAC,0x3D,0xD7,0xF0,0x56,0x3D,0xC8,0x2D,0xFB,0x3C,0xFD,0x02,0x96,0x3D,0xEC,0x00,0xEC,0x39,
	0xFC,0x83,0x31,0x3E,0xCA,0xA7,0xD7,0x3D,0xA4,0x43,0x7A,0x3E,0x59,0xCB,0x32,0x3F,0x61,0xC2,0x76,0x3C,0x46,0xE1,0xBB,0x3E,0x95,0x54,0x8D,0x3E,0x58,0x44,0xF4,0x3E,0xA5,0xC8,0x37,0x3C,0x62,0x1D,0x7D,0x3E,0x4D,0xAF,0xA6,0x3C,0x00,0x00,0x00,0x00,0x1B,0x67,0x34,0x3D,0x74,0x5A,0xDA,0x3B,0xC1,0xA7,0x8D,0x3D,0x6A,0xDD,0x1C,0x3D,0x20,0x53,0xB9,0x3D,0x2D,0x1F,0x46,0x3D,0x81,0x3D,0x34,0x3D,0xDC,0x1F,0xA9,0x3D,0xD4,0x47,0xA0,0x3D,0x71,0x25,0xA4,0x3D,0xCB,0x00,0x4B,0x3C,0x10,0xF7,0x28,0x3D,0x9E,0xA8,0x2E,0x3F,0xED,0x4E,0x2B,0x3F,0xB7,0xF6,0x7F,0x3F,0x43,0x49,0xDD,0x3E,0xCB,0x09,0x69,0x3F,0x1A,0xA6,0x25,0x3F,0xB3,0xDD,0x54,0x3F,0x16,0xA7,0xBA,0x3E,0x3E,0x77,0xD7,0x3C,0x59,0xEA,0xBE,0x3D,0x48,0x0A,0x2E,0x3D,0x49,0xD7,0xE2,0x3C,0xEB,0xFE,0x83,0x3D,0x23,0x3B,0xBC,0x3D,0xCA,0x5D,0x7D,0x3D,0xCB,0x1D,0x7E,0x3D,0xC8,0xA4,0xC7,0x3D,0xF1,0x96,0x8A,0x3D,0x26,0xC7,0xBF,0x3D,0xC2,0x60,0xC1,0x3D,0xAB,0x24,0xAB,0x3D,0x34,0xCA,0x99,0x3D,0x9B,0xAD,0xCD,0x3C,0xEF,0xA0,0xEE,0x3C,0xF5,0xB3,0xC1,0x3D,0xA7,0x4C,0xA7,0x3D,0x57,0x31,0x8A,0x3D,0x83,0x90,0x82,0x3C,0x21,0xF7,0xB9,0x3D,0x82,0x54,0x82,0x3D,0xBB,0x27,0x88,0x3D,0x2D,0x47,0xC6,0x3D,0xAF,0x3D,0xE2,0x3C,0xC2,0x83,0x8E,0x3C,0x31,0xF2,0x96,0x3D,0x8E,0xA5,0x40,0x3D,0x49,0x40,0x42,0x3E,0xE9,0x14,0xB3,0x3E,0x58,0x5E,0x7F,0x3E,0x57,0xDC,0x1B,0x3F,
	0x55,0x43,0x86,0x3D,0x5C,0xE8,0xCA,0x3E,0xBE,0x0B,0xF9,0x3D,0x00,0x00,0x00,0x00,0xD2,0x10,0x9E,0x3E,0x49,0xF9,0x64,0x3E,0xF4,0xC4,0xA4,0x3E,0xCD,0xA7,0x20,0x3E,0x20,0x35,0xA6,0x3E,0x0F,0xEC,0x82,0x3E,0xC0,0xC5,0x0A,0x3E,0x05,0x6E,0xE3,0x3E,0x56,0x39,0x2F,0x3E,0x64,0x4F,0x91,0x3E,0x36,0xF8,0x5F,0x3D,0x08,0x11,0xFB,0x3D,0xA5,0x3C,0xA5,0x3D,0x20,0x4A,0x06,0x3D,0xD2,0x63,0x1E,0x3D,0xEA,0x20,0x6A,0x3D,0x91,0x50,0x91,0x3C,0x43,0x9A,0xA9,0x3D,0xC5,0xA7,0x91,0x3D,0xE9,0xE0,0xE8,0x3C,0x92,0x6B,0x59,0x3F,0xD1,0xFA,0x2F,0x3F,0xBB,0xB1,0x39,0x3F,0x7C,0x2F,0x6F,0x3E,0x1D,0xC7,0xB6,0x3C,0x4F,0xAA,0xB5,0x3D,0x24,0x07,0x3D,0x3D,0x8A,0x00,0x0A,0x3C,0x86,0xF8,0xC0,0x3E,0x4B,0xD8,0xA5,0x3E,0x82,0x93,0x9E,0x3E,0x3E,0xFB,0x2D,0x3E,0xD8,0x40,0x58,0x3D,0x72,0xE5,0x24,0x3D,0xEE,0xB3,0xBA,0x3B,0x53,0xAD,0x05,0x3C,0x1D,0xF4,0xE9,0x3C,0xD6,0xD3,0xA2,0x3C,0x10,0xD7,0xA9,0x3D,0xC9,0xA0,0xC8,0x3D,0x2D,0x14,0x79,0x3C,0xF2,0x26,0x8B,0x3D,0x59,0xBA,0xBE,0x3D,0xBB,0xE4,0xBA,0x3D,0x8C,0xB1,0xBE,0x3D,0xFA,0xAB,0x46,0x3D,0xB6,0xEC,0xB5,0x3D,0x23,0x84,0x6F,0x3D,0xC4,0x2D,0xF7,0x3C,0x56,0xA1,0x88,0x3D,0x15,0xBB,0xAE,0x3D,0x60,0xAA,0xC6,0x3D,0xEA,0xB3,0xB6,0x3D,0x8D,0xBA,0x72,0x3D,0x6D,0x4D,0xA0,0x3C,0xFF,0x93,0x4B,0x3D,0x77,0xCD,0xA9,0x3D,0x61,0x7D,0x94,0x3D,0x2E,0x4C,0x7A,0x3D,0xBB,0xB0,0xBA,0x3D,
	0x48,0x65,0x6A,0x3E,0xA9,0x1D,0x8B,0x3E,0xE8,0x9E,0xAA,0x3E,0xD8,0x58,0x92,0x3E,0xEA,0x3C,0x3D,0x3E,0xCF,0x56,0x5F,0x3E,0xD8,0x1B,0xB5,0x3E,0x1F,0x6F,0xC9,0x3E,0x73,0xFB,0xFD,0x3C,0xAC,0x04,0x9C,0x3E,0xBE,0xFF,0x47,0x3E,0x32,0x94,0x0B,0x3F,0x79,0x6D,0x2C,0x3C,0x27,0xF2,0x8C,0x3D,0xB6,0x4D,0x69,0x3D,0xF0,0xE6,0x09,0x3D,0x1B,0x67,0x34,0x3D,0xE6,0x80,0xE5,0x3B,0xC2,0x67,0x8E,0x3D,0xF7,0x0A,0x90,0x3D,0x22,0xC5,0x6B,0x3F,0xA6,0x12,0x28,0x3F,0x96,0x06,0x3C,0x3F,0xDB,0xFD,0x97,0x3E,0x91,0x80,0x10,0x3D,0x14,0x03,0xAD,0x3D,0x29,0xE4,0x75,0x3D,0x50,0xE5,0x02,0x3D,0xCA,0xCB,0x74,0x3F,0x89,0xB4,0x3A,0x3F,0x9E,0xFC,0x56,0x3F,0x56,0x3E,0x32,0x3E,0x35,0xBA,0x9B,0x3C,0xF6,0xC0,0x75,0x3D,0x05,0x67,0x1E,0x3C,0x1B,0x34,0xE7,0x39,0xCF,0x1B,0x55,0x3F,0xC5,0x21,0x5A,0x3F,0x4B,0x66,0x6D,0x3F,0xA5,0xAE,0x42,0x3E,0x00,0x00,0x80,0x3F,0xAF,0x8D,0xDF,0x3D,0x98,0x9B,0x36,0x3E,0x1C,0x6E,0xBE,0x3D,0xE5,0x2B,0x31,0x3D,0x3A,0x4A,0xA0,0x3C,0xFD,0xDF,0xC9,0x3D,0xC7,0xB8,0xC6,0x3D,0x97,0x2D,0x4A,0x3D,0x56,0x9E,0xBC,0x3D,0xC4,0x80,0x43,0x3B,0x2C,0xDA,0x11,0x3D,0xC2,0xC3,0x75,0x3F,0x13,0x91,0x58,0x3F,0x6D,0xCB,0x4C,0x3F,0x4D,0x80,0x16,0x3E,0xF8,0xC1,0x4F,0x3F,0x4D,0xC6,0x2D,0x3F,0x96,0xE7,0x25,0x3F,0x9A,0xCB,0x78,0x3D,0xDF,0x39,0x5E,0x3F,0x1D,0x3A,0x6C,0x3F,0xBA,0xBE,0x47,0x3F,0x53,0x8B,0x7E,0x3C,
	0x23,0x95,0xC7,0x3E,0x13,0x41,0x43,0x3F,0x5F,0xAF,0x42,0x3F,0xB1,0x63,0x3E,0x3D,0x3F,0xE7,0x08,0x3F,0x39,0x28,0x99,0x3E,0xB3,0x8E,0xD1,0x3E,0x96,0x86,0x6F,0x3E,0x27,0x67,0x40,0x3C,0xDE,0x37,0xAA,0x3D,0xC7,0x93,0x93,0x3D,0x21,0xD6,0x86,0x3D,0x7C,0x39,0xAF,0x3D,0xD6,0x7B,0x22,0x3D,0xB1,0x6D,0x64,0x3D,0xE1,0xB3,0x2D,0x3B,0x62,0x46,0xC8,0x3D,0x09,0x97,0xA2,0x3D,0xC1,0xA4,0xC0,0x3D,0x57,0xA5,0x09,0x3D,0x74,0xBD,0xA6,0x3C,0x80,0xDD,0x32,0x3D,0xAC,0xD5,0x5E,0x3D,0x77,0x9A,0xDD,0x3B,0xE5,0x2B,0xB1,0x3D,0x51,0x72,0x37,0x3D,0x2C,0xE7,0xC5,0x3D,0x38,0x27,0x51,0x3C,0x53,0xA9,0x85,0x3D,0xE2,0x88,0x61,0x3D,0x52,0x7D,0x85,0x3C,0xD5,0x33,0x21,0x3A,0xE3,0x48,0x62,0x3D,0xB5,0x53,0x01,0x3D,0x55,0xA7,0x6E,0x3D,0x81,0xDA,0xE6,0x3C,0x10,0x14,0x5D,0x3C,0x06,0x7F,0x1F,0x3D,0x15,0xCB,0xAE,0x3D,0x3D,0x02,0xA3,0x3D,0x2D,0x14,0x79,0x3D,0xCF,0xC0,0xCE,0x3C,0xA8,0x98,0xA7,0x3D,0x80,0x7D,0xB3,0x3C,0x33,0x67,0x4C,0x3A,0x07,0x84,0xD3,0x3C,0x39,0xE7,0x51,0x3C,0x2D,0xA2,0x13,0x3D,0x8F,0xE3,0x11,0x3F,0x0A,0x5F,0x6D,0x3D,0xBD,0xDF,0x0D,0x3F,0xA6,0x16,0xAF,0x3D,0xAD,0x0A,0x11,0x3F,0xB2,0x96,0x1E,0x3F,0x9A,0xDF,0xB9,0x3E,0xA3,0xB2,0x88,0x3D,0x00,0x00,0x80,0x3F,0x34,0x65,0xEE,0x3E,0xAE,0xFB,0x7A,0x3F,0x86,0x74,0x09,0x3E,0x00,0x00,0x80,0x3F,0x67,0x2F,0x39,0x3F,0x53,0xF4,0x30,0x3F,0x00,0x00,0x00,0x00,
	0x00,0x00,0x80,0x3F,0xE2,0xFC,0x49,0x3F,0xF4,0xDB,0x64,0x3F,0x00,0x00,0x00,0x00
},

/* input data and other run-time fields*/

{0}

}; /* done */
