#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#include <conio.h>
#include "Renderer.h"

#define VK_W 0x57//defining WASD
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
//#include "ConsoleEngine.h" 


using namespace std;



enum class Direction { up, down, right, left, none };



int RandomNumberBetween(int minValue, int maxValue)
{
	return minValue + rand() % ((maxValue + 1) - minValue);

}



class MovingObject
{
public:
	//Constructor
	MovingObject(const string& txt, const Point& position, float speed = 1.0f) :
		m_txtSprite{ txt }, m_position{ position }, speed{ speed }
	{

	}


	bool Move(float addX, float addY, std::optional<Rect> boundingRect = std::nullopt)
	{
		bool doMove{ true };
		Point newPosition{ m_position };

		newPosition.x += addX;//adds to newposition
		newPosition.y += addY;

		//check can move inside bounding box.
		//if (boundingRect)
		//{
		//	//is object in boundry rect after move  + GetWidth()
		//	if (newPosition.x + GetSpriteWidth() - 1 > boundingRect->right
		//		|| newPosition.x  < boundingRect->left
		//		|| newPosition.y  < boundingRect->top
		//		|| newPosition.y > boundingRect->bottom) 
		//	{
		//		doMove = false;
		//	}
		//}

		if (doMove) {
			m_position = newPosition;///this is what and when position Value actually changes 
		}

		return doMove;
	}

	bool MoveX(float addX, std::optional<Rect> boundingRect = std::nullopt)
	{
		return Move(addX, 0, boundingRect);
	}

	bool MoveY(float addY, std::optional<Rect> boundingRect = std::nullopt)
	{
		return Move(0, addY, boundingRect);
	}

	bool Move(Direction movedirection, std::optional<Rect> boundingRect = std::nullopt)//direction only
	{
		m_direction = movedirection;
		return Move(boundingRect);
	}


	//Helpers

	void SetSpeed(float newSpeed)//why float, cant use here
	{
		speed = newSpeed;
	}

	float GetSpeed() const {
		return speed;
	}

	void SetDirection(Direction NewDir)
	{
		m_direction = NewDir;
	}

	Direction GetDirection() const
	{
		return m_direction;
	}
	void SetPosition(const Point& position)//teleport
	{
		m_position = position;
	}

	const std::string& GetSpriteTXT() const {//get text
		return  m_txtSprite;
	}

	const Point& GetSpritePos() const { //get position
		return m_position;
	}

	int GetSpriteWidth() const {//get width in characters
		return m_txtSprite.length();
	}

	bool Move(std::optional<Rect> boundingRect = std::nullopt)
	{
		bool domove{ false };

		switch (m_direction)
		{

		case::Direction::up:
			domove = MoveY(-speed, boundingRect);
			break;

		case::Direction::down:
			domove = MoveY(speed, boundingRect);
			break;

		case::Direction::right:
			MoveX(speed, boundingRect);
			domove = MoveX(speed, boundingRect);
			break;

		case::Direction::left:
			domove = MoveX(-speed, boundingRect);
			break;


		}
		return domove;
	}

private:
	std::string m_txtSprite;
	Point m_position;
	float speed{ 1.0f };
	Direction m_direction{ Direction::right };//default direction for all objects.


};

bool isVirtualKeyPressed(int Virtkey)//INPUT
{
	return GetAsyncKeyState(Virtkey) & 0x8000;//memory mask
}

int getChar()
{
	return static_cast<int>(_getwch());
}

class Game
{
public:
	Game();
	void RunGameLoop();
	void SetupGame();

private:
	void UpdateInput();
	void UpdateGameLogic();
	void RenderGame();


	bool isRunningGame;
	Rect playRectArea;
	int FrameSpeed;
	bool shooting;
	bool losemenu = false;

	//create player and enemy.
	MovingObject thePlayer{ " ^ ", { 20,35 }, 1.0f };
	MovingObject theEnemy{ "/~~~\\", { 7,7 }, 1.0f };
	MovingObject Missile{ "!", { 100, 100 }, 1.0f };//redundant?
	std::vector<MovingObject> spaceInvaders;
	std::vector<MovingObject> Missiles;

