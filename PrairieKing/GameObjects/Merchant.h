#pragma once
#include "SpriteGo.h"
class Merchant :public SpriteGo
{
protected:
public:
	Merchant(const string& n = "");
	virtual ~Merchant() override;

	virtual void Init() override; //동적 할당(생성자)
	virtual void Release() override; //동적 할당 해제(소멸자)
	virtual void Reset() override;

	virtual void Update(float dt) override;
};

