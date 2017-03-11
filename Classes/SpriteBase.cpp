//
//  SpriteBase.cpp
//  ProtectPrincess
//
//  
//
//

#include "SpriteBase.h"

SpriteBase* SpriteBase::create(const std::string& filename)
{
	SpriteBase *sprite = new SpriteBase();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

SpriteBase* SpriteBase::createWithSpriteFrameName(const std::string& filename)
{
	SpriteBase *sprite = new SpriteBase();
	if (sprite && sprite->initWithSpriteFrameName(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void SpriteBase::setName(std::string name)
{
	_name = name;
}

void SpriteBase::setGold(int gold)
{
	_gold = gold;
}

int SpriteBase::getGold()
{
	return _gold;
}


std::string SpriteBase::getName()
{
	return _name;
}