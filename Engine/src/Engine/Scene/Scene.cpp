#include"pch.h"

#include"Scene.h"
#include"Entity.h"
#include"Engine/Render/Render2D.h"
#include"Component.h"

#include"box2d/box2d.h"


namespace ENGINE
{
	static const char* LoadScriptFromFile(std::string path)
	{
		std::ifstream file(path);
		if (!file.is_open()) {
			// 处理文件打开失败
			return std::string().c_str();
		}
		// 读取文件内容到std::string
		std::string code((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());
		// 转换为const char*
		return code.c_str();
	}

	template<typename Component>
	static void CopyComponent(entt::registry& src, entt::registry& tar,entt::entity& srcentity,entt::entity&tarentity)
	{
		if (src.has<Component>(srcentity))
		{
			auto& component = src.get<Component>(srcentity);
			tar.emplace_or_replace<Component>(tarentity, component);
		}
	}
	static b2BodyType BodyTypeTob2BodyType(Rigidbody2DComponent::BodyType type)
	{
		switch (type)
		{
		case ENGINE::Rigidbody2DComponent::BodyType::Static:
			return b2BodyType::b2_staticBody;
		case ENGINE::Rigidbody2DComponent::BodyType::Kinematic:
			return b2BodyType::b2_kinematicBody;
		case ENGINE::Rigidbody2DComponent::BodyType::Dynamic:
			return b2BodyType::b2_dynamicBody;
		}
		LOG_CORE_ERROR("错误的刚体类型");
		return b2BodyType::b2_staticBody;
	}
	Scene::Scene()
	{

	}
	Entity Scene::CreateEntity(const std::string &name)
	{
		auto result= Entity(m_Registry.create(),this);
		result.AddComponent<TransformComponent>();
		auto& tag=result.AddComponent<TagComponent>(name);
		tag.tag = name.empty() ? "Entity" : name;
		return result;
	}
	void Scene::OnUpdate(Time t)
	{
		if(!m_Registry.empty())
		{ 
			//CScript
			{
				auto view = m_Registry.view<CScriptComponent>();
				for (auto entity : view)
				{
					auto& csc = view.get<CScriptComponent>(entity);
					if (csc.Instance.use_count() == 0)
					{
						//输入路径
						if (!csc.Path.empty())
						{
							ScriptableEntity *se= new ScriptableEntity(csc.Path.c_str(),"Player",(int)entity,this);
							se->m_entity = Entity(entity, this);
							csc.Instance.reset(se);
							csc.Instance->OnCreate();
						}

					}

					if (csc.Instance.use_count() != 0)
					{
						csc.Instance->OnUpdate(t.GetSecond());
					}

				}
			}
			//Physic2D
			{
				auto view = m_Registry.view<TransformComponent, Rigidbody2DComponent>();
				if (view)
				{
					int32 velocityIterations = 6;   // 速度约束迭代次数
					int32 positionIterations = 2;   // 位置约束迭代次数		
					if(m_b2World)m_b2World->Step(t.GetSecond(), velocityIterations, positionIterations);
				}
				for(auto entity:view)
				{

					auto& trans = view.get<TransformComponent>(entity);
					auto& rgd2d =view.get<Rigidbody2DComponent>(entity);

					b2Body* body = (b2Body*)rgd2d.RuntimeBody;
					if (body->GetType() == b2_kinematicBody)
					{
						b2Vec2 currentPos = body->GetPosition();
						b2Vec2 targetPos = { trans.Translate.x,trans.Translate.y };
						b2Vec2 newPos = currentPos + 0.1f * (targetPos - currentPos); // 插值
						body->SetTransform(newPos, body->GetAngle());
					}
					else
					{
						const auto& position = body->GetPosition();

						const auto& angle = body->GetAngle();

						trans.Translate.x = position.x;
						trans.Translate.y = position.y;
						trans.Rotation.z = angle;
					}

				}

			}
			//Render2D
			{
				Camera* m_Camera = nullptr;
				glm::mat4 trans;

				auto view = m_Registry.view<TransformComponent, CameraComponent>();
				for (auto entity : view)
				{
					auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
					if (camera.Primary)
					{
						m_Camera = &camera.camera;
						trans = transform.GetTransform();
						break;
					}
				}
				if (m_Camera)
				{
					Render2D::BeginScene(*m_Camera, trans);
					//矩形
					{
						auto view = m_Registry.view<TransformComponent, RenderQuadComponent>();
						for (auto entity : view)
						{
							auto [transform, rc] = view.get<TransformComponent, RenderQuadComponent>(entity);
							if (rc.Texture.get())
							{
								Render2D::DrawQuad(transform.GetTransform(), rc.color, rc.Texture, (int)entity);
							}
							else
							{
								Render2D::DrawQuad(transform.GetTransform(), rc.color, (int)entity);
							}
						}
					}
					//圆形
					{
						auto view = m_Registry.view<TransformComponent, RenderCircleComponent>();
						for (auto entity : view)
						{
							auto [transform, rc] = view.get<TransformComponent, RenderCircleComponent>(entity);
							if (rc.Texture.get())
							{
								Render2D::DrawCircle(transform.GetTransform(), rc.color,rc.Texture, rc.thickness, rc.fade, (int)entity);
							}
							else
							{
								Render2D::DrawCircle(transform.GetTransform(), rc.color, rc.thickness, rc.fade, (int)entity);
							}
						}
					}
					Render2D::EndScene();
				}
			}
			//Audio
			{
				auto view = m_Registry.view<AudioComponent>();
				for (auto entity : view)
				{
					auto& ac = view.get<AudioComponent>(entity);
					if(ac.loop)ac.Play();
					
				}
			}
		}
	}
	void Scene::OnUpdateEditor(Time s, const EditorCamera& camera)
	{

		if(!m_Registry.empty())
		{
			Render2D::BeginScene(camera);
			//按钮
			{
				auto view = m_Registry.view<TransformComponent, ButtonComponent>();
				for (auto entity : view)
				{
					auto [transform, bc] = view.get<TransformComponent, ButtonComponent>(entity);

					Render2D::DrawButton(transform.GetTransform(), bc.Path, bc.Text, bc.TextColor,bc.BackColor,(int)entity);
				}
			}
			//标签
			{
				auto view = m_Registry.view<TransformComponent, LableComponent>();
				for (auto entity : view)
				{
					auto [transform, bc] = view.get<TransformComponent, LableComponent>(entity);

					Render2D::DrawLable(transform.GetTransform(), bc.Path, bc.Text, bc.TextColor,(int)entity);
				}
			}
			//矩形
			{
				
				auto view = m_Registry.view<TransformComponent, RenderQuadComponent>();
				for (auto entity : view)
				{
					auto [transform, rc] = view.get<TransformComponent, RenderQuadComponent>(entity);
					if (rc.Texture.get())
					{
						Render2D::DrawQuad(transform.GetTransform(), rc.color, rc.Texture, (int)entity);
						
					}
					else
					{
						Render2D::DrawQuad(transform.GetTransform(), rc.color, (int)entity);
					}
				}
			}
			//圆形
			{
				auto view = m_Registry.view<TransformComponent, RenderCircleComponent>();
				for (auto entity : view)
				{
					auto [transform, rc] = view.get<TransformComponent, RenderCircleComponent>(entity);
					if (rc.Texture.get())
					{
						Render2D::DrawCircle(transform.GetTransform(), rc.color, rc.Texture, rc.thickness, rc.fade, (int)entity);
					}
					else
					{
						Render2D::DrawCircle(transform.GetTransform(), rc.color, rc.thickness, rc.fade, (int)entity);
					}
				}
			}

			Render2D::EndScene();
		}
	}
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		if (m_ViewportWidth == width && m_ViewportHeight == height)
			return;

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		if (!m_Registry.empty())
		{
			// Resize our non-FixedAspectRatio cameras
			auto view = m_Registry.view<CameraComponent>();
			for (auto entity : view)
			{
				auto& cameraComponent = view.get<CameraComponent>(entity);
				if (!cameraComponent.FixedAspectRatio)
					cameraComponent.camera.SetViewportSize(width, height);
			}
		}
	}
	void Scene::DestoryEntity(const Entity& entity)
	{
		this->m_Registry.destroy(entity);
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> res = std::make_shared<Scene>();
		res->m_ViewportHeight = other->m_ViewportHeight;
		res->m_ViewportWidth = other->m_ViewportWidth;
		
		auto& SrcRegistry = other->m_Registry;
		auto& target = res->m_Registry;
		target.clear();
		
		SrcRegistry.each(
			[&](auto srcid)
			{
				auto tarid = target.create();
				CopyComponent<TagComponent>(SrcRegistry, target,srcid,tarid);
				CopyComponent<TransformComponent>(SrcRegistry, target, srcid, tarid);
				CopyComponent<RenderQuadComponent>(SrcRegistry, target, srcid, tarid);
				CopyComponent<RenderCircleComponent>(SrcRegistry, target, srcid, tarid);
				CopyComponent<CameraComponent>(SrcRegistry, target, srcid, tarid);
				CopyComponent<CScriptComponent>(SrcRegistry, target, srcid, tarid);
				CopyComponent<Rigidbody2DComponent>(SrcRegistry, target, srcid, tarid);
				CopyComponent<AudioComponent>(SrcRegistry, target, srcid, tarid);

				CopyComponent<ButtonComponent>(SrcRegistry, target, srcid, tarid);
				CopyComponent<LableComponent>(SrcRegistry, target, srcid, tarid);
			}
		);

		return res;
	}

