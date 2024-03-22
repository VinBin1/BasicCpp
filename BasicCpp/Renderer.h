#pragma once
#include <string>

struct Point
{
	int x;
	int y;

};

struct Rect //could be two "Points"
{
	int left;
	int top;
	int right;
	int bottom;

};

void StartRender();

class Renderer
{
public:
	Renderer();
	~Renderer();

	void DrawtoScreen(const std::string& text, const Point& position);

	void DrawSpritetoScreen(const std::string text[], const Point& position);


};