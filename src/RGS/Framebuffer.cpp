#include "Framebuffer.h"
namespace RGS
{
	Framebuffer::Framebuffer(const int width, const int height)
		:m_Width(width),m_Height(height)
	{
		m_PixelSize = m_Height * m_Width;
		m_ColorBuffer = new Vec3[m_PixelSize]();
		m_DepthBuffer = new float[m_PixelSize]();
		Clear({ 0.0f,0.0f,0.0f });
		ClearDepth(1.0f);
	}

	Framebuffer::~Framebuffer()
	{
		delete[] m_ColorBuffer;
		delete[] m_DepthBuffer;
		m_ColorBuffer = nullptr;
		m_DepthBuffer = nullptr;
	}

	void Framebuffer::SetColor(const int x, const int y, const Vec3& color)
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		{
			return;
		}
		int index = GetPixelIndex(x, y);
		m_ColorBuffer[index] = color;
	}

	Vec3 Framebuffer::GetColor(const int x, const int y) const
	{
		int index = GetPixelIndex(x, y);
		if (index < m_PixelSize && index >= 0)
		{
			return	m_ColorBuffer[index];
		}
		else
		{
			return {0.0f,0.0f,0.0f};
		}
	}

	void Framebuffer::SetDepth(const int x, const int y, const float depth)
	{
		int index = GetPixelIndex(x, y);
		if (index < m_PixelSize && index >= 0)
		{
			m_DepthBuffer[index] = depth;
		}
		else
		{
			
		}
	}

	float Framebuffer::GetDepth(const int x, const int y) const
	{
		int index = GetPixelIndex(x, y);
		if (index < m_PixelSize && index >= 0)
		{
			return	m_DepthBuffer[index];
		}
		else
		{
			return 0.0f;
		}
	}

	void Framebuffer::Clear(const Vec3& color)
	{
		for (int i = 0; i < m_PixelSize; i++)
		{
			m_ColorBuffer[i] = color;
		}
	}

	void Framebuffer::ClearDepth(float depth)
	{
		for (int i = 0; i < m_PixelSize; i++)
		{
			m_DepthBuffer[i] = depth;
		}
	}

}
