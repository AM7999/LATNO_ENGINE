#include "../declarations/Application.h"
namespace Latno
{
	Application::~Application()
	{
		ImGui_ImplGlfwGL3_Shutdown();
		ImGui::DestroyContext();

		glfwTerminate();
	}

	void Application::BehaviorTick(double deltaTime)
	{
		for (int i = 0; i < Latno::BehaviorList.size(); i++)
		{
			Latno::BehaviorList[i]->Update(deltaTime);
		}
	}
	void Application::Load()
	{
		// To be overriden
	}
	void Application::BehaviorLoad()
	{
		for (int i = 0; i < Latno::BehaviorList.size(); i++)
		{
			Latno::BehaviorList[i]->Start();
		}
		AppLog = new DevLog("AppLog");

		DevLog::LOGLN("App Load Finished", "EngineLog");
	}
	bool Application::Tick(double deltaTime)
	{
		// To be overriden
		return true;
	}

	void Application::Startup()
	{
		if (!glfwInit())
		{
			return;
		}

		// vv Make GL Version core instead of compact vv
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// ^^ Make GL Version core instead of compat ^^


		window = glfwCreateWindow(WINDOW_LENGTH, WINDOW_HEIGHT, "LATNO ENGINE", NULL, NULL); // Creates window context
		if (!window)
		{
			glfwTerminate();
			return;
		}
		glfwSetWindowSizeLimits(window, WINDOW_LENGTH, WINDOW_HEIGHT, WINDOW_LENGTH, WINDOW_HEIGHT);
		glfwMakeContextCurrent(window);

		glfwSwapInterval(1); // Syncs swap interval with vsync

		if (glewInit() != GLEW_OK) // Checks glew state for errors
			std::cout << "ERROR!" << std::endl;

		// Enabling Blending
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// ^^^ INITIALIZATION ^^^
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		

		DevLog::CLEAR("EngineLog");
		DevLog::CLEAR("GL_ERROR_LOG");
		srand(time(NULL));
		
		Load();
		BehaviorLoad();
		Run();

		delete this;
	}

	void Application::Run()
	{
		Timer DeltaCalc; // Used to calculate deltaTime
		float prevDeltaTime = 0;

		while (true)
		{
			// vv RENDERING vv
			ImGui_ImplGlfwGL3_NewFrame();
			currentScene->Render();


			// vv TICK vv
			if (!Tick(prevDeltaTime))
				return;

			BehaviorTick(prevDeltaTime);
			
			prevDeltaTime = DeltaCalc.GetTime();
			DeltaCalc.Reset();

			ImGui::Render();

			GLOBAL_DELTA_TIME = prevDeltaTime;

			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(window);
			glfwPollEvents();

		}
	}
}