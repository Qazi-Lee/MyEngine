#include"pch.h"
#include"LayerStack.h"


namespace ENGINE {
	LayerStack::LayerStack()
	{
		InsertIndex = 0;
	}
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}
	void LayerStack::PushLayer(Layer* layer)
	{
		//非覆盖层置于层栈的前端，按照传递顺序优先渲染
		m_Layers.emplace(m_Layers.begin() + InsertIndex, layer);
		InsertIndex++;
		layer->OnAttach();
		
   }

	void LayerStack::PushOverLay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.begin() + InsertIndex)
		{
			layer->OnDetach();
			m_Layers.erase(it);
			InsertIndex--;
		}
	}

	void LayerStack::PopOverLay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			overlay->OnDetach();
			m_Layers.erase(it);
		}
	}
}