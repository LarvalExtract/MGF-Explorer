#pragma once

#include "Common.h"

#pragma pack(push, 1)
struct TIF_PIC {
	char	 pic[4];
	uint32_t padding1;
	uint32_t fileLength;
	uint32_t nine;
};

struct TIF_VERSION {
	char	 ver[4];
	uint32_t length;
	uint8_t  version;
};

struct TIF_FLAGS {
	char	 flgs[4];
	uint32_t length;
	uint32_t flags;
};

struct TIF_WIDTH {
	char	 wdth[4];
	uint32_t length;
	uint32_t imageWidth;
};

struct TIF_HEIGHT {
	char	 hgt[4];
	uint32_t length;
	uint32_t imageHeight;
};

struct TIF_MIPS {
	char	 mips[4];
	uint32_t length;
	uint32_t numMips;
};

struct TIF_SIZE {
	char	 size[4];
	uint32_t length;
	uint32_t imageSize;
};

struct TIF_FRAMES {
	char	 frms[4];
	uint32_t length;
	uint32_t numFrames;
};

struct TIF_DEPTH {
	char     dpth[4];
	uint32_t length;
	uint32_t imageDepth;
};

struct TIF_BITS {
	char	 bits[4];
	uint32_t length;
};

struct MA1_TIF_HEADER
{
	MGI_HEADER  cMgi;
	TIF_PIC		cPic;
	TIF_VERSION cVersion;
	TIF_FLAGS   cFlags;
	TIF_WIDTH	cWidth;
	TIF_HEIGHT  cHeight;
	TIF_MIPS    cMips;
	TIF_SIZE    cSize;
	TIF_BITS    cBits;
};

struct MA2_TIF_HEADER
{
	MGI_HEADER  cMgi;
	TIF_PIC		cPic;
	TIF_VERSION cVersion;
	TIF_FLAGS   cFlags;
	TIF_WIDTH	cWidth;
	TIF_HEIGHT  cHeight;
	TIF_MIPS    cMips;
	TIF_SIZE    cSize;
	TIF_FRAMES  cFrames;
	TIF_DEPTH   cDepth;
	TIF_BITS    cBits;
};
#pragma pack(pop)