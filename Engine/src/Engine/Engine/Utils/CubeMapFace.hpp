#pragma once
#include <cstdint>


namespace Polyboid
{

    template <typename T>
    class CubeMapFace
    {
    private:
        T** m_faces = nullptr;
        std::vector<std::vector<T>> m_ArrayFaces;
        int32_t m_Resolution;
        int32_t m_Channels;
        bool m_UseVector = false;

    public:

        CubeMapFace(int32_t resolution, int32_t channels);
        CubeMapFace(const std::vector<std::vector<T>>& facesData, int32_t resolution, int32_t channels);
        int32_t GetResolution() { return m_Resolution; }
        int32_t GetChannels() { return m_Channels; }

      
        std::vector<std::vector<T>>& GetArrayFaces() { return m_ArrayFaces; }
        T* GetFrontArray() { return m_ArrayFaces[0].data(); }
        T* GetBackArray() { return m_ArrayFaces[1].data(); }
        T* GetLeftArray() { return m_ArrayFaces[2].data(); }
        T* GetRightArray() { return m_ArrayFaces[3].data(); }
        T* GetUpArray() { return m_ArrayFaces[4].data(); }
        T* GetDownArray() { return m_ArrayFaces[5].data(); }

        T** GetFaces() { return m_faces; }
        T* GetFront() { return m_faces[0]; }
        T* GetBack() { return m_faces[1]; }
        T* GetLeft() { return m_faces[2]; }
        T* GetRight() { return m_faces[3]; }
        T* GetUp() { return m_faces[4]; }
        T* GetDown() { return m_faces[5]; }

        bool GetIsUsingVector() { return m_UseVector; }


        ~CubeMapFace()
        {
	        if (!m_UseVector)
	        {
                for (int i = 0; i < 6; i++)
                    delete[] * m_faces++;
	        }
       
        }
    };

    template <>
    inline  CubeMapFace<uint8_t>::CubeMapFace(const std::vector<std::vector<uint8_t>>& facesData, int32_t resolution, int32_t channels): m_ArrayFaces(facesData), m_Resolution(resolution), m_Channels(channels), m_UseVector(true)
    {

    }



    template <>
    inline CubeMapFace<float>::CubeMapFace(int32_t resolution, int32_t channels) : m_Resolution(resolution), m_Channels(channels), m_UseVector(false)
    {
        m_faces = new float* [6];
        for (int i = 0; i < 6; i++)
            m_faces[i] = new float[resolution * resolution * channels];
    }


    template <>
    inline CubeMapFace<uint8_t>::CubeMapFace(int32_t resolution, int32_t channels) : m_Resolution(resolution), m_Channels(channels), m_UseVector(false)
    {
        m_faces = new uint8_t * [6];
        for (int i = 0; i < 6; i++)
            m_faces[i] = new uint8_t[resolution * resolution * channels];
    }


}
