#include "BlinnShader.h"

namespace RGS
{
	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms)
	{
		varyings.ClipPos = uniforms.MVP * vertex.ModelPos;
		varyings.TexCoord = vertex.TexCoord;
		varyings.WorldPos = uniforms.Model * vertex.ModelPos;
		varyings.WorldNormal = uniforms.ModelNormalToWorld * Vec4{ vertex.ModelNormal,0.0f };
		/*varyings.ClipPos = uniforms.MVP * vertex.ModelPos;
		varyings.TexCoord = vertex.TexCoord;*/
	}

	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms)
	{
		discard = false;

		const Vec3& cameraPos = uniforms.CameraPos;
		const Vec3& lightPos = uniforms.LightPos;
		const Vec3& worldPos = varyings.WorldPos;
		Vec3 worldNormal = Normalize(varyings.WorldNormal);
		Vec3 viewDir = Normalize(cameraPos - worldPos);//�۲췽��
		Vec3 lightDir = Normalize(lightPos - worldPos);//���շ���
		Vec3 halfDir = Normalize(lightDir + viewDir);//��������������㾵���͹۲�ǵ�


		Vec3 ambient = uniforms.LightAmbient;//������
		Vec3 specularStrength{ 1.0f,1.0f,1.0f };//����ǿ��
		Vec3 diffColor{ 1.0f,1.0f,1.0f };//����
		if (uniforms.Diffuse && uniforms.Specular)
		{
			const Vec2& texCoord = varyings.TexCoord;
			diffColor = uniforms.Diffuse->Sample(texCoord);
			ambient = ambient * diffColor;//����
			specularStrength = uniforms.Specular->Sample(texCoord);//����ǿ��
		}

		Vec3 diffuse = std::max(0.0f, Dot(worldNormal, lightDir)) * uniforms.LightDiffuse*diffColor;//�������
		Vec3 specular = (float)pow(std::max(0.0f, Dot(halfDir, worldNormal)), uniforms.Shininess) * uniforms.LightSpecular*specularStrength;//�����

		Vec3 rescult = ambient + diffuse + specular;

		return{ rescult,1.0f };
		/*return Vec4{varyings.TexCoord.X,varyings.TexCoord.Y,0.0f,1.0f};*/
	}

}

