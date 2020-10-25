#include "configfile.h"

//#include <cctype>
//#include <algorithm>
#include <fstream>

ConfigFile::ConfigFile(const std::string& buffer)
{
    LoadFromBuffer(buffer);
}

ConfigFile::ConfigFile(const std::filesystem::path& filepath) :
    m_File(filepath)
{
    std::ifstream file(filepath, std::ios::binary | std::ios::end);
    
    if (!file.is_open())
        return;

    file.seekg(0, std::ios::end);
    int size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string buffer;
    buffer.resize(size);
    file.read(&buffer[0], size);

    LoadFromBuffer(buffer);
}

void ConfigFile::LoadFromBuffer(const std::string& buffer)
{
    std::size_t sectionStart = buffer.find('[', 0);
    std::size_t sectionEnd = std::min(buffer.find('[', sectionStart + 1), buffer.size());
    std::size_t sectionLength = sectionEnd - sectionStart;
    std::string_view buf_view(buffer);

    while (sectionStart < buffer.size())
    {
        ConfigSection vars(buf_view.substr(sectionStart, sectionLength));

        m_Categories.emplace(vars.Name(), vars);

        sectionStart = sectionEnd;
        sectionEnd = std::min(buf_view.find('[', sectionEnd + 1), buffer.size());
        sectionLength = sectionEnd - sectionStart;
    }
}

ConfigSection::ConfigSection(const std::string_view &section)
{
    auto nameStart = section.find('[', 0) + 1;
    auto nameEnd = section.find(']', nameStart);

    this->name = section.substr(nameStart, nameEnd - nameStart);

    std::size_t lineStart = 0;
    std::size_t lineEnd = section.find('\n', lineStart);;

    while (lineStart < section.size())
    {
        std::string_view line = section.substr(lineStart, lineEnd - lineStart);

        if (line[0] == ';')
        {
            // do nothing
        }
        else if (auto equals = line.find('='); equals != std::string::npos)
        {
            static const char whitespaces[] = " \"\t\r\n";

            std::size_t keyStart = line.find_first_not_of(whitespaces);
            std::size_t keyEnd = line.find_last_not_of(whitespaces, equals - 1) + 1;
            std::size_t keyLength = keyEnd - keyStart;            

            std::size_t valStart = line.find_first_not_of(whitespaces, equals + 1);
            std::size_t valEnd = line.find_last_not_of(whitespaces) + 1;
            std::size_t valLength = valEnd - valStart;            

            std::string_view key = line.substr(keyStart, keyLength);
            std::string_view val = (valStart != std::string::npos) ? line.substr(valStart, valLength) : "";

            m_Variables.insert(std::make_pair(key, val));

        }

        lineStart = lineEnd + 1;
        lineEnd = std::min(section.find('\n', lineStart), section.size());
    }
}

void ConfigFile::WriteOut()
{
    std::ofstream file(m_File);

    if (!file.is_open())
        return;

    for (const auto& cat : m_Categories)
    {
        file << '[' << cat.first << ']' << '\n';
        for (const auto& var : cat.second.m_Variables)
        {
            file << var.first << '=' << var.second << '\n';
        }
        file << '\n';
    }

    file.close();
}
