#pragma once
#include "RGS/Maths.h"
#include <string>

namespace RGS
{
	struct VertexBase
	{
		Vec4 ModelPos = { 0,0,0,1 };//模型空间
		operator const std::string()const { return "ModelPos:" + (std::string)ModelPos; }
	};
	struct VaryingBase
	{
		Vec4 ClipPos = { 0.0f,0.0f,0.0f,1.0f };//裁剪
		Vec4 NdcPos = { 0.0f,0.0f,0.0f,1.0f };//投影矩阵
		Vec4 FragPos = { 0.0f,0.0f,0.0f,1.0f };//视口变换
	};
	struct UniformsBase
	{
		Mat4 MVP;
		operator const std::string()const { return(std::string)MVP; }
	};
}