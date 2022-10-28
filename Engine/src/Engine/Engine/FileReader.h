#pragma once
#include <cstdint>
#include <string>


namespace Polyboid
{
    class FileReader
    {
    public:

        static std::string  ReadStringEF(const std::string& Path);
        static std::string ReadString(const std::string& Path);
    
    };

}

