#include <iostream>
#include <filesystem>
#include <regex>
#include <fstream>

namespace fs = std::filesystem;

void handlingFile(fs::path pathToFile, std::regex regexExtension, std::regex regexParsing);
std::string searchLinesByRegex(fs::path pathToFile, std::regex regexParsing);

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");

    if (argc < 3)
    {
        std::cout << "Too few arguments" << std::endl;
        std::cout << "Correct format is LogParsing.cpp <<path to file/directory>> <<regular expression>>" << std::endl;
        std::cout << "For example, LogParsing.cpp D:\\Projects\\LogParsing ^(.*?(\bError\b)[^$]*)$" << std::endl;
        
        return -1;
    }

    const fs::path pathTo{argv[1]};
    std::regex regexParsing(argv[2]);
    std::regex regexExtension("^\.(txt|log)$");

    try
    {
        if (fs::is_regular_file(pathTo))//exists and this is path to file
        {
            handlingFile(pathTo, regexExtension, regexParsing);
        }
        else if(fs::is_directory(pathTo))//exists and this is path to directory
        {
            std::cout << "path to directory" << std::endl;

            for (const auto& entry : std::filesystem::directory_iterator(pathTo))
            {
                if (fs::is_regular_file(entry.path()))
                {
                    handlingFile(entry, regexExtension, regexParsing);
                }

            }
        }
        else
        {
            std::cout << "Path not exists" << std::endl;
        }
    }
    catch (fs::filesystem_error const& e)
    {
        std::cout << e.code().message() << std::endl;
    }
    
    return 0;
}

void handlingFile(fs::path pathToFile, std::regex regexExtension, std::regex regexParsing)
{
    std::string fileExtension = pathToFile.extension().string();

    if (std::regex_match(fileExtension, regexExtension))
    {
        std::cout << searchLinesByRegex(pathToFile, regexParsing) << std::endl;
    }
}

std::string searchLinesByRegex(fs::path pathToFile, std::regex regexParsing)
{
    std::ifstream inputFile(pathToFile, std::ios::in);

    std::string result = "";
    std::string line;
    while (std::getline(inputFile, line))
    {
        std::smatch match;
        if (std::regex_search(line, match, regexParsing))
        {
            result += line + '\n';
        }
    }

    return result;
}