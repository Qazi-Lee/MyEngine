#include"pch.h"

#include"Scene.h"
#include"Entity.h"
#include"Engine/Render/Render2D.h"
#include"Component.h"

#include"box2d/box2d.h"


namespace ENGINE
{
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
			//Update Script
			{
				auto view = m_Registry.view<NativeScriptComponent>();
				for (auto entity : view)
				{
					auto& nsc = view.get<NativeScriptComponent>(entity);
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InitInstanceFunc();
						nsc.Instance->m_entity = Entity(entity, this);
						nsc.Instance->OnCreate();
					}
					nsc.Instance->OnUpdate(t);
				}

			}
			//Physic2D
			{

				int32 velocityIterations = 6;   // 速度约束迭代次数
				int32 positionIterations = 2;   // 位置约束迭代次数
				m_b2World->Step(t.GetSecond(), velocityIterations, positionIterations);
				auto view = m_Registry.view<TransformComponent, Rigidbody2DComponent>();
				for(auto entity:view)
				{
					auto& trans = view.get<TransformComponent>(entity);
					auto& rgd2d =view.get<Rigidbody2DComponent>(entity);

					b2Body* body = (b2Body*)rgd2d.RuntimeBody;
					const auto& position = body->GetPosition();
					
					const auto& angle = body->GetAngle();

					trans.Translate.x= position.x;
					trans.Translate.y= position.y;
					trans.Rotation.z = angle;
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
		}
	}
	void Scene::OnUpdateEditor(Time s, const EditorCamera& camera)
	{

		if(!m_Registry.empty())
		{
			Render2D::BeginScene(camera);
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
				CopyComponent<ScriptComponent>(SrcRegistry, target, srcid, tarid);
				CopyComponent<NativeScriptComponent>(SrcRegistry, target, srcid, tarid);
				CopyComponent<Rigidbody2DComponent>(SrcRegistry, target, srcid, tarid);
			}
		);

		return res;
	}

	void Scene::OnRuntimeStart()
	{
		////输入重力创建世界
		b2Vec2 gravity(0.0f, -9.8f);
		m_b2World = new b2World(gravity);
		//m_b2World = new b2World({2.0,1.0});
		//创建body
		if (!m_Registry.empty())
		{
			auto view = m_Registry.view<TransformComponent,Rigidbody2DComponent>();
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
				bodyshape.SetAsBox(trans.Scale.x * 0.5, trans.Scale.y * 0.5);
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
		delete m_b2World;
		m_b2World = nullptr;
	}

	void Scene::ShowEntitySize()
	{
		auto view = m_Registry.view<TagComponent>();
		std::cout << "实体个数为：" << view.size() << std::endl;
	}

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
	void Scene::OnComponentAdded<NativeScriptComponent>(NativeScriptComponent& component)
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
}