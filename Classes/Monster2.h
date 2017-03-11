#ifndef __ProtectCastle__Monster2__
#define __ProtectCastle__Monster2__


#include"MonsterBase.h"
class Monster2 :public MonsterBase
{
private:
	int _lifeValue;  // 怪物的生命值
	LoadingBar* _HP; // 怪物血条
	double _HPInterval; // 怪物血条的更新量
	std::string _name; // 怪物名称
	int _gold; // 消灭怪物获得的金币
	int armor = 0;
	const int dodge=40;
public:
	Monster2();
	~Monster2();
	static Monster2* create(const std::string& filename);
	static Monster2* createWithSpriteFrameName(const std::string& filename);
	//闪避
	void getInjured();
};

#endif