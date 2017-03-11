#include "MonsterBase.h"



MonsterBase::MonsterBase()
{
}


MonsterBase::~MonsterBase()
{
}

void MonsterBase::setName(std::string name)
{
	_name = name;
}

std::string MonsterBase::getName()
{
	return _name;
}

void MonsterBase::setGold(int gold)
{
	_gold = gold;
}

int MonsterBase::getGold()
{
	return _gold;
}
void MonsterBase::setLifeValue(int lifeValue)
{
	_lifeValue = lifeValue;
}

int MonsterBase::getLifeValue()
{
	return _lifeValue;
}

void MonsterBase::setHP(LoadingBar* HP)
{
	_HP = HP;
}

LoadingBar* MonsterBase::getHP()
{
	return _HP;
}

void MonsterBase::setHPInterval(double HPInterval)
{
	_HPInterval = HPInterval;
}

float MonsterBase::getHpInterval()
{
	return _HPInterval;
}