#include"MonsterFactory.h"



		 MonsterBase* MonsterFactory::getMonster(MonsterData* monsterData)
{
			 MonsterBase* monster;
			 if(monsterData->getMonsterType()==1)
			 { 
				 monster = Monster1::create(monsterData->getName());
			 }
			 else
			 {
				 monster = Monster2::create(monsterData->getName());
			 }
		// 根据怪物名称创建怪物
		
		

		// 设置怪物名称、生命值、消灭怪物的金币值和怪物出现坐标
		monster->setName(monsterData->getName());
		monster->setLifeValue(monsterData->getLifeValue());


		monster->setGold(monsterData->getGold());
		monster->setScale(0.7);
		

		// 创建进度条
		auto loadingBar = LoadingBar::create("planeHP.png");
		// 设置进度条方向
		loadingBar->setDirection(LoadingBar::Direction::LEFT);
		// 设置进度条大小
		loadingBar->setScale(0.06f, 0.1f);
		// 设置进度条百分比
		loadingBar->setPercent(100);
		// 将进度条设置为怪物血条
		monster->setHP(loadingBar);

		return monster;

}
