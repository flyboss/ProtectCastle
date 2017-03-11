//
//  MonsterData.cpp
//  ProtectPrincess
//
//  
//
//

#include "MonsterData.h"

MonsterData* MonsterData::create()
{
	MonsterData *sprite = new MonsterData();
	if (sprite)
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void MonsterData::setName(std::string name)
{
	_name = name;
}

std::string MonsterData::getName()
{
	return _name;
}

void MonsterData::setLifeValue(int lifeValue)
{
	_lifeValue = lifeValue;
}

int MonsterData::getLifeValue()
{
	return _lifeValue;
}

void MonsterData::setGold(int gold)
{
	_gold = gold;
}

int MonsterData::getGold()
{
	return _gold;
}

int MonsterData::getMonsterType()
{
	return _lifeValue;
}