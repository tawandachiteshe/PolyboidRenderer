#include "StatsWindow.h"
#include "imgui.h"
#include "Engine/Engine/ImguiSetup.h"
#include "Engine/Renderer/Image2D.h"
#include "Engine/Renderer/Texture2D.h"

namespace Polyboid
{
	StatsWindow::StatsWindow()
	{
		m_Name = "Settings";
		auto texture = Texture2D::Create({ .sizedFormat = EngineGraphicsFormats::RGBA8, .usage = ImageUsage::Sampling, .path = "Assets/Textures/pic.jpg" });
		m_Texture = Imgui::GetVulkanTextureID(texture);
		const uint32_t red = 0xFF'00'00'FF;

		auto redTexture = Texture2D::Create({ .sizedFormat = EngineGraphicsFormats::RGBA8, .usage = ImageUsage::Sampling, .Width = 1, .Height = 1 }, &red);
		m_RedTexture = Imgui::GetVulkanTextureID(redTexture);

		const uint32_t green = 0xFF'00'FF'00;

		auto greenTexture = Texture2D::Create({ .sizedFormat = EngineGraphicsFormats::RGBA8, .usage = ImageUsage::Sampling, .Width = 1, .Height = 1 }, &green);
		m_GreenTexture = Imgui::GetVulkanTextureID(greenTexture);

		const uint32_t blue = 0xFF'FF'00'00;

		auto blueTexture = Texture2D::Create({ .sizedFormat = EngineGraphicsFormats::RGBA8, .usage = ImageUsage::Sampling, .Width = 1, .Height = 1 }, &blue);
		m_BlueTexture = Imgui::GetVulkanTextureID(blueTexture);

	}


	void StatsWindow::RenderImgui()
	{
		ImGui::Begin(m_Name.c_str());

		auto frameRate = ImGui::GetIO().Framerate;

		ImGui::Text("Frame time %f", 1.0f / frameRate);
		ImGui::Text("Frame rate %f", frameRate);
		ImGui::Text("Mem usage %llu", EngineMemoryManager::GetMemoryUsage() / 1024llu);
		ImGui::Text("Mem Allocation Count %llu", EngineMemoryManager::GetAllocationCount());
		ImGui::Text("Mem Free Count %llu", EngineMemoryManager::GetFreeCount());

		ImGui::Image(m_Texture, { 128, 128 });
		ImGui::SameLine();
		ImGui::Image(m_RedTexture, { 128, 128 });
		ImGui::SameLine();
		ImGui::Image(m_GreenTexture, { 128, 128 });
		ImGui::SameLine();
		ImGui::Image(m_BlueTexture, { 128, 128 });

		ImGui::End();
	}

	void StatsWindow::Update(float ts)
	{
	}

	StatsWindow::~StatsWindow()
	{
	}
}