	void Scene::OnRuntimeStart()
	{
		//创建body
		if (!m_Registry.empty())
		{
			auto view = m_Registry.view<TransformComponent,Rigidbody2DComponent>();
			if (view)
			{
				////输入重力创建世界
				b2Vec2 gravity(0.0f, -9.8f);
				m_b2World = new b2World(gravity);
			}
			for (auto entity : view)
			{
				auto& trans =view.get<TransformComponent>(entity);
				auto& rgd2d =view.get<Rigidbody2DComponent>(entity);

				b2BodyDef bodydef;
				bodydef.position.Set(trans.Translate.x, trans.Translate.y);
				bodydef.angle = trans.Rotation.z;
				bodydef.type = BodyTypeTob2BodyType(rgd2d.Type);
				bodydef.fixedRotation = rgd2d.FixedRotation;

				b2Body* body = m_b2World->CreateBody(&bodydef);

				rgd2d.RuntimeBody = body;

				b2PolygonShape bodyshape;
				if (m_Registry.has<RenderQuadComponent>(entity))
				{
					bodyshape.SetAsBox(trans.Scale.x * rgd2d.size.x, trans.Scale.y * rgd2d.size.y);
				}
				else if (m_Registry.has<RenderCircleComponent>(entity))
				{

					//近似模拟圆形，只支持最大8边形
					float a = trans.Scale.x * rgd2d.size.x; // X 半径
					float b = trans.Scale.y * rgd2d.size.y; // Y 半径
					const int numSegments = 36;
					//若需更多顶点，需修改 b2Settings.h 中的 b2_maxPolygonVertices（不推荐破坏兼容性）。已更改为36 影响性能
					// 生成顶点
					b2Vec2 vertices[numSegments];
					vertices->SetZero();
					for (int i = 0; i < numSegments; ++i) {
						float theta = 2.0f * b2_pi * i / numSegments;
						vertices[i].Set(a * cosf(theta), b * sinf(theta));
					}
					bodyshape.Set(vertices, numSegments);
				}
				else
				{
					//暂时设置
					bodyshape.SetAsBox(trans.Scale.x * rgd2d.size.x, trans.Scale.y * rgd2d.size.y);
				}
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &bodyshape;
				fixtureDef.density = rgd2d.Density;
				fixtureDef.friction = rgd2d.Friction;
				fixtureDef.restitution = rgd2d.Restitution;
				fixtureDef.restitutionThreshold = rgd2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);

			}
		}
	}

	void Scene::OnRuntimeEnd()
	{

		if (m_b2World)
		{
			delete m_b2World;
			m_b2World = nullptr;
		}
		//Audio
		{
			auto view = m_Registry.view<AudioComponent>();
			for (auto entity : view)
			{
				auto& ac = view.get<AudioComponent>(entity);
				ac.Stop();
			}
		}
	}

	void Scene::ShowEntitySize()
	{
		auto view = m_Registry.view<TagComponent>();
		std::cout << "实体个数为：" << view.size() << std::endl;
	}

	//Add
	template<typename T>
	void Scene::OnComponentAdded(T& componen)
	{

	}
	template<>
	void Scene::OnComponentAdded<TagComponent>(TagComponent& component)
	{

	}
	template<>
	void Scene::OnComponentAdded<TransformComponent>(TransformComponent& component)
	{

	}
	template<>
	void Scene::OnComponentAdded<CameraComponent>(CameraComponent& component)
	{
		component.camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		//判断是否存在其他相机
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cc = view.get<CameraComponent>(entity);
			cc.Primary = false;
		}
		component.Primary = true;
	}
	template<>
	void Scene::OnComponentAdded<RenderQuadComponent>(RenderQuadComponent& component)
	{

	}
	template<>
	void Scene::OnComponentAdded<CScriptComponent>(CScriptComponent& component)
	{		

	}
	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Rigidbody2DComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<RenderCircleComponent>(RenderCircleComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<AudioComponent>(AudioComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<ButtonComponent>(ButtonComponent& component)
	{

	}
	template<>
	void Scene::OnComponentAdded<LableComponent>(LableComponent& component)
	{

	}
	//Remove
	template<typename T>
	void Scene::OnComponentRemoved(T& conponent)
	{

	}
}