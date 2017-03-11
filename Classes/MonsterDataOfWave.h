#ifndef __ProtectCastle__MonsterDataOfWave__
#define __ProtectCastle__MonsterDataOfWave__

#include <iostream>
#include "cocos2d.h"
#include "MonsterData.h"

class MonsterDataOfWave : public Ref
{
private:
	int _monsterCount;
	int _monsterDelivery;
	Vector<MonsterData*> _monsterDatas;
public:
	static MonsterDataOfWave* create();

	MonsterData* at(int pos);

	bool pushBack(MonsterData* monsterdata);

	unsigned size();

	bool setCount(int monstercount);

	int getCount();

	bool setDelivery(int monsterdelivery);

	int getDelivery();

	bool upgrade();
};
#endif