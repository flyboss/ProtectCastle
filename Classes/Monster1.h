#ifndef __ProtectCastle__Monster1__
#define __ProtectCastle__Monster1__


#include"MonsterBase.h"
class Monster1:public MonsterBase
{
private:
	int _lifeValue;  // 怪物的生命值
	LoadingBar* _HP; // 怪物血条
	double _HPInterval; // 怪物血条的更新量
	std::string _name; // 怪物名称
	int _gold; // 消灭怪物获得的金币
public:
	Monster1();
	~Monster1();
	static Monster1* create(const std::string& filename);
	static Monster1* createWithSpriteFrameName(const std::string& filename);
	void getInjured() ;
};

#endif