#pragma once
#include "ShaderBase.h"
#include "Math.h"
#include"RGS/Texture.h"

namespace RGS
{
	struct  BlinnVertex :public VertexBase
	{
		Vec3 ModelNormal;
		Vec2 TexCoord = { 0.0f,0.0f };//ģ�Ϳռ����������
	};
	struct  BlinnVaryings :public VaryingBase
	{
		Vec3 WorldPos;
		Vec3 WorldNormal;
		Vec2 TexCoord;
	};
	struct BlinnUniforms:public UniformsBase
	{
		Mat4 Model;
		Mat4 ModelNormalToWorld;
		Vec3 LightPos{ 0.0f,1.0f,2.0f };
		Vec3 LightAmbient{ 0.3f,0.3f,0.3f };//������
		Vec3 LightDiffuse{ 0.5f,0.5f,0.5f };//�������
		Vec3 LightSpecular{ 1.0f,1.0f,1.0f };//�߹�
		Vec3 ObjectColor{ 1.0f,1.0f,1.0f };//������ɫ
		Vec3 CameraPos;
		float Shininess = 32.0f;//�߹ⷴ���

		Texture* Diffuse = nullptr;
		Texture* Specular = nullptr;
	};
	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms);
	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms);
}