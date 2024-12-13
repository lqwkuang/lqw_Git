#include"Texture.h"
#include"Base.h"

#include"stb_image/stb_image.h"
#include<iostream>

namespace RGS
{

	Texture::Texture(const std::string& path)
		:m_Path(path)
	{
		Init();
	}

	Texture::~Texture()
	{
		if (m_Data)
			delete[] m_Data;
		m_Data = nullptr;
	}

	Vec4 Texture::Sample(Vec2 texCoords) const
	{
		//²ÉÑù
		float vx = Clamp(texCoords.X, 0.0f, 1.0f);
		float vy = Clamp(texCoords.Y, 0.0f, 1.0f);

		int x = vx * (m_Width - 1) + 0.5f;
		int y = vy * (m_Height - 1) + 0.5f;

		int index = y * m_Width + x;
		return m_Data[index];
	}

	void Texture::Init()
	{
		int width, height, channles;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		data = stbi_load(m_Path.c_str(), &width, &height, &channles, 0);

		m_Height = height;
		m_Width = width;
		m_Channels = channles;
		int size = height * width;
		m_Data = new Vec4[size];

		if (channles == 4)
		{
			for (int i = 0; i < size; i++)
			{
				m_Data[i].X = UChar2Float(data[i * 4]);
				m_Data[i].Y = UChar2Float(data[i * 4]+1);
				m_Data[i].Z = UChar2Float(data[i * 4]+2);
				m_Data[i].W = UChar2Float(data[i * 4+3]);
			}
		}
		else if (channles == 3)
		{
			for (int i = 0; i < size; i++)
			{
				m_Data[i].X = UChar2Float(data[i * 4]);
				m_Data[i].Y = UChar2Float(data[i * 4] + 1);
				m_Data[i].Z = UChar2Float(data[i * 4] + 2);
				m_Data[i].W = 0.0f;
			}
		}
		else if (channles == 2)
		{
			for (int i = 0; i < size; i++)
			{
				m_Data[i].X = UChar2Float(data[i * 4]);
				m_Data[i].Y = UChar2Float(data[i * 4] + 1);
				m_Data[i].Z = 0.0f;
				m_Data[i].W = 0.0f;
			}
		}
		else if (channles == 1)
		{
			for (int i = 0; i < size; i++)
			{
				m_Data[i].X = UChar2Float(data[i * 4]);
				m_Data[i].Y =0.0f;
				m_Data[i].Z = 0.0f;
				m_Data[i].W = 0.0f;
			}
		}
	}

	
}