#include <windowsx.h>

#include "Window.h"
#include "Texts.h"

LRESULT CALLBACK WindowFunction(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window* window = reinterpret_cast<Window *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (window)
		switch (message)
		{
		case WM_PAINT:
			window->Draw();
			ValidateRect(hWnd, NULL);
			return 0;
			break;
		case WM_SIZE:
			window->Resize((int)LOWORD(lParam), (int)HIWORD(lParam));
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_LBUTTONDOWN:
			window->MouseLeftDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_LBUTTONUP:
			window->MouseLeftUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_RBUTTONDOWN:
			window->MouseRightDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_RBUTTONUP:
			window->MouseRightUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_MOUSEMOVE:
			window->MouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;

		case WM_KEYDOWN:
			window->KeyDown((unsigned)wParam);
			break;
		case WM_KEYUP:
			if (wParam == VK_ESCAPE)
				PostQuitMessage(0);
			else
				window->KeyUp((unsigned)wParam);
			break;
		
		default:
			break;
		}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

const wchar_t* Window::GetErrorText(Window::Error errorId)
{
	switch (errorId)
	{
	case REGISTERCLASS:
		return L"Class registration failed";
	case CREATEWINDOW:
		return L"Window creation failed";
	case GETDC:
		return L"Cannot get device context";
	case CHOOSEPIXELFORMAT:
		return L"Cannot choose pixel format";
	case SETPIXELFORMAT:
		return L"Cannot set pixel format";
	case CREATERC:
		return L"Resource context creation failed";
	case MAKECURRENTRC:
		return L"Cannot make resource context as current";
	}
	return L"";
}

Window::Window()
	: hWnd(NULL),
		hDC(NULL),
		hRC(NULL),
		game(NULL)
{
}

Window::~Window()
{
}


Window::Error Window::Create(const wchar_t* className, const wchar_t* title, int width, int height)
{
	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	WNDCLASS	wc;
	DWORD		style;
	DWORD		extStyle;
	RECT		wndRect;
	int			pixelFormat;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowFunction;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = className;
	if (!RegisterClass(&wc))
		return REGISTERCLASS;

	style = WS_CAPTION | WS_OVERLAPPED;
	extStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	wndRect.left = 0;
	wndRect.right = width;
	wndRect.top = 0;
	wndRect.bottom = height;
	AdjustWindowRectEx(&wndRect, style, FALSE, extStyle);

	hWnd = CreateWindowEx(
					extStyle,
					className,
					title,
					style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
					0, 0, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top,
					NULL,
					NULL,
					GetModuleHandle(NULL),
					NULL);
	if (!hWnd)
		return CREATEWINDOW;
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	hDC = GetDC(hWnd);
	if (!hDC)
		return GETDC;
	Texts::SetDC(hDC);

	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (!pixelFormat)
		return CHOOSEPIXELFORMAT;
	if (!SetPixelFormat(hDC, pixelFormat, &pfd))
		return SETPIXELFORMAT;
	hRC = wglCreateContext(hDC);
	if (!hRC)
		return CREATERC;
	if (!wglMakeCurrent(hDC, hRC))
		return MAKECURRENTRC;

	return NONE;
}

void Window::Destroy(const wchar_t* className)
{
	Texts::Destroy();

	if (hRC)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
		hRC = NULL;
	}

	if (hDC)
	{
		ReleaseDC(hWnd, hDC);
		hDC = NULL;
	}

	if (hWnd)
	{
		DestroyWindow(hWnd);
		hWnd = NULL;
	}

	UnregisterClass(className, GetModuleHandle(NULL));
}

void Window::Init(int iW, int iH)
{
	if (game)
		game->Init(iW, iH);
}

void Window::Show()
{
	ShowWindow(hWnd, SW_SHOW);
}

void Window::Hide()
{
	ShowWindow(hWnd, SW_HIDE);
}

void Window::Resize(int iW, int iH)
{
	if (game)
		game->Resize(iW, iH);
}

void Window::Draw()
{
	if (game)
		game->Draw();
	SwapBuffers(hDC);
}

void Window::Update()
{
	if (game)
		game->Update();
}

void Window::MouseLeftDown(int xPos, int yPos)
{
	if (game)
		game->MouseLeftDown(xPos, yPos);
}

void Window::MouseLeftUp(int xPos, int yPos)
{
	if (game)
		game->MouseLeftUp(xPos, yPos);
}

void Window::MouseRightDown(int xPos, int yPos)
{
	if (game)
		game->MouseRightDown(xPos, yPos);
}

void Window::MouseRightUp(int xPos, int yPos)
{
	if (game)
		game->MouseRightUp(xPos, yPos);
}

void Window::MouseMove(int xPos, int yPos)
{
	if (game)
		game->MouseMove(xPos, yPos);
}

KeyCode Window::RemapKeyCode(unsigned keyCode)
{
	switch (keyCode)
	{
	case VK_LEFT:
		return ARROWLEFT;
	case VK_RIGHT:
		return ARROWRIGHT;
	case VK_UP:
		return ARROWUP;
	case VK_DOWN:
		return ARROWDOWN;
	case VK_SPACE:
		return SPACE;
	default:
		break;
	}
	return UNSUPPORTED;
}

void Window::KeyDown(unsigned keyCode)
{
	if (game)
		game->KeyDown(RemapKeyCode(keyCode));
}

void Window::KeyUp(unsigned keyCode)
{
	if (game)
		game->KeyUp(RemapKeyCode(keyCode));
}
