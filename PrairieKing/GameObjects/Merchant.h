#pragma once
#include "SpriteGo.h"
class Merchant :public SpriteGo
{
protected:
public:
	Merchant(const string& n = "");
	virtual ~Merchant() override;

	virtual void Init() override; //���� �Ҵ�(������)
	virtual void Release() override; //���� �Ҵ� ����(�Ҹ���)
	virtual void Reset() override;

	virtual void Update(float dt) override;
};

