#include "boidpch.h"
#include "ShadercIncluder.h"


namespace Polyboid
{
	ShadercIncluder::ShadercIncluder(const std::string& rootPath): m_IncludeRootPath(rootPath)
	{
	}

    shaderc_include_result* ShadercIncluder::GetInclude(const char* requested_source, shaderc_include_type type,
	                                                    const char* requesting_source, size_t include_depth)
    {
        std::string include_filename(requested_source);

    	std::ifstream include_file(m_IncludeRootPath + "/" + include_filename);

        

        if (!include_file.is_open()) {
            __debugbreak();
            std::cerr << "Could not open included file: " << include_filename << std::endl;
            return nullptr;
        }
        std::string contents((std::istreambuf_iterator<char>(include_file)),
                                      std::istreambuf_iterator<char>());
        const std::string name = std::string(requested_source);

        include_file.close();

        auto container = new std::array<std::string, 2>;
        (*container)[0] = name;
        (*container)[1] = contents;

        auto data = new shaderc_include_result;

        data->user_data = container;

        data->source_name = (*container)[0].data();
        data->source_name_length = (*container)[0].size();

        data->content = (*container)[1].data();
        data->content_length = (*container)[1].size();
    	
        return data;
    }

    void ShadercIncluder::ReleaseInclude(shaderc_include_result* data)
    {
        // Implement your logic for releasing the memory allocated for the
        // shaderc_include_result struct here
        //delete[] data->content;

        delete static_cast<std::array<std::string, 2>*>(data->user_data);
        delete data;

       // delete data;
    }

    ShadercIncluder::~ShadercIncluder()
    {
    }
}
