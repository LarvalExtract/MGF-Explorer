#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <unordered_map>
#include <string>
#include <filesystem>

using ConVariables = std::unordered_map<std::string, std::string>;

class ConfigSection
{
    friend class ConfigFile;

public:
    ConfigSection() = default;
    ConfigSection(const std::string_view& section);

    const std::string& Name() const { return name; }
    const std::string& operator[](const std::string& name) { return m_Variables[name]; }

private:
    std::string name;
    ConVariables m_Variables;
};

using ConCategories = std::unordered_map<std::string, ConfigSection>;

class ConfigFile
{
public:
    ConfigFile() = default;
    explicit ConfigFile(const std::string& buffer);
    explicit ConfigFile(const std::filesystem::path& filepath);

    ConVariables& operator[](const std::string& index) { return m_Categories[index].m_Variables; }

    void WriteOut();

private:
    ConCategories m_Categories;
    std::filesystem::path m_File;

    void LoadFromBuffer(const std::string& buffer);
};



#endif // CONFIGFILE_H
