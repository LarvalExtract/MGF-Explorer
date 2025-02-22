#pragma once

#include "MGFArchive.h"
#include "MGFFile.h"

class MGFFileDeserializer
{
public:
	MGFFileDeserializer(const MGFFile& mgfFile) : m_mgfFile(mgfFile), m_SavedOffset(m_mgfFile.MgfArchive.tellg()) {}

	~MGFFileDeserializer()
	{
		m_mgfFile.MgfArchive.seekg(m_SavedOffset);
	}

	void ReadBytes(char* dst, std::size_t count, std::size_t offset = 0)
	{
		m_mgfFile.MgfArchive.seekg(m_mgfFile.FileOffset + offset).read(dst, std::min(count, m_mgfFile.FileLength - offset));
	}

	std::vector<char> ReadAllBytes() const
	{
		std::vector<char> bytes(m_mgfFile.FileLength);
		m_mgfFile.MgfArchive.seekg(m_mgfFile.FileOffset).read(bytes.data(), bytes.size());
		return bytes;
	}

	template<typename Structure>
	Structure Deserialize(std::size_t offset = 0, std::size_t length = sizeof(Structure))
	{
		Structure result{};
		ReadBytes(reinterpret_cast<char*>(&result), length, offset ? offset : m_Offset);
		m_Offset += length;
		return result;
	}

	auto CurrentOffset() const
	{
		return m_Offset;
	}

	auto& CurrentOffset()
	{
		return m_Offset;
	}

private:
	const MGFFile& m_mgfFile;
	std::size_t m_Offset = 0;
	const size_t m_SavedOffset;
};
