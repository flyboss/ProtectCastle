#ifndef __ProtectCastle__MonsterBoss__
#define __ProtectCastle__MonsterBoss__

#include"MonsterBase.h"
#include"Monster2.h"
class MonsterBoss:public Monster2
{
private:
	Monster2* _Boss;
	const int dodge = 40;
public:
	MonsterBoss() {};
	MonsterBoss(Monster2* mBase);
	~MonsterBoss() {};
};
#endif