#include"pch.h"
#include"ImGuiLayer.h"

#include"imgui.h"
#include"Engine/Application.h"
#include"backends/imgui_impl_glfw.h"
#include"backends/imgui_impl_opengl3.h"
#include"backends/imgui_impl_vulkan.h"
#include"Platform/Vulkan/VulkanContext.h"
#include"ImGuizmo.h"
#include<GLFW/glfw3.h>
#include"Engine/Window.h"
#include <vulkan/vulkan.h>
static ImGui_ImplVulkanH_Window* wd = nullptr;

static ENGINE::GraphicsContext* context = nullptr;
static VulkanInstance* instance = nullptr; 

static ENGINE::GraphicsAPI GetAPI()
{
	ENGINE::Application& app = ENGINE::Application::GetApp();
	ENGINE::GraphicsAPI api = app.GetWindow().GetGraphicsAPI();
	return api;
}
static GLFWwindow* GetGLFWwindow()
{
	ENGINE::Application& app = ENGINE::Application::GetApp();
	GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
	return window;
}
static GLFWwindow* window = nullptr;

namespace ENGINE {
	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer")
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		ENGINE::GraphicsAPI api = GetAPI();
		if (api == ENGINE::GraphicsAPI::VULKAN)
		{
			window = GetGLFWwindow();
			context= static_cast<ENGINE::GraphicsContext*>(ENGINE::Application::GetApp().GetWindow().GetContext());
			instance = static_cast<VulkanInstance*>(context->GetInstance());
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
			//io.ConfigViewportsNoAutoMerge = true;
			//io.ConfigViewportsNoTaskBarIcon = true;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			// Setup Platform/Renderer backends
			ImGui_ImplGlfw_InitForVulkan(window, true);

			// 创建专用于 ImGUI 的描述符池
			VkDescriptorPoolSize poolSizes[] = {
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 }, // 调整大小根据需要
			};

			VkDescriptorPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			poolInfo.maxSets = 1000; // 调整大小根据需要
			poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
			poolInfo.pPoolSizes = poolSizes;

			VkDescriptorPool descriptorPool;
			vkCreateDescriptorPool(instance->device, &poolInfo, nullptr, &descriptorPool);

			ImGui_ImplVulkan_InitInfo init_info = {};
			init_info.Instance = instance->instance;
			init_info.PhysicalDevice = instance->physicalDevice;
			init_info.Device = instance->device;
			init_info.QueueFamily = instance->FindQueueFamilies(instance->physicalDevice).graphicsFamily;
			init_info.Queue = instance->graphicsQueue;
			init_info.PipelineCache = VK_NULL_HANDLE;
			init_info.DescriptorPool = descriptorPool;
			init_info.Allocator = nullptr;
			init_info.MinImageCount = instance->querySwapChainSupport(instance->physicalDevice).capabilities.minImageCount;
			init_info.ImageCount = instance->querySwapChainSupport(instance->physicalDevice).capabilities.minImageCount+1;
			init_info.CheckVkResultFn = nullptr;
			ImGui_ImplVulkan_Init(&init_info,instance->renderPass);

			// 使用单次命令缓冲上传字体
			uint32_t imageIndex;
			vkAcquireNextImageKHR(instance->device, instance->swapChain, std::numeric_limits<uint64_t>::max(),
				instance->imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
			VkCommandBuffer commandBuffer = instance->commandBuffers[imageIndex]; // 创建或获取一个命令缓冲
			vkResetCommandBuffer(commandBuffer, 0);

			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			vkBeginCommandBuffer(commandBuffer, &beginInfo);

			ImGui_ImplVulkan_CreateFontsTexture(commandBuffer); // 上传字体

			vkEndCommandBuffer(commandBuffer);

			// 提交并等待完成
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;
			vkQueueSubmit(instance->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
			vkDeviceWaitIdle(instance->device);

			// 清理字体上传资源
			ImGui_ImplVulkan_DestroyFontUploadObjects();

			//{
			//	// Use any command queue
			//	VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
			//	VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;

			//	err = vkResetCommandPool(g_Device, command_pool, 0);
			//	check_vk_result(err);
			//	VkCommandBufferBeginInfo begin_info = {};
			//	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			//	begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			//	err = vkBeginCommandBuffer(command_buffer, &begin_info);
			//	check_vk_result(err);

			//	ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

			//	VkSubmitInfo end_info = {};
			//	end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			//	end_info.commandBufferCount = 1;
			//	end_info.pCommandBuffers = &command_buffer;
			//	err = vkEndCommandBuffer(command_buffer);
			//	check_vk_result(err);
			//	err = vkQueueSubmit(g_Queue, 1, &end_info, VK_NULL_HANDLE);
			//	check_vk_result(err);

			//	err = vkDeviceWaitIdle(g_Device);
			//	check_vk_result(err);
			//	ImGui_ImplVulkan_DestroyFontUploadObjects();
			//}
		}
		else if(api == ENGINE::GraphicsAPI::OPENGL)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			// Setup Platform/Renderer bindings
			Application& app = Application::GetApp();
			GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 460");
		}	
	}

	void ImGuiLayer::OnDetach()
	{
		ENGINE::GraphicsAPI api = GetAPI();
		if (api == ENGINE::GraphicsAPI::VULKAN)
		{
			ImGui_ImplVulkan_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

			//vkDestroyDescriptorPool(instance->device, descriptorPool, nullptr);

		}
		else if (api == ENGINE::GraphicsAPI::OPENGL)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

	}

	void ImGuiLayer::Begin()
	{
		ENGINE::GraphicsAPI api = GetAPI();
		if (api == ENGINE::GraphicsAPI::VULKAN)
		{
			// Start the Dear ImGui frame
			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
		else if (api == ENGINE::GraphicsAPI::OPENGL)
		{
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGuizmo::BeginFrame();
		}

	}

	void ImGuiLayer::OnImGuiRender()
	{
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);
	}


	void ImGuiLayer::End()
	{
		ENGINE::GraphicsAPI api = GetAPI();
		if (api == ENGINE::GraphicsAPI::VULKAN)
		{
			//GetWindow
			ImGuiIO& io = ImGui::GetIO();
			Application& app =Application::GetApp();
			io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			ImGui::Render();
			if (ImGui::GetDrawData()) {
				uint32_t imageIndex;
				vkAcquireNextImageKHR(instance->device, instance->swapChain, std::numeric_limits<uint64_t>::max(),
					instance->imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
				ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),instance->commandBuffers[0]);
			}
		}
		else if (api == ENGINE::GraphicsAPI::OPENGL)
		{
			//GetWindow
			ImGuiIO& io = ImGui::GetIO();
			Application& app = Application::GetApp();
			io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
			//Render
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			//docking
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
		}

	}



}