	bool missilesNotMade = true;
	int numMissilesOnScreen = 0;
	int maxMissiles = 5;

	string BackgroundTXT = "_______     _______     _______     _______     _______     ________      ________      ________";//ART!!, 
	string man[3] = { "\\o/", " | ", "/ \\" };
	string playerTxt[3] = { " ^ ","/_\\","   " };
	string winTxt[4] = { "___","| |" ,"@@@","  " };
	string invader[5] = { "\\   /","(*_*)"," { }" };
	string invader2[5] = { "\\   /","(O_O)"," {-}" };

	string GameOverTXTsprite[40] =
	{ "_______  _______  __   __  _______   ",
	"|       ||   _   ||  |_|  ||       |  ",
	"|    ___||  |_|  ||       ||    ___|  ",
	"|   | __ |       ||       ||   |___   ",
	"|   ||  ||       ||       ||    ___| ",
	"|   |_| ||   _   || ||_|| ||   |___   ",
	"|______ ||__| |__||_|   |_|| _______| " };

	string GameOVER_TXTsprite[40] =
	{ " _______   __   __  _______  ______",
	   "|       ||  | |  ||       ||    _ |",
	   "| _     ||  |_|  ||    ___||   | ||",
	   "|  | |  ||       ||   |___ |   |_||__",
	   "|  |_|  ||       ||    ___||      __  |",
	   "|       | |     | |   |___ |     |  | |",
	   "| _______|  |___|  |_______|| ___|  |_|" };

	string endMenuTXT[20] = { "Press N for NewGame","Press X to Exit" };

	string stickman0[10] =
	{ "   O   ",
	 "  /|\\_",
	 " / |   ",
	 "__/ \\  ",
	 "     \\ " };
	string stickman1[10] =
	{ "   O  ",
	 "  /|_  ",
	 "  \\|\\  ",
	 "  / \\    ",
	 " /  |   " };
	string stickman2[10] =
	{ "   O  ",
	 "   |  ",
	 "   |\\  ",
	 "  _|  ",
	 "   |  " };

	string stickman3[10] =
	{ "   O  ",
	 "   |-  ",
	 "   |_  ",
	 "  /  \\ ",
	 " |     " };

	bool flipAnim = true;
	int score = 1;
	string scoreTXT = "SCORE: ";
	string scoreString = "SCORE: 0";
	bool invadersNeeded = true;
	int numofInvaders = 7;
	int delayCount = 0;
	int delay = 5;
	int HomeCoord = 30;
	bool animisPlaying = true;
	int animframe = 0;
	int animdelay = 6;
	int animdelayCount = 0;
};

Game::Game()
{

	//DATA-----------------------------------------------
	//set inital values for player, enemy and missile
	playRectArea.left = 5;//Measured in character,from top left (0,0).
	playRectArea.top = 5;
	playRectArea.right = 55;
	playRectArea.bottom = 28;

	FrameSpeed = 33; //update every 60 milliseconds
	shooting = false;
	isRunningGame = true;
	//thePlayer.SetDirection(Direction::right);
	theEnemy.SetDirection(Direction::down);
	Missile.SetDirection(Direction::up);
	int numMissilesOnScreen = 0;
	int maxMissiles = 5;
	int delayCount = 0;
	int delay = 5;



}

void Game::RunGameLoop()
{
	auto previousFrameTime = std::chrono::steady_clock::now();
	auto currentFrameTime = std::chrono::steady_clock::now();

	while (isRunningGame)
	{
		currentFrameTime = std::chrono::steady_clock::now();
		int frameDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - previousFrameTime).count();

		if (frameDurationMs > FrameSpeed)
		{
			previousFrameTime = currentFrameTime;
			UpdateInput();
			UpdateGameLogic();
			RenderGame();
		}


	}
}

