#ifndef __ProtectCastle__MonsterFactory__
#define __ProtectCastle__MonsterFactory__

#include"MonsterBase.h"
#include "Monster1.h"
#include"Monster2.h"
#include"MonsterBoss.h"
#include"Monster.h"
#include "MonsterData.h"
using namespace std;
class MonsterFactory {

public:  
	MonsterBase* getMonster(MonsterData* monsterData);
};
#endif