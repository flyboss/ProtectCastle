#include "MonsterDataOfWave.h"

MonsterDataOfWave* MonsterDataOfWave::create()
{
	MonsterDataOfWave *sprite = new MonsterDataOfWave();
	if (sprite)
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

MonsterData* MonsterDataOfWave::at(int pos)
{
	return _monsterDatas.at(pos);
}

bool MonsterDataOfWave::pushBack(MonsterData* monsterdata)
{
	_monsterDatas.pushBack(monsterdata);
	return true;
}

unsigned MonsterDataOfWave::size()
{
	return _monsterDatas.size();
}

bool MonsterDataOfWave::setCount(int monstercount)
{
	_monsterCount = monstercount;
	return true;
}

int MonsterDataOfWave::getCount()
{
	return _monsterCount;
}


bool MonsterDataOfWave::setDelivery(int monsterdelivery)
{
	_monsterDelivery = monsterdelivery;
	return true;
}

int MonsterDataOfWave::getDelivery()
{
	return _monsterDelivery;
}

bool MonsterDataOfWave::upgrade()
{
	for (int i = 0; i != size(); i++)
	{
		auto monsterdata = at(i);
		int lifevalue = monsterdata->getLifeValue();
		if (lifevalue < 30)
		{
			lifevalue += 2;
		}
		else
		{
			lifevalue += 30;
		}
		monsterdata->setLifeValue(lifevalue);
		int gold = monsterdata->getGold();
		if (gold < 20)
		{
			gold -= 1;
		}
		else
		{
			gold -= 10;
		}
		if (gold < 1)
		{
			gold = 1;
		}
		monsterdata->setGold(gold);
	}
	return true;
}