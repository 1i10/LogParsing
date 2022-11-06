#include <iostream>
#include <filesystem>
#include <regex>
#include <fstream>
#include <chrono>
#include <sstream>

namespace fs = std::filesystem;

void handlingFile(fs::path pathToFile, std::regex regexExtension, std::regex regexParsing, std::string outFilename);
std::string searchLinesByRegex(fs::path pathToFile, std::regex regexParsing);
void saveDataInFile(std::string data, std::string filename);
std::string getCurrentTime();

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");

    if (argc < 3)
    {
        std::cout << "Too few arguments" << std::endl;
        std::cout << "For example, correct arguments is argv[1] - D:\\Projects\\LogParsing, argv[2] - ^(.*?(\bError\b)[^$]*)$" << std::endl;
        
        return -1;
    }

    const fs::path pathTo{argv[1]};
    std::regex regexParsing(argv[2]);
    std::regex regexExtension("^\.(txt|log)$");
    std::string outputFilename = "LogParsing_" + getCurrentTime() + ".txt";

    try
    {
        if (fs::is_regular_file(pathTo))//exists and this is path to file
        {
            handlingFile(pathTo, regexExtension, regexParsing, outputFilename);
        }
        else if(fs::is_directory(pathTo))//exists and this is path to directory
        {
            for (const auto& entry : std::filesystem::directory_iterator(pathTo))
            {
                if (fs::is_regular_file(entry.path()))
                {
                    handlingFile(entry, regexExtension, regexParsing, outputFilename);
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

void handlingFile(fs::path pathToFile, std::regex regexExtension, std::regex regexParsing, std::string outFilename)
{
    std::string fileExtension = pathToFile.extension().string();

    if (std::regex_match(fileExtension, regexExtension))
    {
        std::cout << "\033[0;31mFile: " << pathToFile.filename() << "\033[0m" << std::endl;

        std::string searchLines = searchLinesByRegex(pathToFile, regexParsing);

        saveDataInFile(searchLines, outFilename);
        std::cout << "The data is saved in " + outFilename << std::endl;
    }
}

std::string searchLinesByRegex(fs::path pathToFile, std::regex regexParsing)
{
    std::ifstream inputFile(pathToFile, std::ios::in);

    __int64 lineCountInFile = std::count(std::istreambuf_iterator<char>(inputFile), std::istreambuf_iterator<char>(), '\n');
    inputFile.clear();
    inputFile.seekg(std::ifstream::beg);

    std::string result = "";
    std::string line;
    int countLinesFound = 0;
    
    for(int indexLine = 1; std::getline(inputFile, line); indexLine++)
    {
        std::smatch match;
        if (std::regex_search(line, match, regexParsing))
        {
            result += line + '\n';
            
            countLinesFound++;
        }

        std::cout << "Number of lines read: " << indexLine << "/" << lineCountInFile << ". "
            << "Lines found: " << countLinesFound << "\r";
        std::cout.flush();
    }
    std::cout << std::endl << "\033[1;32mDone!\033[0m" << std::endl;

    return result;
}

void saveDataInFile(std::string data, std::string filename)
{
    std::ofstream out(filename, std::ofstream::app);

    out << data;

    out.close();
}

std::string getCurrentTime()
{
    const auto now = std::chrono::system_clock::now();
    const auto inTimeT = std::chrono::system_clock::to_time_t(now);

    std::stringstream outStream;

    struct tm timeInfo;
    const auto errnoValue = localtime_s(&timeInfo, &inTimeT);
    if (errnoValue != 0)
    {
        throw std::runtime_error("localtime_s() failed: " + std::to_string(errnoValue));
    }

    outStream << std::put_time(&timeInfo, "%Y-%m-%d_%H-%M-%S");
    
    return outStream.str();
}