void Game::UpdateInput()
{
	if (isVirtualKeyPressed(VK_D))
	{
		thePlayer.Move(Direction::right, playRectArea);
	}
	else if (isVirtualKeyPressed(VK_A))
	{
		thePlayer.Move(Direction::left, playRectArea);
	}
	else if (isVirtualKeyPressed(VK_S) && thePlayer.GetSpritePos().y < playRectArea.bottom)
	{
		thePlayer.Move(Direction::down, playRectArea);
	}
	else if (isVirtualKeyPressed(VK_W))
	{
		thePlayer.Move(Direction::up, playRectArea);

	}

	else if (isVirtualKeyPressed(VK_SPACE)) //SPACE pressed
	{
		if (delayCount > delay)
		{
			shooting = true;//??

			if (numMissilesOnScreen < maxMissiles)//Missiles available check
			{
				//Fire! Missile!
				//add missile and fire
				numMissilesOnScreen++;
				int fireMissileID = numMissilesOnScreen - 1;//??

				if (Missiles.size() > 0)
				{//place missile at player position
					Missiles[fireMissileID].SetPosition({ (thePlayer.GetSpritePos().x + 1),(thePlayer.GetSpritePos().y) });
				}

			}
			delayCount = 0;//reset delay


		}
		else { delayCount++; }




	}
	else if (isVirtualKeyPressed(VK_ESCAPE)) //ESC pressed
	{
		isRunningGame = false;

	}
}


void Game::UpdateGameLogic()
{

	//Missiles
	//move forward
	if (numMissilesOnScreen > 0)
	{
		//Missile.Move();
		for (int i = 0; i < numMissilesOnScreen; i++)
		{
			Missiles[i].Move();
		}
	}

	//remove missile when off screem
	if (Missiles.size() > 0)
	{
		for (int i = 0; i < Missiles.size(); i++)//Check no. of missiles on screen remove from count
		{
			if (Missiles[i].GetSpritePos().y < 5)
			{
				Missiles[i].SetPosition({ 6,6 });
				numMissilesOnScreen--;
			}
		}
	}


	//Random
	int chance = (RandomNumberBetween(1, 10));//random number between 1 and 6

	//move and check invaders
	for (int i = 0; i < spaceInvaders.size(); i++)
	{
		int chance = (RandomNumberBetween(1, 24));//random number between 1 and 6

		if (chance == 2)
		{
			spaceInvaders[i].Move();
		}

		if (spaceInvaders[i].GetSpritePos().y > 32)//check if invaders at bottom of screen.
		{
			spaceInvaders[i].SetPosition({ spaceInvaders[i].GetSpritePos().x, 1 });
		}
	}


	//animation
	if (chance > 7)
	{
		flipAnim = !flipAnim;

	}
	else {

	}

	///COLLISION AND PHYSICS?	
//COLLISION--
//bool checkForCollision(MovingObject firstObject, MovingObject secondObject)
//{	//}	 
//////////////////////////Missile collision
	for (int j = 0; j < numMissilesOnScreen; j++)
	{

		for (int i = 0; i < spaceInvaders.size(); i++)
		{


			if ((Missiles[j].GetSpritePos().x >= spaceInvaders[i].GetSpritePos().x - 2) && (Missiles[j].GetSpritePos().x <= spaceInvaders[i].GetSpritePos().x + 3))
			{
				if (Missiles[j].GetSpritePos().y == spaceInvaders[i].GetSpritePos().y)
				{
					//hit
					spaceInvaders[i].SetPosition({ 5, 5 });

					//REWARD THE PLAYER, EXPLOSION,SOUND
					score++;
					string scoreValueasTXT = to_string(score);// turn int value into text
					scoreString = scoreTXT + scoreValueasTXT;
				}
			}

		}
	}

	//check for lose-invasion sucess
	for (int i = 0; i < spaceInvaders.size(); i++)
	{

		if (spaceInvaders[i].GetSpritePos().y > HomeCoord)
		{
			losemenu = true;
			isRunningGame = false;
		}
	}

	//TODO WIN SCENARIO

	//if (levelUp)
	//{
	//	//for (int i = 0; i < levelup->length(); i++)
	//	//{
	//	renderer.DrawtoScreen("LEVEL UP!!!  C|_|", { 10  , 10 });
	//	// }
	//}*/
}

