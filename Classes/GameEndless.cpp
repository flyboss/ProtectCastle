#include "GameEndless.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

static int monsterCount = 1; // ÿһ�����ֶ��ٹ���
static int _currentLevel = 1;  // ��ǰ�ؿ�

Scene* GameEndless::createSceneWithLevel(int selectLevel)
{
	// ��ùؿ���
	_currentLevel = selectLevel;
	// ����һ���������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto scene = Scene::create();
	// ��������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
	auto layer = GameEndless::create();
	// ��Game����Ϊ�ӽڵ���ӵ�����
	scene->addChild(layer);
	// ���س�������
	return scene;
}

bool GameEndless::init()
{
	if (!initialize(_currentLevel))
	{
		return false;
	}
	_sumNum = 1;
	// �����ٲ�����
	_currNumLabel = Label::createWithSystemFont(StringUtils::format("This is the %d wave of monsters", _sumNum), "Arial", 32);
	_currNumLabel->setColor(Color3B::BLUE);
	_currNumLabel->setPosition(_screenWidth*0.5, _screenHeight*0.96);
	_tileMap->addChild(_currNumLabel, 2);
	//��ʼ��Ϸ��ѭ��
	auto countdown = Sequence::create(
		DelayTime::create(4),
		CallFunc::create([=]{
		scheduleUpdate();
	}),
		NULL);
	this->runAction(countdown);
	return true;
}

void GameEndless::update(float delta)
{
	_count++;
	updateMonster(delta);
	detectionTurret(delta);
	collisionDetection(delta);
	detectionPrincess(delta);
	updateHUD(delta);
	updateLable(delta);
}

void GameEndless::updateMonster(float delta){
	// _isFinish���Ϊfalse��ʾ���Գ��ֹ���
	if (!_isFinish) {
		if (_isThisFinish)
		{
			// _delivery����Ϊȡģ������ͨ�����øñ���ֵ�����Կ��ƹ������Ƶ��
			if (_count % _delivery == 0) {
				// ��ǰ���ֹ���������1
				_currentCount++;
				if (_currentCount == monsterCount)
				{
					_isThisFinish = false;
				}
				// �����ǰ���ֹ�����������ÿһ�����ֶ��ٹ����ʾһ������
				if (_currentCount > monsterCount) {
					// �ؿ������ܲ�����1
					_number -= 1;
					// ���ؿ������ܲ���Ϊ0
					if (_number == 0) {
						// ���ñ�ǣ���ʾ�ؿ����й���ȫ���������
						_isFinish = true;
						_sumNum++;
					}
					else{
						// ���¿�ʼһ������
						_currentCount = 0; // ��ǰ���ֹ�����������
						//						_delivery -= 5; // �ı�ȡģ���������������ٶȼӿ�
						_currNum++; // ��ǰ���ֹ��ﲨ����1
						_sumNum++;
						_monsterDatas = _monsterAllDatas.at(_currNum - 1);
						monsterCount = _monsterDatas->getCount();
						_delivery = _monsterDatas->getDelivery();
					}
				}
				else{
					// ����TMXTiledMap��getObjectGroup������ȡ�����
					auto pestObject = _tileMap->getObjectGroup("monsterObject");
					// ���ݶ������ƻ�ȡ�������Ϣ
					ValueMap pestValueMap = pestObject->getObject("monster");
					// ��ȡ��ͼ�����õ�player�����x��yֵ
					int pestX = pestValueMap.at("x").asInt();
					int pestY = pestValueMap.at("y").asInt();
					// һ���ؿ����ܳ��ֶ��ֹ��������json�ļ�������
					int random = rand() % _monsterDatas->size();
					auto monsterData = _monsterDatas->at(random);
					// ���ݹ������ƴ�������
					auto monster = Monster::create(monsterData->getName());
					_aliveCount++;

					// ���ù������ơ�����ֵ���������Ľ��ֵ�͹����������
					monster->setName(monsterData->getName());
					monster->setLifeValue(monsterData->getLifeValue());
					monster->setGold(monsterData->getGold());
					monster->setScale(0.7);
					monster->setPosition(pestX, pestY);

					// ����������
					auto loadingBar = LoadingBar::create("planeHP.png");
					// ���ý���������
					loadingBar->setDirection(LoadingBar::Direction::LEFT);
					// ���ý�������С
					loadingBar->setScale(0.06f, 0.1f);
					// ���ý������ٷֱ�
					loadingBar->setPercent(100);
					// ������������Ϊ����Ѫ��
					monster->setHP(loadingBar);
					// ��ӹ���Ѫ��
					_tileMap->addChild(monster->getHP(), 2);
					// ����Ѫ��������
					monster->setHPInterval(100 / monster->getLifeValue());
					// ��������Ϊ��ͼ�ӽڵ�
					_tileMap->addChild(monster, 2);
					// ��������ӵ��ѳ��ֹ�������
					_monsterVector.pushBack(monster);
					// �������·�����ƶ�
					monsterMoveWithWayPoints(_pathPoints, monster);
				}
			}
		}
	}
	else{
		// _isFinish���Ϊtrue��ʾ�ؿ����й����Ѿ��������
		if (_monsterVector.size() <= 0) {
			// ��Ϸʤ��
			for (int i = 0; i < _monsterAllDatas.size(); i++)
			{
				_monsterAllDatas.at(i)->upgrade();
			}
			_currentCount = 0; // ��ǰ���ֹ�����������
			_currNum = 1; // ��ǰ���ֹ��ﲨ����Ϊ1
			_monsterDatas = _monsterAllDatas.at(_currNum - 1);
			monsterCount = _monsterDatas->getCount();
			_delivery = _monsterDatas->getDelivery();
			_number = _monsterAllDatas.size();
			_isFinish = false;
		}
	}
}

