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
		Vec3 viewDir = Normalize(cameraPos - worldPos);//观察方向
		Vec3 lightDir = Normalize(lightPos - worldPos);//光照方向
		Vec3 halfDir = Normalize(lightDir + viewDir);//半程向量用来计算镜面光和观察角的


		Vec3 ambient = uniforms.LightAmbient;//环境光
		Vec3 specularStrength{ 1.0f,1.0f,1.0f };//反射强度
		Vec3 diffColor{ 1.0f,1.0f,1.0f };//纹理
		if (uniforms.Diffuse && uniforms.Specular)
		{
			const Vec2& texCoord = varyings.TexCoord;
			diffColor = uniforms.Diffuse->Sample(texCoord);
			ambient = ambient * diffColor;//纹理
			specularStrength = uniforms.Specular->Sample(texCoord);//反射强度
		}

		Vec3 diffuse = std::max(0.0f, Dot(worldNormal, lightDir)) * uniforms.LightDiffuse*diffColor;//漫反射光
		Vec3 specular = (float)pow(std::max(0.0f, Dot(halfDir, worldNormal)), uniforms.Shininess) * uniforms.LightSpecular*specularStrength;//镜面光

		Vec3 rescult = ambient + diffuse + specular;

		return{ rescult,1.0f };
		/*return Vec4{varyings.TexCoord.X,varyings.TexCoord.Y,0.0f,1.0f};*/
	}

}

