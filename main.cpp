#include "Application.h"
#include "Window.h"

constexpr auto CLASSNAME = L"win-base-window";
constexpr auto WINDOWTITLE = L"win-base";
constexpr int WINDOWWIDTH = 1024;
constexpr int WINDOWHEIGHT = 768;
constexpr int FPS = 20;

int main()
{
	Application		application;
	Window			window;
	Window::Error	error;
	//Game*			game;

	application.PrepareRandoms();

	error = window.Create(CLASSNAME, WINDOWTITLE, WINDOWWIDTH, WINDOWHEIGHT);
	if (error != Window::NONE)
	{
		MessageBox(NULL, Window::GetErrorText(error), L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	//game = new Game();
	//window.SetGame(game);

	window.Init(WINDOWWIDTH, WINDOWHEIGHT);
	window.Show();

	application.GameLoop(&window, FPS);

	window.Hide();
	window.Destroy(CLASSNAME);

	//delete game;

	return 0;
}