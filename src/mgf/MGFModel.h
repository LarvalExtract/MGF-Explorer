#pragma once

#include "MGFResourceManager.h"
#include "MGFTreeNode.h"
#include "MGFMesh.h"

#include <fstream>
#include <vector>

class MGFModel
{
public:
	MGFModel(const MGFTreeNode& node, std::ifstream& fileStream, MGFResourceManager& resourceManager);
	~MGFModel();

private:
	MGFResourceManager& resourceManager;
};