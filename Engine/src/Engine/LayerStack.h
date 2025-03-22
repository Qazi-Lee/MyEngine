#pragma once

#include"Layer.h"
namespace ENGINE
{
	//存放层的类：层栈
	class  LayerStack
	{
	public:
		LayerStack();
		~LayerStack();
		void PushLayer(Layer* layer);
		//覆盖层，应置于层栈的末尾保证最后渲染
		void PushOverLay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverLay(Layer* overlay);
		inline std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		inline std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*>m_Layers;
		//指向推入的层的末尾
		int InsertIndex;
};

}