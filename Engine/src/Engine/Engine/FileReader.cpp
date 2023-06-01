﻿#include "boidpch.h"

#include "FileReader.h"

#include <fstream>
#include <sstream>
#include <ios>



namespace Polyboid
{
    std::string FileReader::ReadStringEF(const std::string& Path)
    {
        std::ifstream in(Path, std::ios::binary);

        if (in)
        {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return contents;
        }

        return "";
    }

    std::string FileReader::ReadString(const std::string& Path)
    {
        std::ifstream in(Path);
        std::stringstream ss;
        ss << in.rdbuf();
        return ss.str();
    }
}
