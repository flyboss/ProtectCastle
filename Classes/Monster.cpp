//
//  Monster.cpp
//  ProtectPrincess
//
//  
//
//
#include "Monster.h"

Monster* Monster::create(const std::string& filename)
{
	Monster *sprite = new Monster();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Monster* Monster::createWithSpriteFrameName(const std::string& filename)
{
	Monster *sprite = new Monster();
	if (sprite && sprite->initWithSpriteFrameName(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Monster::setLifeValue(int lifeValue)
{
	_lifeValue = lifeValue;
}

int Monster::getLifeValue()
{
	return _lifeValue;
}

void Monster::setHP(LoadingBar* HP)
{
	_HP = HP;
}

LoadingBar* Monster::getHP()
{
	return _HP;
}

void Monster::setHPInterval(double HPInterval)
{
	_HPInterval = HPInterval;
}

float Monster::getHpInterval()
{
	return _HPInterval;
}

void Monster::getInjured() { 
	
	_lifeValue - 1;
}