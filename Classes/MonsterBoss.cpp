#include "MonsterBoss.h"

MonsterBoss::MonsterBoss(Monster2* mBase)
{
	mBase->setLifeValue(mBase->getLifeValue() + 20);
}