#pragma once
#include "RectangleGo.h"
class Ground :public RectangleGo
{
protected:

public:
	Ground(sf::Vector2f& size, const string& n = "");
	virtual ~Ground() override;

	virtual void Init() override; //동적 할당(생성자)
	virtual void Reset() override;
	virtual void Update(float dt) override;
};

