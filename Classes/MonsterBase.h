#ifndef __ProtectCastle__MonsterBase__
#define __ProtectCastle__MonsterBase__

#include <iostream>
#include "cocos2d.h"
#include "SpriteBase.h"
#include "ui/CocosGUI.h"
using namespace ui;
class MonsterBase: public SpriteBase
{
private:
	int _lifeValue;  // 怪物的生命值
	LoadingBar* _HP; // 怪物血条
	double _HPInterval; // 怪物血条的更新量
	std::string _name; // 怪物名称
	int _gold; // 消灭怪物获得的金币
public:
	MonsterBase();

	~MonsterBase();

	// 静态的create函数
	//virtual MonsterBase* create(const std::string& filename)=0 ;

	//virtual MonsterBase* createWithSpriteFrameName(const std::string& filename)=0 ;

	// setter和getter函数
	void setName(std::string name);

	std::string getName();

	void setGold(int gold);

	int getGold();

	void setLifeValue(int lifeValue);

	int getLifeValue();

	void setHP(LoadingBar* HP);

	LoadingBar* getHP();

	void setHPInterval(double HPInterval);

	float getHpInterval();


	//接口
	virtual void getInjured() = 0;
};
#endif
