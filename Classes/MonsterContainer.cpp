#include "MonsterContainer.h"
#include <vector>


MonsterContainer::MonsterContainer()
{

}


MonsterContainer::~MonsterContainer()
{
}
void MonsterContainer::loadMonster(Game& game, rapidjson::Value& monstersArray) {



	for (int i = 0; i < monstersArray.Size(); i++) {
		auto monsterDatas = MonsterDataOfWave::create();
		int count = monstersArray[i]["count"].GetInt();
		monsterDatas->setCount(count);
		int delivery = monstersArray[i]["delivery"].GetInt();
		monsterDatas->setDelivery(delivery);
		const rapidjson::Value& monsterArray = monstersArray[i]["monster"];
		//		_monsterDatas.clear();
		for (int j = 0; j < monsterArray.Size(); j++) {
			// 获得每一种怪物的name、lifeValue和gold数据
			std::string name = monsterArray[j]["name"].GetString();
			int lifeValue = monsterArray[j]["lifeValue"].GetInt();
			int gold = monsterArray[j]["gold"].GetInt();
			// 将获得的name、lifeValue和gold数据存储到自定义MonsterData对象当中
			auto monsterData = MonsterData::create();
			monsterData->setName(name);
			monsterData->setLifeValue(lifeValue);
			monsterData->setGold(gold);
			// 将MonsterData存储到关卡怪物数据的集合当中
			monsterDatas->pushBack(monsterData);
		}
		game._monsterAllDatas.pushBack(monsterDatas);
	}
	game._monsterDatas =game. _monsterAllDatas.at(0);
	game._delivery =game._monsterDatas->getDelivery();

}
void MonsterContainer::updateMonster(float delta, Game& game,int monsterCount) {

	// _isFinish标记为false表示可以出现怪物
	if (! game._isFinish) {
		if (game._isThisFinish)
		{
			// _delivery变量为取模基数，通过设置该变量值，可以控制怪物出现频率
			if (game._count %game. _delivery == 0) {
				// 当前出现怪物数量加1
				game._currentCount++;
				if (game._currentCount == monsterCount)
				{
					game._isThisFinish = false;
				}
				// 如果当前出现怪物数量大于每一波出现多少怪物，表示一波结束
				if (game._currentCount > monsterCount) {
					// 关卡怪物总波数减1
					game._number -= 1;
					// 当关卡怪物总波数为0
					if (game._number == 0) {
						// 设置标记，表示关卡所有怪物全部出现完毕
						game._isFinish = true;
					}
					else {
						// 重新开始一波怪物
						game._currentCount = 0; // 当前出现怪物数量归零
										   //						_delivery -= 5; // 改变取模基数，则怪物出现速度加快
						game._currNum++; // 当前出现怪物波数加1
						game._monsterDatas = game._monsterAllDatas.at(game._currNum - 1);
						monsterCount =game. _monsterDatas->getCount();
						game._delivery =game. _monsterDatas->getDelivery();
					}
				}
				else {
					// 调用TMXTiledMap的getObjectGroup函数获取对象层
					auto pestObject = game._tileMap->getObjectGroup("monsterObject");
					// 根据对象名称获取对象的信息
					ValueMap pestValueMap = pestObject->getObject("monster");
					// 获取地图中设置的player对象的x和y值
					int pestX = pestValueMap.at("x").asInt();
					int pestY = pestValueMap.at("y").asInt();
					/**
					* 一个关卡可能出现多种怪物，可以在json文件中设置
					* 随机创建怪物精灵，并将地图中对象的x和y值作为怪物出现的位置
					*/
					int random = rand() % game._monsterDatas->size();
					auto monsterData = game._monsterDatas->at(random);

					game._aliveCount++;
					MonsterFactory* mstFcty = new MonsterFactory();
					MonsterBase* monster = mstFcty->getMonster(monsterData);
					monster->setPosition(pestX, pestY);
					// 添加怪物血条
					game._tileMap->addChild(monster->getHP(), 2);
					// 计算血条更新量
					monster->setHPInterval(100 / monster->getLifeValue());
					// 将怪物添为地图子节点
					game._tileMap->addChild(monster, 2);
					// 将怪物添加到已出现怪物数组
					game._monsterVector.pushBack(monster);
					// 怪物根据路径点移动
					game.monsterMoveWithWayPoints(game._pathPoints, monster);
				}
			}
		}
	}
	else {
		// _isFinish标记为true表示关卡所有怪物已经出现完毕
		if (game._monsterVector.size() <= 0) {
			// 游戏胜利
			game.gameOver(1);
		}
	}
}


