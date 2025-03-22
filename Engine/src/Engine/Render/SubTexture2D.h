#pragma once
#include"Texture.h"
#include"glm/glm.hpp"
namespace ENGINE
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, glm::vec2 min, glm::vec2 max);

		const Ref<Texture2D>& GetTexture()const { return m_texture; }
		const glm::vec2* GetTextureCoord()const { return TextureCoord; }
		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, glm::vec2 index, glm::vec2 cellsize, const glm::vec2& spritesize = glm::vec2(1.0f));

	private:
		Ref<Texture2D>m_texture;
		glm::vec2 TextureCoord[4];
	};

}