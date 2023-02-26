#pragma once
#include <shaderc/shaderc.hpp>


namespace Polyboid
{
    class ShadercIncluder : public shaderc::CompileOptions::IncluderInterface
    {

    private:
        std::string m_IncludeRootPath;

    public:
        explicit ShadercIncluder(const std::string& rootPath);

        shaderc_include_result* GetInclude(const char* requested_source, shaderc_include_type type,
            const char* requesting_source, size_t include_depth) override;
        void ReleaseInclude(shaderc_include_result* data) override;
        ~ShadercIncluder() override;
    };

}

