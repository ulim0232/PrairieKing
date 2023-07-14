#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "DataTableMgr.h"
#include "ClipTable.h"
#include "ResourceMgr.h"
#include "SpriteGo.h"

void Player::Init()
{
	SpriteGo::Init();
}

void Player::Reset()
{
	SpriteGo::Reset();


}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);
	animation.Update(dt);
	float h = INPUT_MGR.GetAxis(Axis::Horizontal);
	float v = INPUT_MGR.GetAxis(Axis::Vertical);

	//이동
	velocity.x = h * speed;
	velocity.y = v * speed;
	position += velocity * dt;

	/*if (!wallBounds.contains(position))
	{
		position = 
		
		::Clamp(position, wallBoundsLT, wallBoundsRB);
	}*/

	SetPosition(position);

	//애니메이션
	
}