void GameEndless::updateLable(float delta)
{
	// ���¹��ﲨ����
	_currNumLabel->setString(StringUtils::format("This is the %d wave of monsters", _sumNum));
}

void GameEndless::gameOver(int tag)
{
	// ֹͣ��Ϸ��ѭ��
	this->unscheduleUpdate();
	// ֹͣ���й��ﶯ��
	for (unsigned int i = 0; i < _monsterVector.size(); i++)
	{
		auto monster = _monsterVector.at(i);
		monster->stopAllActions();
	}
	// ɾ����Ļ�����ڵ�
	for (unsigned int i = 0; i < _bulletVector.size(); i++)
	{
		auto bullet = _bulletVector.at(i);
		bullet->stopAllActions();
		_tileMap->removeChild(bullet);
	}
	_bulletVector.clear();
	// ѡ����Ļ��
	auto temp = Sprite::createWithSpriteFrameName("block.png");
	temp->setPosition(_screenWidth / 2, _screenHeight / 2);
	temp->setColor(Color3B::GRAY);
	_tileMap->addChild(temp, 2);
	// ֹͣ��������
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(MUSIC_FILE);
	auto messageLabel = Label::createWithSystemFont(StringUtils::format("You have repelled [%d]\n waves of monsters!", _sumNum - 1), "Arial", 38);
	messageLabel->setColor(Color3B::WHITE);
	messageLabel->setPosition(_screenWidth / 2, _screenHeight / 2 + 50);
	_tileMap->addChild(messageLabel, 4);
	// ��ѡ��ؿ�����ť
	auto select_button = Button::create("btn-select.png");
	select_button->setPosition(Vec2(_screenWidth / 2 - 100, _screenHeight / 2 - 80));
	select_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			// �ұ߻���糡Ч��
			auto transition = TransitionSlideInR::create(2.0, LevelManager::createScene());
			// push��Ϸ����
			Director::getInstance()->pushScene(transition);
		}
	});
	this->addChild(select_button, 3);

	// ������һ�Ρ���ť
	auto play_button = Button::create();
	// ������Ϸʧ����Ч
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE2);
	play_button->loadTextures("btn-again.png", "btn-again.png");

	play_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED){
			// TransitionSplitRows ���һ����糡Ч��
			auto transition = TransitionSplitRows::create(2.0, GameEndless::createSceneWithLevel(_currentLevel));
			// push��Ϸ����
			Director::getInstance()->pushScene(transition);
		}
	});
	play_button->setPosition(Vec2(_screenWidth / 2 + 100, _screenHeight / 2 - 80));
	this->addChild(play_button, 3);
}
