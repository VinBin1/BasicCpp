#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "Renderer.h"




Renderer::Renderer()//constructor (first thing Object does)

{
	//Render stuff
	std::this_thread::sleep_for(std::chrono::milliseconds(10));//pause

	LockWindowUpdate(GetConsoleWindow());//suspend for flicker

	system("cls"); //clears screen, you are re-writing entire screen every loop!

}



// This does most the work!

BOOL setCursorToPosition(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}


void Renderer::DrawtoScreen(const std::string& text, const Point& position)
{
	setCursorToPosition(position.x, position.y);
	std::cout << text;
}

void Renderer::DrawSpritetoScreen(const std::string text[], const Point& position)
{
	for (int i = 0; i < text->length(); i++)
	{

		setCursorToPosition(position.x, position.y + i);
		std::cout << text[i];
	}

}


Renderer::~Renderer()
{
	LockWindowUpdate(NULL);
}

void StartRender()
{
	HWND console = GetConsoleWindow();
	RECT ConsoleRect;
	GetWindowRect(console, &ConsoleRect);
	MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 1000, 680, TRUE);//fixed size screen

	HANDLE hout;
	CONSOLE_CURSOR_INFO ConCurinf;
	hout = GetStdHandle(STD_OUTPUT_HANDLE);

	//Supressor cursor
	ConCurinf.dwSize = 10;
	ConCurinf.bVisible = FALSE;//hide

	SetConsoleCursorInfo(hout, &ConCurinf);

}


