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
			// ���ÿһ�ֹ����name��lifeValue��gold����
			std::string name = monsterArray[j]["name"].GetString();
			int lifeValue = monsterArray[j]["lifeValue"].GetInt();
			int gold = monsterArray[j]["gold"].GetInt();
			// ����õ�name��lifeValue��gold���ݴ洢���Զ���MonsterData������
			auto monsterData = MonsterData::create();
			monsterData->setName(name);
			monsterData->setLifeValue(lifeValue);
			monsterData->setGold(gold);
			// ��MonsterData�洢���ؿ��������ݵļ��ϵ���
			monsterDatas->pushBack(monsterData);
		}
		game._monsterAllDatas.pushBack(monsterDatas);
	}
	game._monsterDatas =game. _monsterAllDatas.at(0);
	game._delivery =game._monsterDatas->getDelivery();

}
void MonsterContainer::updateMonster(float delta, Game& game,int monsterCount) {

	// _isFinish���Ϊfalse��ʾ���Գ��ֹ���
	if (! game._isFinish) {
		if (game._isThisFinish)
		{
			// _delivery����Ϊȡģ������ͨ�����øñ���ֵ�����Կ��ƹ������Ƶ��
			if (game._count %game. _delivery == 0) {
				// ��ǰ���ֹ���������1
				game._currentCount++;
				if (game._currentCount == monsterCount)
				{
					game._isThisFinish = false;
				}
				// �����ǰ���ֹ�����������ÿһ�����ֶ��ٹ����ʾһ������
				if (game._currentCount > monsterCount) {
					// �ؿ������ܲ�����1
					game._number -= 1;
					// ���ؿ������ܲ���Ϊ0
					if (game._number == 0) {
						// ���ñ�ǣ���ʾ�ؿ����й���ȫ���������
						game._isFinish = true;
					}
					else {
						// ���¿�ʼһ������
						game._currentCount = 0; // ��ǰ���ֹ�����������
										   //						_delivery -= 5; // �ı�ȡģ���������������ٶȼӿ�
						game._currNum++; // ��ǰ���ֹ��ﲨ����1
						game._monsterDatas = game._monsterAllDatas.at(game._currNum - 1);
						monsterCount =game. _monsterDatas->getCount();
						game._delivery =game. _monsterDatas->getDelivery();
					}
				}
				else {
					// ����TMXTiledMap��getObjectGroup������ȡ�����
					auto pestObject = game._tileMap->getObjectGroup("monsterObject");
					// ���ݶ������ƻ�ȡ�������Ϣ
					ValueMap pestValueMap = pestObject->getObject("monster");
					// ��ȡ��ͼ�����õ�player�����x��yֵ
					int pestX = pestValueMap.at("x").asInt();
					int pestY = pestValueMap.at("y").asInt();
					/**
					* һ���ؿ����ܳ��ֶ��ֹ��������json�ļ�������
					* ����������ﾫ�飬������ͼ�ж����x��yֵ��Ϊ������ֵ�λ��
					*/
					int random = rand() % game._monsterDatas->size();
					auto monsterData = game._monsterDatas->at(random);

					game._aliveCount++;
					MonsterFactory* mstFcty = new MonsterFactory();
					MonsterBase* monster = mstFcty->getMonster(monsterData);
					monster->setPosition(pestX, pestY);
					// ��ӹ���Ѫ��
					game._tileMap->addChild(monster->getHP(), 2);
					// ����Ѫ��������
					monster->setHPInterval(100 / monster->getLifeValue());
					// ��������Ϊ��ͼ�ӽڵ�
					game._tileMap->addChild(monster, 2);
					// ��������ӵ��ѳ��ֹ�������
					game._monsterVector.pushBack(monster);
					// �������·�����ƶ�
					game.monsterMoveWithWayPoints(game._pathPoints, monster);
				}
			}
		}
	}
	else {
		// _isFinish���Ϊtrue��ʾ�ؿ����й����Ѿ��������
		if (game._monsterVector.size() <= 0) {
			// ��Ϸʤ��
			game.gameOver(1);
		}
	}
}


