#pragma once

#include "MGFTreeNode.h"
#include "../renderer/Texture.h"

#include <fstream>

class MGFTexture : public Texture2D
{
public:
	MGFTexture(const MGFTreeNode& treeNode);
	~MGFTexture();

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

