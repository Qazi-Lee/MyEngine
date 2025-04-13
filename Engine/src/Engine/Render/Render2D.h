#pragma once
#include"Engine/Render/GraphicsCamera.h"
#include"Engine/Render/Texture.h"
#include"Engine/Render/SubTexture2D.h"
#include"Engine/Render/Camera.h"
#include "Engine/Render/EditorCamera.h"
//#include"Engine/Scene/SceneCamera.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace ENGINE
{
	class Render2D 
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const GraphicsCamera2D&camera);
		static void BeginScene(const Camera& camera,const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawText_(const glm::mat4& transform);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,const float&rotation=0.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,const float&rotation=0.0f);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color,const int& entityID=-1);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, const int& entityID = -1);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>&texture, const glm::vec4& color = glm::vec4(1.0f), const float &rotation=0.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>&texture, const glm::vec4& color = glm::vec4(1.0f), const float &rotation=0.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), const float& rotation = 0.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), const float& rotation = 0.0f);
		
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);

		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID = -1);

		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID = -1);
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);

		static void SetLineWidth(float width);
		static float GetLineWidth();
		struct Stats
		{
			int QuadCount = 0;
			int DrawCalls = 0;

			int GetQuadVertexCount()const { return QuadCount*4; }
			int GetQuadIndexCount()const { return QuadCount * 6; }
		};

		static void ResetStats();
		static Stats GetStats();

	private:
		static void StartBatch();
		static void NextBatch();
	};


}