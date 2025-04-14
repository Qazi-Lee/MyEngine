//#include<Engine.h>
/************程序入口*************/
#include"Engine/EntryPoint.h"
#include"imgui.h"
#include"Platform/OpenGL/OpenGLShader.h"
#include<glm/gtc/type_ptr.hpp>
#include"head/Layer2D.h"
#include"head/RunGame.h"
//#define ENGINE_EDITOR
class Game :public ENGINE:: Application
{
public:

	Game() 
	{
#ifdef ENGINE_EDITOR
		Layer2D* layer2D = new Layer2D();
		PushLayer(layer2D);

#else
		RunGame* game = new RunGame();
		PushLayer(game);
#endif // ENGINE_EDITOR



	};
	~Game() {};
};

ENGINE::Application* ENGINE::CreatApp()
{
	return new Game();
}
