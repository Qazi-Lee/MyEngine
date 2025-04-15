#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include"vulkan/vulkan.h"
#include<vector>
#include <set>
struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};
struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};
class VulkanInstance
{
public:
	VulkanInstance(GLFWwindow* window) :m_Window(window) {}
	//主循环
	void mainloop();
	void drawFrame();

	void Init();
	//创建实例
	void CreateInstance();
	//TODO:debug功能启用
	void SetupDebugMessenger();
	//TODO:非离屏渲染是要surface和交换链
	void CreateSurface();

	//判断交换链是否支持
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	//TODO:查询交换链细节,以及交换链参数设置
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	//设置交换链表面格式
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	//设置交换链呈现模式
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	//设置交换链交换范围
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	// 创建交换链（可添加至上下文设置中）
	void CreateSwapChain();
	//创建交换链中的图像视口（绑定交换链图像）
	void  CreateImageViews();
	//物理设备选择
	void PickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	//队列族号
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	//逻辑设备
	void CreateLogicalDevice();
	//着色器模块
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
	//渲染流程
	void CreateRenderPass();
	//图形管线
	void CreateGraphicsPipeline();
	//帧缓冲
	void CreateFramebuffers();
	//命令池
	void CreateCommandPool();
	//指令缓冲
	void CreateCommandBuffers();
	//信号量
	void CreateSemaphores();
	void Cleanup();
public:
	GLFWwindow* m_Window;
	VkInstance instance;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView>swapChainImageViews;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	//渲染过程信号量
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
public:
	//存储支持交换链
	const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
};

