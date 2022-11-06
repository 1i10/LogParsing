#include <iostream>
#include <filesystem>
#include <regex>
#include <fstream>

namespace fs = std::filesystem;

void handlingFile(fs::path pathToFile, std::regex regexExtension);
std::string readFile(fs::path pathToFile);

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");

    if (argc < 3)
    {
        std::cout << "Too few arguments" << std::endl;
        std::cout << "Correct format is LogParsing.cpp <<path to file/directory>> <<regular expression>>" << std::endl;
        std::cout << "For example, LogParsing.cpp D:\\Projects\\LogParsing Error" << std::endl;
        
        return -1;
    }

    const fs::path pathTo{argv[1]};
    std::regex regexExtension("^\.(txt|log)$");

    try
    {
        if (fs::is_regular_file(pathTo))//exists and this is path to file
        {
            handlingFile(pathTo, regexExtension);
        }
        else if(fs::is_directory(pathTo))//exists and this is path to directory
        {
            std::cout << "path to directory" << std::endl;

            for (const auto& entry : std::filesystem::directory_iterator(pathTo))
            {
                if (fs::is_regular_file(entry.path()))
                {
                    handlingFile(entry, regexExtension);
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

void handlingFile(fs::path pathToFile, std::regex regexExtension)
{
    std::string fileExtension = pathToFile.extension().string();

    if (std::regex_match(fileExtension, regexExtension))
    {
        std::cout << readFile(pathToFile) << std::endl;
    }
}

std::string readFile(fs::path pathToFile)
{
    std::ifstream f(pathToFile, std::ios::in);

    const auto sz = fs::file_size(pathToFile);

    std::string result(sz, '\0');

    f.read(result.data(), sz);

    return result;
}