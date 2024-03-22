#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>


using namespace std;




BOOL setCursorToPosition(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}



bool isVirtualKeyPressed(int Virtkey)//INPUT
{
	return GetAsyncKeyState(Virtkey) & 0x8000;
}




int main()//Main function!!
{

	//reset movement amount
	int MoveX = 0, MoveY = 0;
	int decreasingY = 24;
	bool shooting = false;


	//Supressor cursor
	HANDLE hout;
	CONSOLE_CURSOR_INFO ConCurinf;
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	ConCurinf.dwSize = 10; ConCurinf.bVisible = FALSE;//hide
	SetConsoleCursorInfo(hout, &ConCurinf);


	//create background
	string BackgroundTXT;
	BackgroundTXT = "_______     _______     _______     _______     _______     _______";//ART, 


	//CONTROLS
	while (true)//MAIN is running
	{
		if (isVirtualKeyPressed(VK_RIGHT))
		{
			//right pressed
			++MoveX; //add one to MoveX Value MoveX=(MoveX+1);
		}
		else if (isVirtualKeyPressed(VK_LEFT))
		{
			//left pressed //Subract one to from MoveX Value MoveX=(MoveX-1);
			--MoveX;
		}

		else if (isVirtualKeyPressed(VK_SPACE))

		{
			//SPACE pressed
			if (shooting != true)
			{
				shooting = true;
			}
		}
		else if (isVirtualKeyPressed(VK_ESCAPE))
		{
			//ESC pressed
			break;
		}


#pragma region Rendering
		//Render stuff
		std::this_thread::sleep_for(std::chrono::milliseconds(10));//pause

		LockWindowUpdate(GetConsoleWindow());//suspend for flicker
		system("cls"); //clears screen, you are re-writing entire screen every loop!
		setCursorToPosition(30 + MoveX, 25);
	std:cout << "-^-";// Prints the "player Image" to screen

		//move cursor and set backgound
		setCursorToPosition(0, 20);
		std::cout << BackgroundTXT;


		//some LOGIC
		if (shooting == true)
		{
			setCursorToPosition(30 + MoveX, decreasingY--);
			std::cout << "!";
		}

		//reset Y when "missile" goes off screen (y<0)



		if (decreasingY < 0)
		{
			decreasingY = 25;
			shooting = false;
		}

		LockWindowUpdate(NULL);
#pragma endregion

	}

} //dont forget!!
