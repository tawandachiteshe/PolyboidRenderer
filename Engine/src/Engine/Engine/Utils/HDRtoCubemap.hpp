#pragma once

/**
* @file HdriToCubemap.hpp
*
* @brief Simple single-header library to convert an equirectangular hdri image to cubemap images.
*
* @author Ingvar Out
* Contact: ivarout2@gmail.com
*
* @date November 22, 2020
*/


#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "CubeMapFace.hpp"
#include "Engine/Engine/Base.h"
#include "glm/ext/scalar_common.hpp"

#include "glm/ext/scalar_constants.hpp"

namespace Polyboid
{

    template <typename T>
    class HDRToCubemap
    {
    public:
        static constexpr auto M_PI = glm::pi<float>();
        HDRToCubemap(const std::string& fileLocation, int cubemapResolution, bool filterLinear = true);
        ~HDRToCubemap();

        bool IsHdri() const { return m_isHdri; }
        Ref<CubeMapFace<T>> GetCubeMapFace() { return m_faces; }
        void WriteCubemap(const std::string& outputFolder = "");

    private:
        void CalculateCubemap();

    private:
        bool m_isHdri;
        int m_width{}, m_height{}, m_channels{};
        bool m_filterLinear;
        T* m_imageData;
    	Ref<CubeMapFace<T>> m_faces = nullptr;
        int m_cubemapResolution;
    };

    template <>
    inline HDRToCubemap<uint8_t>::HDRToCubemap(const std::string& pathHdri, int cubemapResolution, bool filterLinear)
        : m_filterLinear(filterLinear), m_cubemapResolution(cubemapResolution)
    {
        stbi_set_flip_vertically_on_load(1);
        m_isHdri = stbi_is_hdr(pathHdri.c_str());
        m_isHdri = false;

        m_imageData = stbi_load(pathHdri.c_str(), &m_width, &m_height, &m_channels, 0);

        m_faces =  std::make_shared<CubeMapFace<uint8_t>>(m_cubemapResolution, m_channels);
        CalculateCubemap();
    }

    template <>
    inline HDRToCubemap<float>::HDRToCubemap(const std::string& pathHdri, int cubemapResolution, bool filterLinear)
        : m_filterLinear(filterLinear), m_cubemapResolution(cubemapResolution)
    {
        m_isHdri = stbi_is_hdr(pathHdri.c_str());
        m_isHdri = true;

        m_imageData = stbi_loadf(pathHdri.c_str(), &m_width, &m_height, &m_channels, 0);

        m_faces = std::make_shared<CubeMapFace<float>>(m_cubemapResolution, m_channels);

        CalculateCubemap();
    }

    template<typename T>
    HDRToCubemap<T>::~HDRToCubemap()
    {
        stbi_image_free(m_imageData);
    }

    template<typename T>
    void HDRToCubemap<T>::WriteCubemap(const std::string& outputFolder)
    {
        stbi_flip_vertically_on_write(1);
        std::vector<std::string> filenames = { "front", "back", "left", "right", "up", "down" };
        for (int i = 0; i < 6; i++)
        {
            int success;

            std::string path = outputFolder + (outputFolder.empty() ? "" : "/") + filenames[i];
            if (m_isHdri)
            {
                path += ".hdr";
                success = stbi_write_hdr(path.c_str(), m_cubemapResolution, m_cubemapResolution, m_channels, (const float*)m_faces[i]);
            }
            else
            {
                path += ".png";
                success = stbi_write_png(path.c_str(), m_cubemapResolution, m_cubemapResolution, m_channels, (const unsigned char*)m_faces[i], 0);
            }
            if (!success)
                std::cout << "Warning: could not write '" << path << "'";
        }
    }

