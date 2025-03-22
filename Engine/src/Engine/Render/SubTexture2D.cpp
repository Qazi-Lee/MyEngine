#include"pch.h"
#include"SubTexture2D.h"

namespace ENGINE
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, glm::vec2 min, glm::vec2 max)
	{
		m_texture = texture;

		TextureCoord[0] = { min.x,min.y };
		TextureCoord[1] = { max.x,min.y };
		TextureCoord[2] = { max.x,max.y };
		TextureCoord[3] = { min.x,max.y };
	}
	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, glm::vec2 index, glm::vec2 cellsize, const glm::vec2& spritesize)
	{
		glm::vec2 min = { (index.x * cellsize.x) / (float)texture->GetWidth(),(index.y * cellsize.y) / (float)texture->GetHeight() };
		glm::vec2 max = { ((index.x+spritesize.x) * cellsize.x) / (float)texture->GetWidth(),((index.y+spritesize.y) * cellsize.y) / (float)texture->GetHeight() };
		Ref<SubTexture2D>result = std::make_shared<SubTexture2D>(texture,min,max);
		return result;
	}
}