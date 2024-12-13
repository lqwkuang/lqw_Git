#pragma once
#include "RGS/Maths.h"
#include <string>

namespace RGS
{
	struct VertexBase
	{
		Vec4 ModelPos = { 0,0,0,1 };//ģ�Ϳռ�
		operator const std::string()const { return "ModelPos:" + (std::string)ModelPos; }
	};
	struct VaryingBase
	{
		Vec4 ClipPos = { 0.0f,0.0f,0.0f,1.0f };//�ü�
		Vec4 NdcPos = { 0.0f,0.0f,0.0f,1.0f };//ͶӰ����
		Vec4 FragPos = { 0.0f,0.0f,0.0f,1.0f };//�ӿڱ任
	};
	struct UniformsBase
	{
		Mat4 MVP;
		operator const std::string()const { return(std::string)MVP; }
	};
}