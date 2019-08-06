#include "MGFResourceManager.h"
#include "MGFArchive.h"

MGFResourceManager::MGFResourceManager()
{

}

MGFResourceManager::~MGFResourceManager()
{

}

MGFTexture_ptr MGFResourceManager::GetTexture(const std::string& path, MGFArchive& archive)
{
	if (mapTextures.find(path) != mapTextures.end())
	{
		return mapTextures[path];
	}
	else
	{
		MGFTexture_ptr newTexture = std::make_shared<MGFTexture>(*archive.GetNodeFromPath(path));
		mapTextures.insert(std::make_pair(path, newTexture));
		return newTexture;
	}
}

MGFTexture_ptr MGFResourceManager::GetTexture(const MGFTreeNode& node)
{
	const std::string& path = node.GetFullPath();

	if (mapTextures.find(path) != mapTextures.end())
	{
		return mapTextures[path];
	}
	else
	{
		MGFTexture_ptr newTexture = std::make_shared<MGFTexture>(node);
		mapTextures.insert(std::make_pair(path, newTexture));
		return newTexture;
	}
}

//MGFMesh_ptr MGFResourceManager::GetMesh(const std::string& path, MGFArchive& archive)
//{
//	if (mapMeshes.find(path) != mapMeshes.end())
//	{
//		return mapMeshes[path];
//	}
//	else
//	{
//		MGFMesh_ptr newMesh = std::make_shared<MGFMesh>(*archive.GetNodeFromPath(path), archive.FileStream(), );
//		mapMeshes.insert(std::make_pair(path, newMesh));
//		return newMesh;
//	}
//}
