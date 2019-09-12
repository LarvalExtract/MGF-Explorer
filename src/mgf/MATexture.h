#pragma once

#include "MGFTreeNode.h"
#include "../renderer/Texture.h"

#include <fstream>

class MATexture : public Texture2D
{
public:
	MATexture(const MGFTreeNode& treeNode);

public:
	unsigned int depth;
	unsigned int mips;
	unsigned int size;
	unsigned int frames;
	unsigned int format;
	unsigned int flags;

private:
	void InitTexture(unsigned char* pixels);
	unsigned int CalculateStride();
};

class MGFTextureArray : public Texture2DArray
{

};