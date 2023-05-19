#include "StatsWindow.h"
#include "imgui.h"
#include "Engine/Renderer/Image2D.h"
#include "Engine/Renderer/Texture.h"

namespace Polyboid
{
	StatsWindow::StatsWindow()
	{
		m_Name = "Settings";
		m_Texture = Texture::Create({ .sizedFormat = EngineGraphicsFormats::RGBA8, .usage = ImageUsage::Sampling, .path = "Assets/Textures/pic.jpg" });

		const uint32_t red = 0xFF'00'00'FF;

		m_RedTexture = Texture::Create({ .sizedFormat = EngineGraphicsFormats::RGBA8, .usage = ImageUsage::Sampling, .Width = 1, .Height = 1 }, &red);

		const uint32_t green = 0xFF'00'FF'00;

		m_GreenTexture = Texture::Create({ .sizedFormat = EngineGraphicsFormats::RGBA8, .usage = ImageUsage::Sampling, .Width = 1, .Height = 1 }, &green);

		const uint32_t blue = 0xFF'FF'00'00;

		m_BlueTexture = Texture::Create({ .sizedFormat = EngineGraphicsFormats::RGBA8, .usage = ImageUsage::Sampling, .Width = 1, .Height = 1 }, &blue);

	}

	void StatsWindow::RenderImgui()
	{
		ImGui::Begin(m_Name.c_str());

		auto frameRate = ImGui::GetIO().Framerate;

		ImGui::Text("Frame time %f", 1.0f / frameRate);
		ImGui::Text("Frame rate %f", frameRate);

		ImGui::Image(std::any_cast<ImTextureID>(m_Texture->GetImGuiTexture()), { 128, 128 });
		ImGui::SameLine();
		ImGui::Image(std::any_cast<ImTextureID>(m_RedTexture->GetImGuiTexture()), { 128, 128 });
		ImGui::SameLine();
		ImGui::Image(std::any_cast<ImTextureID>(m_GreenTexture->GetImGuiTexture()), { 128, 128 });
		ImGui::SameLine();
		ImGui::Image(std::any_cast<ImTextureID>(m_BlueTexture->GetImGuiTexture()), { 128, 128 });

		ImGui::End();
	}

	void StatsWindow::Update(float ts)
	{
	}

	StatsWindow::~StatsWindow()
	{
	}
}
