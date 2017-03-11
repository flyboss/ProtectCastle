#ifndef __ProtectCastle__MonsterData__
#define __ProtectCastle__MonsterData__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

// MonsterData类，用来存储关卡的怪物信息
// "monsters": [{"name":"pest.png","lifeValue":2,"gold":15}]
class MonsterData : public Ref
{
private:
	std::string _name; // 怪物名称
	int _lifeValue;  // 怪物的生命值
	int _gold; // 消灭怪物获得的金币
public:
	// 静态的create函数
	static MonsterData* create();

	// setter和getter函数
	void setName(std::string name);

	std::string getName();

	void setLifeValue(int lifeValue);

	int getLifeValue();

	int getMonsterType();

	void setGold(int gold);

	int getGold();
};


#endif