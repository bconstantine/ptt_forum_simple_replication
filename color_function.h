#pragma once

#include <iostream>
#include <windows.h>

enum Color
{
	L_BLACK,
	L_BLUE,
	L_GREEN,
	L_CYAN,
	L_RED,
	L_PINK,
	L_YELLOW,
	L_WHITE,
	S_BLACK,
	S_BLUE,
	S_GREEN,
	S_CYAN,
	S_RED,
	S_PINK,
	S_YELLOW,
	S_WHITE,
};

void resetColor()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void setColor(int bgColor, int textColor)
{
	int colorNumber = (16 * bgColor) + textColor;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNumber);
}

void colorText(int bgColor, int textColor, string text)
{
	int colorNumber = (16 * bgColor) + textColor;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNumber);

	cout << text;

	resetColor();
}
