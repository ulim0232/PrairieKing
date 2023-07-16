#include "stdafx.h"
#include "Ground.h"

Ground::Ground(sf::Vector2f& size, const string& n)
	:RectangleGo(size, n)
{
}

Ground::~Ground()
{
}

void Ground::Init()
{
	rectangle.setFillColor(sf::Color::Blue);
	SetOrigin(Origins::TC);
	SetPosition(0, 0);
}

void Ground::Reset()
{
}

void Ground::Update(float dt)
{
}
