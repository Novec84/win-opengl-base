#pragma once

#include "Font.h"

#include <map>

#include <Windows.h>

class Texts
{
	static HDC dc;
	static std::map<unsigned, Font*> fonts;
public:
	Texts() = delete;

	static void SetDC(HDC newDC) { dc = newDC; }
	static void Destroy();

	static unsigned CreateFont(const wchar_t* name, int height, bool bold, bool cursive, bool underscore, bool crossed);

	static int GetTextHeight(unsigned fontId);
	static int GetTextWidth(unsigned fontId, const char* fmt, ...);
	static void DrawText(unsigned fontId, double x, double y, const char* fmt, ...);
};