 // cpu implementation
    template<typename T>
    void HDRToCubemap<T>::CalculateCubemap()
    {
        std::array<std::array<glm::vec3, 3>, 6> startRightUp = { { // for each face, contains the 3d starting point (corresponding to left bottom pixel), right direction, and up direction in 3d space, correponding to pixel x,y coordinates of each face		{{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        	{{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}},   // right
        	{{{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}}},  // left
             {{{-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}},   // down
        	{{{-1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}},   // up
        	{{{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}},  // front
            {{{1.0f, -1.0f, 1.0f},  {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}},   // back 
        } };

        for (int i = 0; i < 6; i++)
        {
            glm::vec3& start = startRightUp[i][0];
            glm::vec3& right = startRightUp[i][1];
            glm::vec3& up = startRightUp[i][2];

            T* face = (m_faces->GetFaces())[i];
            glm::vec3 pixelDirection3d {}; // 3d direction corresponding to a pixel in the cubemap face
            //#pragma omp parallel for (private pixelDirection?)
            for (int row = 0; row < m_cubemapResolution; row++)
            {
                for (int col = 0; col < m_cubemapResolution; col++)
                {
                    pixelDirection3d.x = start.x + ((float)col * 2.0f + 0.5f) / (float)m_cubemapResolution * right.x + ((float)row * 2.0f + 0.5f) / (float)m_cubemapResolution * up.x;
                    pixelDirection3d.y = start.y + ((float)col * 2.0f + 0.5f) / (float)m_cubemapResolution * right.y + ((float)row * 2.0f + 0.5f) / (float)m_cubemapResolution * up.y;
                    pixelDirection3d.z = start.z + ((float)col * 2.0f + 0.5f) / (float)m_cubemapResolution * right.z + ((float)row * 2.0f + 0.5f) / (float)m_cubemapResolution * up.z;

                    float azimuth = atan2f(pixelDirection3d.x, -pixelDirection3d.z) + (float)M_PI; // add pi to move range to 0-360 deg
                    float elevation = atanf(pixelDirection3d.y / sqrtf(pixelDirection3d.x * pixelDirection3d.x + pixelDirection3d.z * pixelDirection3d.z)) + (float)M_PI / 2.0f;


                    float colHdri = (azimuth / (float)M_PI / 2.0f) * m_width; // add pi to azimuth to move range to 0-360 deg
                    float rowHdri = (elevation / (float)M_PI) * m_height;

                    if (!m_filterLinear)
                    {
                        int colNearest = glm::clamp((int)colHdri, 0, m_width - 1);
                        int rowNearest = glm::clamp((int)rowHdri, 0, m_height - 1);

                        face[col * m_channels + m_cubemapResolution * row * m_channels] = m_imageData[colNearest * m_channels + m_width * rowNearest * m_channels]; // red
                        face[col * m_channels + m_cubemapResolution * row * m_channels + 1] = m_imageData[colNearest * m_channels + m_width * rowNearest * m_channels + 1]; //green
                        face[col * m_channels + m_cubemapResolution * row * m_channels + 2] = m_imageData[colNearest * m_channels + m_width * rowNearest * m_channels + 2]; //blue
                        if (m_channels > 3)
                            face[col * m_channels + m_cubemapResolution * row * m_channels + 3] = m_imageData[colNearest * m_channels + m_width * rowNearest * m_channels + 3]; //alpha
                    }
                    else // perform bilinear interpolation
                    {
                        float intCol, intRow;
                        float factorCol = modf(colHdri - 0.5f, &intCol);        // factor gives the contribution of the next column, while the contribution of intCol is 1 - factor
                        float factorRow = modf(rowHdri - 0.5f, &intRow);

                        const int low_idx_row = static_cast<int>(intRow);
                        const int low_idx_column = static_cast<int>(intCol);
                        int high_idx_column;
                        if (factorCol < 0.0f)                           //modf can only give a negative value if the azimuth falls in the first pixel, left of the center, so we have to mix with the pixel on the opposite side of the panoramic image
                            high_idx_column = m_width - 1;
                        else if (low_idx_column == m_width - 1)          //if we are in the right-most pixel, and fall right of the center, mix with the left-most pixel
                            high_idx_column = 0;
                        else
                            high_idx_column = low_idx_column + 1;

                        int high_idx_row;
                        if (factorRow < 0.0f)
                            high_idx_row = m_height - 1;
                        else if (low_idx_row == m_height - 1)
                            high_idx_row = 0;
                        else
                            high_idx_row = low_idx_row + 1;

                        factorCol = abs(factorCol);
                        factorRow = abs(factorRow);
                        float f1 = (1 - factorRow) * (1 - factorCol);
                        float f2 = factorRow * (1 - factorCol);
                        float f3 = (1 - factorRow) * factorCol;
                        float f4 = factorRow * factorCol;

                        for (int j = 0; j < m_channels; j++)
                        {
	                        auto interpolatedValue = static_cast<uint8_t>(m_imageData[low_idx_column * m_channels + m_width * low_idx_row * m_channels + j] * f1 +
                                m_imageData[low_idx_column * m_channels + m_width * high_idx_row * m_channels + j] * f2 +
                                m_imageData[high_idx_column * m_channels + m_width * low_idx_row * m_channels + j] * f3 +
                                m_imageData[high_idx_column * m_channels + m_width * high_idx_row * m_channels + j] * f4);
                            face[col * m_channels + m_cubemapResolution * row * m_channels + j] = glm::clamp(interpolatedValue, static_cast<uint8_t>(0), static_cast<uint8_t>(255));
                        }
                    }
                }
            }
        }
    }
}
