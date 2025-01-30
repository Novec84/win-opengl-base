
#pragma once

#include <windows.h>

#include "Game.h"
#include "KeyCode.h"

class Window
{
	HWND		hWnd;
	HDC			hDC;
	HGLRC		hRC;

	Game*	game;

	KeyCode RemapKeyCode(unsigned keyCode);
public:
	enum Error
	{
		REGISTERCLASS,
		CREATEWINDOW,
		GETDC,
		CHOOSEPIXELFORMAT,
		SETPIXELFORMAT,
		CREATERC,
		MAKECURRENTRC,

		NONE
	};

	static const wchar_t* GetErrorText(Error errorId);

	Window();
	~Window();

	void SetGame(Game* newGame) { game = newGame; }

	Error Create(const wchar_t* className, const wchar_t* title, int width, int height);
	void Destroy(const wchar_t* className);
	void Init(int iW, int iH);
	void Show();
	void Hide();
	void Resize(int iW, int iH);
	void Draw();
	void Update();

	void MouseLeftDown(int xPos, int yPos);
	void MouseLeftUp(int xPos, int yPos);
	void MouseRightDown(int xPos, int yPos);
	void MouseRightUp(int xPos, int yPos);
	void MouseMove(int xPos, int yPos);

	void KeyDown(unsigned keyCode);
	void KeyUp(unsigned keyCode);
};
