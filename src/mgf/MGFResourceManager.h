#pragma once

#include "MGFArchive.h"
#include "MGFTreeNode.h"
#include "MGFTexture.h"
#include "MGFMesh.h"

#include <unordered_map>
#include <memory>

typedef std::shared_ptr<MGFTexture> MGFTexture_ptr;
typedef std::shared_ptr<MGFMesh> MGFMesh_ptr;

class MGFResourceManager
{
public:
	MGFResourceManager();
	~MGFResourceManager();

	MGFTexture_ptr GetTexture(const std::string& path, MGFArchive& archive);
	MGFTexture_ptr GetTexture(const MGFTreeNode& node);

	//MGFMesh_ptr GetMesh(const std::string& path, MGFArchive& archive);

private:
	std::unordered_map<std::string, MGFTexture_ptr> mapTextures;
	//std::unordered_map<std::string, MGFMesh_ptr> mapMeshes;
};