void Game::RenderGame()
{
	//RENDERING :	------------------------------------------------------------------------------------------	
	Renderer renderer;

	renderer.DrawtoScreen(BackgroundTXT, { 5,HomeCoord });


	for (int i = 0; i < 7; i++) //draw spaceinvaders
	{
		renderer.DrawtoScreen(spaceInvaders[i].GetSpriteTXT(), spaceInvaders[i].GetSpritePos());
	}

	//draw onscreen missiles		
	for (int i = 0; i < numMissilesOnScreen; i++)
	{
		renderer.DrawtoScreen(Missiles[i].GetSpriteTXT(), Missiles[i].GetSpritePos());
	}

	//other
	//renderer.DrawSpritetoScreen(man, { 10,20 });
	//renderer.DrawSpritetoScreen(winTxt, { 25,10 });

	/*if (flipAnim == true)
	{
		renderer.DrawSpritetoScreen(invader, { 5,5 });

	}else	{
		renderer.DrawSpritetoScreen(invader2, { 5,5 });

	}*/
	animdelayCount++;

	if (animdelayCount > animdelay)
	{
		animdelayCount = 0;
	}
	if (animisPlaying)
	{
		if (animframe < 4)//nextframe
		{
			animframe++;
		}
		else {
			animframe = 0;
		}
	}

	//int xx = thePlayer.GetSpritePos().x-2;
	//int yy = thePlayer.GetSpritePos().y-10;

	int xx = 10;
	int yy = 10;

	switch (animframe)//show frame
	{
	case 0:
		//print frame0
		renderer.DrawSpritetoScreen(stickman0, { xx,yy });
		break;

	case 1:
		//print frame1
		renderer.DrawSpritetoScreen(stickman1, { xx,yy });
		break;
	case 2:
		//print frame2
		renderer.DrawSpritetoScreen(stickman2, { xx,yy });
		break;

	case 3:
		renderer.DrawSpritetoScreen(stickman3, { xx,yy });
		break;
	case 4:
		renderer.DrawSpritetoScreen(stickman0, { xx,yy });
		break;


	}


	// UI Show score
	renderer.DrawtoScreen(scoreString, { 55,2 });
	string diag = to_string(numMissilesOnScreen);
	renderer.DrawtoScreen(diag, { 25,25 });
	//draw player last(on top
	renderer.DrawSpritetoScreen(playerTxt, thePlayer.GetSpritePos());

	if (losemenu)
	{
		//system("cls");
		renderer.DrawSpritetoScreen(GameOverTXTsprite, { 10,7 });
		//renderer.DrawSpritetoScreen(GameOVER_TXTsprite, { 10,13 });
		//renderer.DrawSpritetoScreen(endMenuTXT, { 17,22 });


	}

}


int main()
{
	StartRender();

	bool continuegame = true;
	bool EnemyAlive = true;
	bool levelUp = false;
	Game game;
	game.SetupGame();

	while (continuegame)
	{
		Game game;
		game.SetupGame();//??
		game.RunGameLoop();

		while (1)//endless loop waiting player to press x or n
		{
			if (_kbhit())
			{
				char ch = getChar();
				if (ch == 'n')//new game
				{

					break;
				}
				else {
					if (ch == 'x')//exit
					{
						continuegame = false;
						break;
					}

				}

			}

		}
	}
	return 0;
}

void Game::SetupGame()
{//INITIALISATION:	create invaders and missiles.//TODO

		//make invaders
	for (int i = 0; i < numofInvaders; i++)
	{
		int x = 12 + (i * 7);
		spaceInvaders.push_back({ "/~~~\\", { x,4} , 1.0f });
		spaceInvaders[i].SetDirection(Direction::down);
	}


	for (int i = 0; i < maxMissiles; i++)//create maxMissiles missiles.
	{
		int x = 12 + (i * 6);
		Missiles.push_back({ "!", { 25 + i,22} , 1.0f });
		Missiles[i].SetDirection(Direction::up);

	}


	return;
}
