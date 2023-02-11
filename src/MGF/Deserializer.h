#pragma once

#include "Archive.h"
#include "File.h"

namespace MGF
{
	class Deserializer
	{
	public:
		Deserializer(const MGF::File& mgfFile) : m_mgfFile(mgfFile), m_SavedOffset(m_mgfFile.MGFArchive.tellg()) {}

		~Deserializer()
		{
			m_mgfFile.MGFArchive.seekg(m_SavedOffset);
		}

		void ReadBytes(char* dst, std::size_t count, std::size_t offset = 0)
		{
			m_mgfFile.MGFArchive.seekg(m_mgfFile.FileOffset + offset).read(dst, std::min(count, m_mgfFile.FileLength - offset));
		}

		std::vector<char> ReadAllBytes() const
		{
			std::vector<char> bytes(m_mgfFile.FileLength);
			m_mgfFile.MGFArchive.seekg(m_mgfFile.FileOffset).read(bytes.data(), bytes.size());
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
		const MGF::File& m_mgfFile;
		std::size_t m_Offset = 0;
		const std::size_t m_SavedOffset;
	};
}