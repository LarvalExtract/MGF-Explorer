#include "MGFModel.h"

#include <pugixml-1.9/src/pugixml.hpp>

MGFModel::MGFModel(const MGFTreeNode& node, std::ifstream& fileStream, MGFResourceManager& resourceManager) :
	resourceManager(resourceManager)
{
	// Copy MGMODEL XML contents to memory
	std::vector<char> xmlBuffer(node.FileLength());
	fileStream.seekg(node.FileOffset(), std::ios::beg);
	fileStream.read(xmlBuffer.data(), node.FileLength());

	pugi::xml_document doc;
	doc.load_buffer(xmlBuffer.data(), xmlBuffer.size());

	pugi::xml_node xmlres;
	xmlres = doc.first_child();


}

MGFModel::~MGFModel()
{

}
